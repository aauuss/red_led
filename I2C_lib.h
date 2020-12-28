/*
 * Cамая лучшая библиотека для I2C
 */


#include <avr/io.h>

#define SCL		PB0   // ----> [SCL]	Pin 3 on the SSD1306 display board
#define SDA		PB1   // ----> [SDA]	Pin 4 on the SSD1306 display board
                              //                ATTiny85

#define I2CSW_HIGH(PORT) PORTB |= (1 << PORT)
#define I2CSW_LOW(PORT) PORTB &= ~(1 << PORT)


void i2csw_start(void);         
void i2csw_stop(void);          
void i2csw_byte(uint8_t byte);
uint8_t i2csw_read(void);
uint8_t i2csw_readACK(void);
uint8_t i2csw_readNAK(void);
