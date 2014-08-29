#include "Config.h"

int main(void) {
	int version = 0;
	int state = 0;

	// Initialize all components
	Initialize();

	// Start on version 1
	LED0_ON; LED1_OFF; LED2_OFF; LED3_OFF;

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

#ifdef MONITOR
  uart_putchar((char)(3));
  uart_putchar((char)(2));
  uart_putchar((char)(1));
  uart_putchar((char)(0));
#endif
  
	LED1_ON; LED2_ON; LED3_ON; LED0_OFF;
	// 3
	Delay_mS(800); LED1_OFF;
	// 2
	Delay_mS(800); LED2_OFF;
	// 1
	Delay_mS(800); LED3_OFF;

	// Run control loop with version selected
	Run(version);
	return 0;
}
