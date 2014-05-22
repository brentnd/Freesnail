#ifndef PID_H_
#define PID_H_

#define	SETPOINT			0.0	// Line is in middle
#define MAX					100.0
#define	MIN					-100.0

//Direction consts
double kp, ki, kd;

//Direction buffer
double lastError, llastError;
int lastOutput;

/* 
 * Give a line position input and return
 * the new servo value to set.
 */
int ComputeTurn(int Input);

/*
 * Set values for PID coefficients
 */
void TunePID(double Kp, double Ki, double Kd);

#endif /* PID_H_ */
