#ifndef SERVO_H_
#define SERVO_H_

// Servo Clock
#define SERVO_CLK				(CORE_CLOCK)
#define SERVO_CLK_PRESCALE		6
#define SERVO_FREQ				50

// Servo Limits
//0.065
#define SERVO_DUTY_RIGHT		0.067
#define SERVO_DUTY_LEFT			0.029
#define SERVO_DUTY_RANGE		(SERVO_DUTY_RIGHT-SERVO_DUTY_LEFT)
#define SERVO_DUTY_CENTER		((SERVO_DUTY_RANGE)/2)+SERVO_DUTY_LEFT

void InitServo();
void SetServo(int Position);
void SetServoDutyCycle(float DutyCycle);

#endif /* SERVO_H_ */
