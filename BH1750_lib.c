#ifndef I2CLIB
    #define I2CLIB
    #include "I2C_lib.h"
#endif

#ifndef F_CPU
#define F_CPU 9600000UL  
#endif

#include "BH1750_lib.h"

#include <util/delay.h>
/*                              убрано ради уменьшения веса. можно вернуть.
void BH1750_initLow(void) {
    i2csw_start();
    i2csw_byte(ADDR << 1);
    i2csw_byte(0x01);
    i2csw_stop();
    _delay_us(120);     //подождать чтоб прочихался
    i2csw_start();
    i2csw_byte(ADDR << 1);
    i2csw_byte(0x13);
    i2csw_stop();
    _delay_us(120);     //подождать чтоб прочихался
}
*/
void BH1750_initHigh(void) {
    i2csw_start();
    i2csw_byte(ADDR << 1);
    i2csw_byte(0x01);
    i2csw_stop();
    _delay_us(150);     //подождать чтоб прочихался
    i2csw_start();
    i2csw_byte(ADDR << 1);
    i2csw_byte(0x10);
    i2csw_stop();
    _delay_us(120);     //подождать чтоб прочихался
}


uint16_t BH1750_getLux(void)
{
    uint16_t _data;
    i2csw_start();
    i2csw_byte((ADDR << 1) | (1 << 0));    
    _data = i2csw_readACK() << 8;
    _data |= i2csw_readNAK();
    i2csw_stop();
    
    return _data;
    
 
}

