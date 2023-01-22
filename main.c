// main.c


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include </home/pi/9dof/i2cwrap.h>
#include </home/pi/9dof/gy85.h>
#include <signal.h>
#include <math.h>

#define ACCEL_POWER_CTL 0x2d // write 0x08 to start measuring

#define GYRO_POWER_MGM 0x3e
#define GYRO_SAMPLERATE_DIV 0x15
#define GYRO_INT_CFG 0x17
#define GYRO_LPF_FS 0X16

#define MAG_CONFIGREG_A 0x00
#define MAG_CONFIGREG_B 0x01
#define MAG_MODEREG 0x02


void sigint_handler(int sig_num)
{
	i2cclose();
	exit(0);
}

int main()
{
	signal(SIGINT,sigint_handler);

	printf("here we go <enhanced>\n");
	i2cinit();
	gy85setup();
	sleep(0.1);     



	int16_t accelcal[3];
	int16_t gyrocal[3];
	int16_t magcal[3];

	float accelx;
	float accely;
	float accelz;

 	float magx;
	float magy;
	float magz;
	

	float gyrox;
	float gyroy;
	float gyroz;
        float gyroxoffset;
        float gyroyoffset;
	float gyrozoffset;	
     
	float angleyx;
	float anglezx;

        gyroaverage(gyrocal,10); //offset calibration for gyro

	gyroxoffset = gyrocal[0]/14.375;
	gyroyoffset = gyrocal[1]/14.375;
	gyrozoffset = gyrocal[2]/14.375;

	while(1)
	
	{

   	accelread(accelcal);
	
	
	gyroaverage(gyrocal,10);
	

	magread(magcal);
        
        
        //ACCELOMETER 


	accelx = accelcal[0]*(0.039); //  multiply by (scalefactor/1000) for default 10-bit resolution +-2g or 9.81/256
        accely = accelcal[1]*(0.039);
	accelz = accelcal[2]*(0.039);

        //GYROSCOPE
	

	gyrox = (gyrocal[0]/14.375) - gyroxoffset;
	gyroy = (gyrocal[1]/14.375) - gyroyoffset;
	gyroz = (gyrocal[2]/14.375) - gyrozoffset;

	//MAGNETOMETER
	

	magx = magcal[0] ;
	magy = magcal[1] ;
	magz = magcal[2] ;

	angleyx = ((float)atan(magy/magx) * 180) / 3.141592 ;
	anglezx = ((float)atan(magz/magx) * 180) / 3.141592 ;
       
        //we re going up, accel positive in z direction 
	printf("accel calc: %f %f %f \n" ,accelx,accely,accelz);
	printf("gyro calc: %f %f %f \n" ,gyrox,gyroy,gyroz);
	printf("mag yx: %f  mag zx: %f \n" ,angleyx ,anglezx);
        	
        	
        	
	
	sleep(1); 
	system("clear");
        }


	i2cclose();

	return 0;
}

