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
	printf("\n Signal handler, closing i2c fd\n");
	i2cclose();
	printf("Done!\n");
	exit(0);
}

int main()
{
	signal(SIGINT,sigint_handler);

	printf("here we go <enhanced>\n");
	i2cinit();
	gy85setup();
	sleep(0.1);     


  	uint8_t acceldata [6] = {0};
 	uint8_t gyrodata [6] = {0};
        uint8_t magdata [6] = {0};

	int16_t accelcal [3];
	int16_t gyrocal [3];
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

        gyroread(gyrodata); //offset calibration for gyro
	gyrocal[0] = (gyrodata[0] << 8) + gyrodata [1];// first value is msb
	gyrocal[1] = (gyrodata[2] << 8) + gyrodata [3];
	gyrocal[2] = (gyrodata[4] << 8) + gyrodata [5];

	gyroxoffset = gyrocal[0]/14.375;
	gyroyoffset = gyrocal[1]/14.375;
	gyrozoffset = gyrocal[2]/14.375;

	while(1)
	
	{

   	accelread(acceldata);
	gyroread(gyrodata);
	magread(magdata);
        
        printf("Raw: \n");	
	printf("accel : %x %x %x %x %x %x \n",acceldata[0],acceldata[1],acceldata[2],acceldata[3],acceldata[4],acceldata[5]);	
        printf("gyro : %x %x %x %x %x %x \n",gyrodata[0],gyrodata[1],gyrodata[2],gyrodata[3],gyrodata[4],gyrodata[5]);
     	printf("mag : %x %x %x %x %x %x \n\n",magdata[0],magdata[1],magdata[2],magdata[3],magdata[4],magdata[5]);	
        
        //ACCELOMETER 

	accelcal[0] = (acceldata[1] << 8) + acceldata [0];// first value is least significant byte of x
	accelcal[1] = (acceldata[3] << 8) + acceldata [2];
	accelcal[2] = (acceldata[5] << 8) + acceldata [4];

	accelx = (float)accelcal[0]*(9.8/256); // or we can multiply by (scalefactor/1000) for default 10-bit resolution +-2g 
        accely = (float)accelcal[1]*(9.8/256);
	accelz = (float)accelcal[2]*(9.8/256);

        //GYROSCOPE
	
	gyrocal[0] = (gyrodata[0] << 8) + gyrodata [1];// first value is msb
	gyrocal[1] = (gyrodata[2] << 8) + gyrodata [3];
	gyrocal[2] = (gyrodata[4] << 8) + gyrodata [5];

	gyrox = (gyrocal[0]/14.375) - gyroxoffset;
	gyroy = (gyrocal[1]/14.375) - gyroyoffset;
	gyroz = (gyrocal[2]/14.375) - gyrozoffset;

	//MAGNETOMETER
	
	magcal[0] = (magdata[0] << 8) + magdata[1]; //first value is MSB of X
	magcal[1] = (magdata[2] << 8) + magdata[3];
	magcal[2] = (magdata[4] << 8) + magdata[5];

	magx = (float)magcal[0] ;
	magy = (float)magcal[1] ;
	magz = (float)magcal[2] ;

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

