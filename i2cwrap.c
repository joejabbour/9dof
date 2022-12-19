//i2cwrap.c

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#define I2C_DEVICE 1

int iic_fd=2;

void i2cinit (void)
{
	char iicdev[20];
	snprintf(iicdev,19,"/dev/i2c-%d",I2C_DEVICE);
	
	iic_fd = open(iicdev,O_RDWR);

	if (iic_fd < 0)
	{ 
		printf("something wrong with open iic_fd\n");
		exit(EXIT_FAILURE);
	}
	
	printf ("open success iic_fd = %i\n", iic_fd);
}

void i2cregwrite (int address, uint8_t reg, uint8_t data)
{
    if(ioctl(iic_fd,I2C_SLAVE_FORCE,address) < 0)
    {
	    printf("ioctl: failed to find device %x for fd = %i \n error: %s \n",address,iic_fd,strerror(errno));
	    exit(EXIT_FAILURE);
    }

    uint8_t message[2];
    message[0] = reg;
    message[1] = data;
    if (write(iic_fd,message,2) != 2)
    {
	    printf("something wrong with write\n");
    }
}

void i2cread ( int address, uint8_t startreg, uint8_t *buff, int cnt)
{
	if(ioctl(iic_fd,I2C_SLAVE_FORCE,address)<0)
	{
		printf("ioctl: failed to find device %x\n",address);
	}

	if(write(iic_fd,&startreg,1) != 1)
	{
		printf("write did not return 1\n");
	}

	if(read(iic_fd,buff,cnt) != cnt)
	{
		printf ("read did not return %i bytes\n",cnt);
	}
}

void i2cclose(void)
{
	close(iic_fd);
}









