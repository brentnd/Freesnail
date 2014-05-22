#ifndef ARM_SYSTICK_H_
#define ARM_SYSTICK_H_


void InitSysTick();
void Delay_mS(unsigned int TicksIn_mS);
void SysTickIrq();

#define SYSTICK_FREQUENCY 1000
#define NUM_TICKERS			4

extern volatile uint32_t Ticker[NUM_TICKERS];

#endif /* ARM_SYSTICK_H_ */
