#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include </home/pi/9dof/i2cwrap.h>



//7-bits addresses
#define ACCEL 0x53
#define GYRO  0x68
#define MAG   0x1e


void accelset (uint8_t reg, uint8_t value)
{
	i2cregwrite(ACCEL,reg,value);
}

void gyroset (uint8_t reg, uint8_t value)
{
	i2cregwrite(GYRO,reg,value);
}

void magset (uint8_t reg, uint8_t value)
{
	i2cregwrite(MAG,reg,value);
}


void accelread (uint8_t *buf)
{	
	i2cread(ACCEL,0x32,buf,6);
}

void gyroread (uint8_t *buf)
{
	i2cread(GYRO,0x1d,buf,6);
}

void magread (uint8_t *buf)
{
	i2cread(MAG,0x03,buf,6);
}



