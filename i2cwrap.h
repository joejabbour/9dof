//i2cwrap.h

#include <stdint.h>

void i2cinit (void);
void i2cregwrite (uint8_t, uint8_t, uint8_t);
void i2cread (uint8_t, uint8_t, uint8_t*,int);
void i2cclose (void);


