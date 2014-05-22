#ifndef CONTROL_H_
#define CONTROL_H_


#define SCUT 25
#define MCUT 65

int straight_speed,medium_speed,turn_speed,brake_speed;

int speedC;

/* Turn the car using servo and differential */
void SetVelocity(int direction, int speed);

/* Set the speed of both motors */
void SetSpeed(int speed_left, int speed_right);

/* Run the Freescale Cup Track */
void Run(int version);

#endif /* CONTROL_H_ */
