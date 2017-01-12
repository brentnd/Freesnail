#include "Config.h"

/* 
 * Give a line position input and return
 * the new servo value to set.
 * Assumes the time SAMPLE_MS has passed before calling
 */
int ComputeTurn(int input) {
	double error, output;
	// e(t)
	error = input;

	// Velocity algorithm for PID uses last computation for I and 3-point backwards difference for D
	// Calculates the change in position needed
	//u(t) =    u(t-1)  + Kp * (e(t)  - e(t-1)   ) + Ki * (e(t) + e(t-1))/2     + Kd * (e(t)  - 2e(t-1)       + e(t-2));
	output = lastOutput + kp * (error - lastError)
			+ ki * (error + lastError + llastError) / 3.0
			+ kd * (error - 2.0 * lastError + llastError);

	if (output > MAX)
		output = MAX;
	else if (output < -MAX)
		output = -MAX;

	//Save last values at t-1
	// e(t-2)  = e(t-1)
	llastError = lastError;
	// e(t-1)  = e(t)
	lastError = error;

	output = lastOutput * 0.3 + output * 0.7;

	// u(t-1)  = u(t)
	lastOutput = (int) output;

	// u(t)
	return output;
}

/*
 * Set values for PID coefficients
 */
void TunePID(double Kp, double Ki, double Kd) {
	// Max swing is 200
	kp = Kp / 2.0;
	// Maximum swing is 100
	ki = Ki / 20.0;
	// Kd maximum swing is 4*100
	kd = Kd / 4.0;

	// Initialise last values
	lastError = 0.0;
	llastError = 0.0;
	lastOutput = 0;
}
