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

//Accel registers
#define ACCEL_POWER_CTL 0x2d
#define ACCEL_OFSX 0x1e
#define ACCEL_OFSY 0x1f
#define ACCEL_OFSZ 0x20

//gyro registers
#define GYRO_POWER_MGM 0x3e
#define GYRO_SAMPLERATE_DIV 0x15
#define GYRO_INT_CFG 0x17
#define GYRO_LPF_FS 0x16

//mag registers
#define MAG_CONFIGREG_A 0x00
#define MAG_CONFIGREG_B 0x01
#define MAG_MODEREG 0x02



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

void gy85setup (void)
{
	//accel
	accelset(0x2d,0x08);

	//gyro
	
	//mag
	magset(0x02,0x00); //continuous measurement mode
}

