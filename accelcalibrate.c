// this is to display on standard output offset values and scales for accelometer


#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include </home/pi/9dof/gy85.h>
#include </home/pi/9dof/i2cwrap.h>

int16_t xmax,ymax,zmax,xmin,ymin,zmin = 0;


void sigint_handler (int sig_num)
{
	int xoffset = (xmax + xmin)/2;
	int yoffset = (ymax + ymin)/2;
	int zoffset = (zmax + zmin)/2;

	float scalex =255 / (float)((xmax - xmin)/2); 
	float scaley =255 / (float)((ymax - ymin)/2);
	float scalez =255 / (float)((zmax - zmin)/2);

	printf ("\nxoffset: %i yoffset: %i zoffset: %i\n scalex: %f scaley: %f scalez %f\n",xoffset,yoffset,zoffset,scalex,scaley,scalez);
	
	exit(0);
}

int main()
{
	signal(SIGINT,sigint_handler);
	int16_t accelval [3] = {0};
	int8_t offset [3] = {0}; //0 for x 1 for y 2 for z;
	
	i2cinit();
	gy85setup();
	sleep(0.1);

	while(1)
	{

	accelaverage(accelval,5);
	if ( accelval[0] > xmax )
		xmax = accelval[0];
	if ( accelval[0] < xmin )
		xmin = accelval[0];
	
	if ( accelval[1] > ymax )
		ymax = accelval[1];
	if ( accelval[1] < ymin )
		ymin = accelval[1];
	
	if ( accelval[2] > zmax )
		zmax = accelval[2];
	if ( accelval[2] < zmin )
		zmin = accelval[2];
	
	printf("accelx: %d accely: %d accelz %d \nxmin: %d ymin: %d zmin: %d\nxmax: %d ymax: %d zmax: %d \n", accelval[0] ,accelval[1], accelval[2],xmin,ymin,zmin,xmax,ymax,zmax);
	sleep(1);
	system("clear");
	}
}
