#ifndef F_CPU
#define F_CPU 9600000UL  
#endif

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#ifndef I2CLIB
    #define I2CLIB
    #include "I2C_lib.h"
#endif

#include "BH1750_lib.h"


#define LED PB4
#define SENS PB2

#define MIN_LUX 0x96    //150
#define MAX_LUX 0xC8    //200

uint16_t sens = 0;
uint8_t pwm = 0;
uint8_t mode = 0;
uint16_t lux = 0;
uint16_t sec = 0;
uint16_t msec = 0;


ISR(TIM0_COMPA_vect) {
  if (OCR0A < 250){
    PORTB |= (1 << LED);
  }
}

ISR(TIM0_COMPB_vect) {
  msec ++;    
}

ISR(TIM0_OVF_vect) {
  if (OCR0A > 0x0A) { 
    PORTB &= ~(1 << LED);
  } 

}

void main(void) {
  DDRB |= (1 << LED);          
    
      //настройка АЦП    
  ADMUX = (1 << ADLAR) | (0 << MUX1) | (1 << MUX0);
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (0 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
      //настройка таймера
  TCCR0A = 0x00;
  TCCR0B = (1 << CS01) | (0 << CS00);
  TIMSK0 = (1 << OCIE0B) | (1 << OCIE0A) | (1 << TOIE0);
  OCR0B = 0x05;

      //включить прерывания
  sei();
  _delay_ms(200);   //отдохнем 
    
  BH1750_initHigh();

        //главный цикл
  while (1) { 
    uint16_t dataLux  = BH1750_getLux(); 
    lux = ((lux*31 + dataLux) >> 5);      
    sens = ((sens*15 + (255 - ADCH)) >> 4);
          
      if (msec > 300) {
        msec = 0;
        sec++;
      }     
  
      if ((lux < MIN_LUX) && ((mode == 0x00) || (mode == 0x03))) {
        msec = 0;
        if (pwm < 5) {sec = 0;} else {sec = pwm;}
        mode = 0x01;
        TIMSK0 |= (1 << OCIE0A);
      } 
      if ((lux > MAX_LUX) && ((mode == 0x02) || (mode == 0x01))) {
        msec = 0;
        if ((sens - pwm) < 5) {sec = 0;} else {sec = sens - pwm;}
        mode = 0x03;
      }

      switch (mode) {
        case 0x00:            //--- выкл
          TIMSK0 &= ~(1 << OCIE0A);
          PORTB &= ~(1 << LED);
          pwm = 0;
          break;
        case 0x01:            //--- рассвет
          if (sec < sens) {pwm = sec;}
          else {mode = 0x02;}
          break;
        case 0x02:            //--- вкл
          pwm = sens;
          break;
        case 0x03:            //--- закат
          if (sec < sens) {pwm = sens - sec;}
          else {mode = 0x00;}
          break;
        }           
        
      OCR0A = (255 - pwm);  
    }
}
