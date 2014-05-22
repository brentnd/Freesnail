#include "Config.h"

#define RUN

int main(void) {
#ifdef TESTING
	int i,line_pos;
	// Initialize all components
	Initialize();

	InitUARTs();
	// Present main menu

	for(;;)
	{
		while(!LineScanImageReady);

		// Get Camera Index (-100 to 100)
		uart_putchar((uint8_t)0);
		uart_putchar((uint8_t)0);
		uart_putchar((uint8_t)0);
		uart_putchar((uint8_t)0);

		// Copy from image to data for modification
		for(i=14; i<114; i++)
		LineScanData[i-14] += LineScanImage[i];

		// Median filter on raw data
		//medianFilter();

		//stretchFilter();

		for(i=0;i<99;i++)
		{
			uint8_t t = (uint8_t)((LineScanData[i])/17+2);
			uart_putchar(t);
		}

		line_pos = 100;		//GetLineIndexEdge();
		LineScanImageReady = FALSE;
		uart_putchar((uint8_t)(line_pos+100));

		uart_putchar((uint8_t)0);
		uart_putchar((uint8_t)0);
		uart_putchar((uint8_t)0);
		uart_putchar((uint8_t)0);
	}
#endif

#ifdef ACCEL
	// Initialize all components
	Initialize();
	InitUARTs();
	io_printf("Accelerometer data\n");
	while(1)
	{
		LED0_OFF; LED1_OFF; LED2_OFF; LED3_OFF;
		if(Zout < 3950)
		{
			if(Xout < -1000)
			LED3_ON;
			else if(Xout > 1000)
			LED0_ON;
		}
		else
		{
			LED1_ON; LED2_ON;
		}
		io_printf("X: %d Y: %d Z: %d\n",Xout,Yout,Zout);
	}
#endif

#ifdef SERVO
	Initialize();
	while(1)
	{
		SetServo(100);
		Delay_mS(1000);
		SetServo(0);
		Delay_mS(1000);
		SetServo(-100);
		Delay_mS(1000);
		SetServo(0);
		Delay_mS(1000);
	}

#endif

#ifdef RUN
	int version = 0;
	int state = 0;

	// Initialize all components
	Initialize();
#ifdef DEBUG
	InitUARTs();
	io_printf("Running data\n");
#endif

	// Start on version 1
	LED0_ON;
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;

	while (!BUTTON_A_PRESSED) {
		if (state & BUTTON_B_PRESSED) {
			//Old off
			GPIOB_PCOR |= ((uint32_t) (1 << (8 + version)));
			//Increment version (0 to 3)
			version = (version + 1) % 4;
			//New on
			GPIOB_PSOR |= ((uint32_t) (1 << (8 + version)));
			state = 0;
		}
		if (!BUTTON_B_PRESSED)
			state = 1;
	}
	// Count down to start
	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED0_OFF;
	// 3
	Delay_mS(240);
	SetServo(-50);
	Delay_mS(240);
	SetServo(50);
	Delay_mS(240);
	SetServo(-50);
	Delay_mS(240);
	SetServo(50);
	LED1_OFF;
	// 2
	Delay_mS(240);
	SetServo(-50);
	Delay_mS(240);
	SetServo(50);
	Delay_mS(240);
	SetServo(-50);
	Delay_mS(240);
	SetServo(50);
	LED2_OFF;
	// 1
	Delay_mS(240);
	SetServo(-50);
	Delay_mS(240);
	SetServo(50);
	Delay_mS(240);
	SetServo(-50);
	Delay_mS(240);
	SetServo(50);
	LED3_OFF;
	SetServo(0);
	Delay_mS(100);

	// Run control loop with version selected
	Run(version);
#endif
	return 0;
}
