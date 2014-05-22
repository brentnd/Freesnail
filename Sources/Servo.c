#include "Config.h"

/*
 * File: Servo.c
 * Freescale Cup 2014
 * Team: Freesnail
 * Author: Brent Dimmig <bnd8678@rit.edu>
 * 
 * Description: Methods and macros for initializing and controlling the main turning servo.
 */

/*
 * Set the servo position
 * @param position - The position (-100 to 100) of the turning servo
 */
void SetServo(int position) {
	position -= 4;
	SetServoDutyCycle(
			((SERVO_DUTY_RANGE * (position + 100)) / 200) + SERVO_DUTY_LEFT);
}

/* 
 * Sets the duty cycle for the forward turning servo
 * Note: Duty cycle should not be outside of valid servo range
 * @param DutyCycle - Float (0.0 to 1.0) Duty Cycle to control servo
 */
void SetServoDutyCycle(float DutyCycle) {
	TPM1_C0V = TPM1_MOD * DutyCycle;
}

volatile unsigned char ServoTickVar;

/* Interrupt handler for the Servo */
void Servo_IRQHandler() {
	//Clear the overflow mask if set.
	if (TPM1_SC & TPM_SC_TOF_MASK)
		TPM1_SC |= TPM_SC_TOF_MASK;
	if (ServoTickVar < 0xff)
		ServoTickVar++;
}

/* Initialize the turning servo with the TPM module.
 * Sets up the timer clock and aligns servo to center
 */
void InitServo() {
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);

	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;

	TPM1_SC = 0;
	TPM1_CONF = 0;

	TPM1_SC = TPM_SC_PS(SERVO_CLK_PRESCALE);
	TPM1_SC |= TPM_SC_TOIE_MASK;

	TPM1_MOD = SERVO_CLK / (1 << (SERVO_CLK_PRESCALE + 1)) / SERVO_FREQ;

	TPM1_C0SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	TPM1_C1SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;

	SetServo(0.0);

	TPM1_SC |= TPM_SC_CMOD(1);

	enable_irq(INT_TPM1 - 16);

	PORTB_PCR0 = PORT_PCR_MUX(3);
}
