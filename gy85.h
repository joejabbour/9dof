//gy85.h

#include <stdint.h>

void accelset (uint8_t,uint8_t);
void gyroset (uint8_t,uint8_t);
void magset (uint8_t,uint8_t);
void accelread (uint8_t*);
void gyroread (uint8_t*);
void magread (uint8_t*);
void gy85setup (void);
