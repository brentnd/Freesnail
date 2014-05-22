#ifndef CONFIG_H_
#define CONFIG_H_

#include "Derivative.h"
#include "System.h"
#include "Servo.h"
#include "Motor.h"
#include "Control.h"
#include "LineScanCamera.h"
#include "ADC.h"
#include "PID.h"
#include "Accel.h"

#define DEUG

#ifdef DEBUG
#include "UART.h"
#endif

#define HBRIDGE_EN_LOC			(1<<21)
#define HBRIDGE_FAULT_LOC     	(1<<20)

#define HBRIDGE_ENABLE			GPIOE_PSOR = HBRIDGE_EN_LOC
#define HBRIDGE_DISABLE			GPIOE_PCOR = HBRIDGE_EN_LOC

#define BUTTON_A					((uint32_t)(1<<13))
#define BUTTON_B					((uint32_t)(1<<17))
#define REED_SWITCH					((uint32_t)(1<<1))

#define LED0						((uint32_t)(1<<8))
#define LED1						((uint32_t)(1<<9))
#define LED2						((uint32_t)(1<<10))
#define LED3						((uint32_t)(1<<11))

#define LEDB						((uint32_t)(1<<1))

#define LEDB_ON				GPIOD_PSOR |= LEDB
#define LED0_ON				GPIOB_PSOR |= LED0
#define LED1_ON				GPIOB_PSOR |= LED1
#define LED2_ON				GPIOB_PSOR |= LED2
#define LED3_ON				GPIOB_PSOR |= LED3

#define LEDB_OFF			GPIOD_PCOR |= LEDB
#define LED0_OFF			GPIOB_PCOR |= LED0
#define LED1_OFF			GPIOB_PCOR |= LED1
#define LED2_OFF			GPIOB_PCOR |= LED2
#define LED3_OFF			GPIOB_PCOR |= LED3

#define LED0_TOG			GPIOB_PTOR |= LED0
#define LED1_TOG			GPIOB_PTOR |= LED1
#define LED2_TOG			GPIOB_PTOR |= LED2
#define LED3_TOG			GPIOB_PTOR |= LED3

#define BUTTON_A_PRESSED	((GPIOC_PDIR&BUTTON_A)>0)
#define BUTTON_B_PRESSED	((GPIOC_PDIR&BUTTON_B)>0)

// Initialize the car
void Initialize();

// Initialize the GPIO pins
void InitGPIO();

#endif /* CONFIG_H_ */
