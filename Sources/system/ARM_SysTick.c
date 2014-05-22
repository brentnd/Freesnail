#include "Config.h"


static volatile unsigned int DelayTimerTick = 0;
volatile uint32_t Ticker[NUM_TICKERS];

void InitSysTick()
{
	uint8_t i;
	
	for(i=0;i<NUM_TICKERS;i++)
		Ticker[i] = 0;
	
	SYST_RVR = CORE_CLOCK/SYSTICK_FREQUENCY;
	SYST_CSR = SysTick_CSR_ENABLE_MASK | SysTick_CSR_TICKINT_MASK | SysTick_CSR_CLKSOURCE_MASK;
}

void SysTick_Handler()
{
	uint8_t i;

	if(DelayTimerTick<0xFFFFFFFF)
		DelayTimerTick++;
	
	for(i=0;i<NUM_TICKERS;i++)
		if(Ticker[i]<0xFFFFFFFF)
			Ticker[i]++;
}

void Delay_mS(unsigned int TicksIn_mS)
{
	DelayTimerTick = 0;

	while(DelayTimerTick<TicksIn_mS)
	{
	}
}

