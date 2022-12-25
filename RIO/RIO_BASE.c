#include "RIO_BASE.h" 

static uint8_t registers[255] __attribute__ ((aligned (4))) = { 1, };

int8_t RIO_REG_DISPLAYMODE_Get(void){
    return *((int8_t *)&registers[0]);
}
void RIO_REG_DISPLAYMODE_Set(int8_t value){
    *((int8_t *)&registers[0]) = value;
}

