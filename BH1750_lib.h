/*
 *маленький люксометр
 */



#define ADDR 0x23

void BH1750_initHigh(void);   //~1 lx
//void BH1750_initLow(void);    //~4 lx

uint16_t BH1750_getLux(void);   

