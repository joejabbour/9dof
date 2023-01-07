gy85: gy85.c main.c i2cwrap.c
	gcc -o gy85 gy85.c i2cwrap.c main.c -lm

accelcalibrate: gy85.c i2cwrap.c accelcalibrate.c 
	gcc -o accelcalibrate gy85.c i2cwrap.c accelcalibrate.c 
