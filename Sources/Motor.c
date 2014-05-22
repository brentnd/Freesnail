#include "Config.h"

/*
 * File: Motor.c
 * Freescale Cup 2014
 * Team: Freesnail
 * Author: Brent Dimmig <bnd8678@rit.edu>
 * 
 * Description: DC Motor setup, PWM clock setup, and motor functions.
 */

/*
 * Initialize the PWM clock and signal for the
 * DC motors. Sets up the signals and
 */
void InitMotor() {
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);

	// Enable TPM Clock
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;

	TPM0_SC = 0;
	TPM0_CONF = 0;

	TPM0_SC = TPM_SC_PS(MOTOR_CLK_PRESCALE);

	// Set output compare value
	TPM0_MOD = MOTOR_CLK / (1 << MOTOR_CLK_PRESCALE) / MOTOR_FREQUENCY;

	// Rising-edge, Left aligned, PWM, set on output match
	TPM0_C0SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	TPM0_C2SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;

	// Rising-edge, Left aligned, PWM, clear on output match
	TPM0_C1SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM0_C3SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

	SetMotors(0, 0);

	TPM0_SC |= TPM_SC_CMOD(1);

	// Connect output channels to PWM function
	PORTC_PCR1 = PORT_PCR_MUX(4);
	PORTC_PCR2 = PORT_PCR_MUX(4);
	PORTC_PCR3 = PORT_PCR_MUX(4);
	PORTC_PCR4 = PORT_PCR_MUX(4);
}

/* 
 * Set the DC motor speeds simultaneously.
 * @param1 MotorB (Left)  - unsigned int (-100 to 100) for motor B
 * @param2 MotorA (Right) - unsigned int (-100 to 100) for motor A
 */
void SetMotors(int MotorB, int MotorA) {
	// Update count compare to effectively change duty cycle
	TPM0_C2V = (uint16_t) ((TPM0_MOD * (MotorA+100)) / 200);
	TPM0_C3V = TPM0_C2V;
	TPM0_C0V = (uint16_t) ((TPM0_MOD * (MotorB+100)) / 200);
	TPM0_C1V = TPM0_C0V;
}
