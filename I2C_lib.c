#include "I2C_lib.h"

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>

#include <util/delay.h>

void i2csw_start(void) {
	DDRB |= (1 << SDA);	// Set port as output
	DDRB |= (1 << SCL);	// Set port as output
	I2CSW_HIGH(SCL);	// Set to HIGH
	I2CSW_HIGH(SDA);	// Set to HIGH
	I2CSW_LOW(SDA);		// Set to LOW
	I2CSW_LOW(SCL);		// Set to LOW
}

void i2csw_stop(void) {
    DDRB |= (1 << SDA);
    DDRB |= (1 << SCL);
	I2CSW_LOW(SCL);		// Set to LOW
	I2CSW_LOW(SDA);		// Set to LOW
	I2CSW_HIGH(SCL);	// Set to HIGH
	I2CSW_HIGH(SDA);	// Set to HIGH
	DDRB &= ~(1 << SDA);// Set port as input
}

void i2csw_byte(uint8_t byte) {
	uint8_t i;
    DDRB |= (1 << SDA);
    DDRB |= (1 << SCL);
	for (i = 0; i < 8; i++) {
		if ((byte << i) & 0x80)
			I2CSW_HIGH(SDA);
		else
			I2CSW_LOW(SDA);
		I2CSW_HIGH(SCL);
        _delay_us(1);
		I2CSW_LOW(SCL);
	}
	I2CSW_HIGH(SDA);
    DDRB &= ~(1 << SDA);        //здесь должене прийти бит ACK
	I2CSW_HIGH(SCL);
    _delay_us(1);
	I2CSW_LOW(SCL);
    _delay_us(1);                   //по хорошему тут надо анализировать, не прижал ли slave ногу SCL к земле
    DDRB |= (1 << SDA);

}

uint8_t i2csw_read(void) {
    uint8_t _data = 0x00;
    I2CSW_HIGH(SDA);    //???
    DDRB &= ~(1 << SDA);        //ставим ногу SDA на прием
    PORTB |= (1 << SDA);
    DDRB |= (1 << SCL);
	for (uint8_t i = 0; i < 8; i++) {
        I2CSW_HIGH(SCL);
        _delay_us(1);
		_data |= (((PINB >> SDA) & 0x01) << (7 - i) );
		I2CSW_LOW(SCL);
        _delay_us(1);                   //TODO: УБРАТЬ НАХУЙ!!!
	}
    return _data;
}

uint8_t i2csw_readACK(void) {
    uint8_t _data = i2csw_read();
    DDRB |= (1 << SDA);         //SDA снова на передачу
	I2CSW_LOW(SDA);          //тут вроде как надо передавать ACK
    I2CSW_HIGH(SCL);
    _delay_us(1);
    I2CSW_LOW(SCL);
	I2CSW_HIGH(SDA);
    DDRB &= ~(1 << SDA); 
    DDRB &= ~(1 << SCL);
    _delay_us(5);               //без этого не работает
    DDRB |= (1 << SCL);
    return _data;
}

uint8_t i2csw_readNAK(void) {
    uint8_t _data = i2csw_read();
    //DDRB |= (1 << SDA);         //SDA снова на передачу
	//I2CSW_HIGH(SDA);             //тут передавать ACK не надо т.к. это последний байт
    I2CSW_HIGH(SCL);
    _delay_us(1);
    I2CSW_LOW(SCL);
	//I2CSW_LOW(SDA);
    DDRB &= ~(1 << SDA); 
    DDRB &= ~(1 << SCL);
    _delay_us(5);               //без этого не работает
    DDRB |= (1 << SCL);
    return _data;
}
