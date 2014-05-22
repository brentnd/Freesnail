#ifndef MOTOR_H_
#define MOTOR_H_

#define MOTOR_CLK				(CORE_CLOCK/2)
#define MOTOR_CLK_PRESCALE		0
#define MOTOR_FREQUENCY			20000

void InitMotor();
void SetMotors(int MotorB, int MotorA);

#endif /* MOTOR_H_ */
