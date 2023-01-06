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


void accelread (int16_t *accelval)
{	
	uint8_t buf [6];
	i2cread(ACCEL,0x32,buf,6);

	accelval[0] = (buf[1] << 8) + buf [0];
	accelval[1] = (buf[3] << 8) + buf [2];
	accelval[2] = (buf[5] << 8) + buf [4];
}

void gyroread (int16_t *gyroval)
{
	uint8_t buf [6];
	i2cread(GYRO,0x1d,buf,6);

	gyroval[0] = (buf[0] << 8) + buf [1];
	gyroval[1] = (buf[2] << 8) + buf [3];
	gyroval[2] = (buf[4] << 8) + buf [5];
}

void magread (int16_t *magval)
{
	uint8_t buf [6];
	i2cread(MAG,0x03,buf,6);

	magval[0] = (buf [0] << 8) + buf[1];
	magval[1] = (buf [2] << 8) + buf[3];
	magval[2] = (buf [4] << 8) + buf[5];
}

void gy85setup (void)
{
	//accel
	accelset(0x2d,0x08);

	//gyro
	gyroset(0x16,0x03);

	//mag
	magset(0x02,0x00); //continuous measurement mode
}

