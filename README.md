# 9dof IMU
 This is to fetch data from gy-85 module, using i2c to get data from 3 measurement units (accelometer, gyroscope, magnetometer) from raspberry pi.
This still needs some optimization but i have been busy so hopefully i will find the time to finish.
i2cwrapper.c is used around ioctl() ,read() and write() sys calls the i2c driver of raspberry pi will handle the rest.
gy85.c is a wrapper around i2cwrapper.c for gy85 module.

In the mainthr.c program, interval timer will execute sequencer() signal handler, which in turns release semaphore on the thread (SCHED_FIFO highest priority on core 4) 
that will fetch i2c data.
User can choose the interval time depending on application.
data is still noisy, i still have to implement digital filters.
also the delta function is recording 18ms on each complete data fetching cycle, so we have a delay of 18ms on each interval. I might have to use SPI instead of i2c since it s faster.
