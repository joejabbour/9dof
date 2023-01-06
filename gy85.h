//gy85.h

#include <stdint.h>

void accelset (uint8_t,uint8_t);
void gyroset (uint8_t,uint8_t);
void magset (uint8_t,uint8_t);
void accelread (int16_t*);
void gyroread (int16_t*);
void magread (int16_t*);
void gy85setup (void);
