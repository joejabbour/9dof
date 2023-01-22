//includes for 
//timers
//semaphores
//PTHREADS
//signals
//printf
//exit
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include </home/pi/9dof/i2cwrap.h>
#include </home/pi/9dof/gy85.h>
#include <signal.h>
#include <math.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>

#define ACCEL_POWER_CTL 0x2d // write 0x08 to start measuring

#define GYRO_POWER_MGM 0x3e
#define GYRO_SAMPLERATE_DIV 0x15
#define GYRO_INT_CFG 0x17
#define GYRO_LPF_FS 0X16

#define MAG_CONFIGREG_A 0x00
#define MAG_CONFIGREG_B 0x01
#define MAG_MODEREG 0x02


#define THREAD_NUM 1


typedef struct {
	int number;
} thread_arg;


pthread_t threadID[THREAD_NUM];
thread_arg thrarg [THREAD_NUM];
pthread_attr_t threadattr[THREAD_NUM];




static timer_t timer_1;
static struct itimerspec itime={{1,0},{1.0}};
static struct itimerspec last_itime;
int flags=0;

sem_t sem1,sem2;

int abort=false;

void sequencer (int id);
void inthandler (int); 



void * getimu (void * threadarg)
{
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


	while(!abort)
	{
		sem_wait(&sem1);
	
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

		system("clear");
	
	}

	
	pthread_exit((void*)0);
}



int main()
{
	signal(SIGINT,inthandler);
	
	i2cinit();
	gy85setup();
	sleep(0.1);

	cpu_set_t CPUSET;
	CPU_ZERO(&CPUSET);
	CPU_SET(3,&CPUSET);

	int maxprio; //maximum priority for sched fifo
	maxprio=sched_get_priority_max(SCHED_FIFO);

	struct sched_param param [THREAD_NUM];
	
	for(int i=0;i<THREAD_NUM;i++)
	{
		pthread_attr_init (&threadattr[i]);
		pthread_attr_setinheritsched (&threadattr[i],PTHREAD_EXPLICIT_SCHED);
		pthread_attr_setscope (&threadattr[i],PTHREAD_SCOPE_SYSTEM);
		pthread_attr_setschedpolicy(&threadattr[i],SCHED_FIFO);
		param[i].sched_priority=maxprio-i-1;
		pthread_attr_setchedparam(&threadattr[i],&param[i]);
		pthread_attr_setaffinity_np(&threadattr[i],sizeof(cpu_set_t),&CPUSET);
	}

	if(sem_init(&sem1,0,0)) {printf("Failed to init. semaphore s1.\n");exit(-1);}
	if(sem_init(&sem2,0,0)) {printf("Failed to init. semaphore s2.\n");exit(-1);} //not used now


	thrarg[0].number=0;

	pthread_create(&threadID[0],
			&threadattr[0],
			getimu,
			(void *)&thrarg[0]);




	//timer
	timer_create(CLOCK_REALTIME,NULL,&timer_1);
	signal(SIGALRM,(void(*)()) sequencer); //clock realtime sends SIGALRM

        //3hz
	itime.it_interval.tv_sec=0;
	itime.it_interval.tv_nsec=333333333;
	itime.it_value.tv_sec=0;
	itime.it_value.tv_nsec=333333333;

	timer_settime(timer_1,flags,&itime,&last_itime);

	
	pthread_join(threadID[0],NULL);


	itime.it_interval.tv_sec=0;
	itime.it_interval.tv_nsec=0;
	itime.it_value.tv_sec=0;
	itime.it_value.tv_nsec=0;

	timer_settime(timer_1,flags,&itime,&last_itime);

	i2cclose();
	printf("the end reached\n.");	
	return 0;
}

void sequencer (int id)      //signal handler ,executes every 333ms

{
	printf("sequencer: %d \n",seqcount);

	if ((seqcount%10)==0) { sem_post(&sem1);}  //

	if (seqcount == 10){
		seqcount=0;  //this is since it s only one thread,in case added threads,
	}                    //seqcount is set to zero each time it reaches Greatest Common
  	                     // Factor to not overflow seqcount.
seqcount++;
}

void inthandler (int sig)
{
	abort=true;
}
