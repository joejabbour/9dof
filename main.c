// main.c


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include </home/pi/9dof/i2cwrap.h>
#include </home/pi/9dof/gy85.h>
#include <signal.h>
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
     


  	uint8_t acceldata [6] = {0};
 	uint8_t gyrodata [6] = {0};
        uint8_t magdata [6] = {0};

	int16_t accelcal [3];
	int16_t gyrocal [3];
	int16_t magcal[3];

	float accelx;
	float accely;
	float accelz;

        accelset(0x2d,0x08);
	magset(0x02,0x00); //continuous measurement mode
     

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

	accelcal[0] = (acceldata[1] << 8) + acceldata [0];
	accelcal[1] = (acceldata[3] << 8) + acceldata [2];
	accelcal[2] = (acceldata[5] << 8) + acceldata [4];

	accelx = (float)accelcal[0]*(9.8/256);
        accely = (float)accelcal[1]*(9.8/256);
	accelz = (float)accelcal[2]*(9.8/256);

        //GYROSCOPE
	

	//Ma
	



        //we re going up, accel positive in z direction 
	printf("accel cal: %f %f %f \n" ,accelx,accely,accelz);
        	
	
	sleep(1); 
	system("clear");
        }


	i2cclose();

	return 0;
}

