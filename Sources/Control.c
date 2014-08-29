#include "Config.h"

/*
 * File: Control.c
 * Freescale Cup 2014
 * Team: Freesnail
 * Author: Brent Dimmig <bnd8678@rit.edu>
 * 		   Ben Kraines 	<bjk6868@rit.edu>
 * 
 * Description: Control methods for Freescale Cup
 */

/*
 * Turn the car using Servo and DC motor differential drive
 */
void SetVelocity(int direction, int speed) {
	int speed_left, speed_right;

	// Turn servo to direction
	SetServo(direction);

	// Left turn
	if (direction < -MCUT) {
		speed_left = speed - 2;
		speed_right = speed + 5;
	}
	// Right turn
	else if (direction > MCUT) {
		speed_left = speed + 5;
		speed_right = speed - 2;
	} else {
		speed_left = speed;
		speed_right = speed;
	}

	SetSpeed(speed_left, speed_right);
}

/*
 * Set the speed of both DC motors
 * Limited by MAX_SPEED
 * @param speed - integer (0 to 100)
 */
void SetSpeed(int speed_left, int speed_right) {
	SetMotors(speed_left, speed_right);
}

/*
 * Calculate the speed based on the current direction of the
 * servos. Currently uses tiered approach. Old was linear
 */
int ComputeSpeed(int direction) {
	int disp = 0;
	int new_speed = 0;

	// Speed proportional to turn range
	// disp = abs(direction);
	if (direction < 0)
		disp = -direction;
	else
		disp = direction;

	// Straight
	if (disp < SCUT) {
		//Increment speed
		speedC++;
		if (speedC > 42)
			speedC = 42;
		if (speedC > 15)
			new_speed = straight_speed - 6;
		else
			new_speed = straight_speed;
	} else if (speedC > 8) {
		if (disp < MCUT) {
			speedC--;
			new_speed = 0;
		} else {
			speedC -= 2;
			new_speed = brake_speed;
		}
	} else {
		speedC = 0;
		if (disp < MCUT)
			new_speed = medium_speed;
		else
			new_speed = turn_speed;
	}

  // Going down the ramp
  if (ramp == -1) {
    new_speed = brake_speed;
  }
  // Going up the ramp
  else if (ramp == 1) {
    new_speed = straight_speed;
  }
	
	return new_speed;

}

/* Stop the car at the finish */
void FinishLine() {

  char finish[4];
  finish[0] = 0;
  finish[1] = 0;
  finish[2] = 0;
  finish[3] = 0;
  uart_write(finish,4);
  
	Delay_mS(100);
	SetSpeed(-60, -60);
	Delay_mS(400);
	SetSpeed(-40, -40);
	Delay_mS(500);
	SetSpeed(-30, -30);
	Delay_mS(1000);
	SetSpeed(-20, -20);
	straight_speed = 0;
	medium_speed = 0;
	turn_speed = 0;
	brake_speed = 0;
	LED0_ON; LED1_OFF; LED2_ON; LED3_OFF;
	while (1) {
		LED0_TOG; LED1_TOG; LED2_TOG; LED3_TOG;
		Delay_mS(100);
	}
}

/* Set speed values per version */
void InitializeControls(int version) {

	// Reset speed counter
	speedC = 0;
	// Start different programs based on the on-board buttons
	switch (version) {
	case 0:
  default:
		brake_speed    = -45;
		turn_speed     = 51;
		medium_speed   = 55;
		straight_speed = 70;
		TunePID(2.2, 0.9, 7.6);
		break;
	}
}

/*
 * Main control loop to run the track for the Freescale Cup
 */
void Run(int version) {
  
  
	// Speed variables
	int speed = 0;
	int direction = 0;
	int i = 1;

	int line_pos = 0;
	int last_line_pos = 0;
	int lines = 0;
	int diff;
	int trans = 0;

	InitializeControls(version);

	// Enable motors after they're stopped
	SetMotors(0, 0);
	HBRIDGE_ENABLE;

	for (;;) {
		// Poll for new image (~10ms)
		while (!LineScanImageReady)
			;

		// Get Camera Index (-100 to 100)
		//line_pos = GetLineIndexCenter();
		line_pos = GetLineIndexEdge();

		// If camera can't see the line (Line not found)
		// continue on our path unless timed out
		if (line_pos == LNF)
			line_pos = last_line_pos;
		else if (line_pos == FINISH) {
			FinishLine();
			line_pos = 0;
		} else
			last_line_pos = line_pos;

		diff = last_line_pos - line_pos;
		if ((diff > 30) | (diff < -30))
			line_pos = last_line_pos;

		i = 1 - i;
		if (i == 0)
			continue;

		// Average both indices
		lines = lines * 0.2 + 0.4 * line_pos + 0.4 * last_line_pos;
		

		// Calculate direction based on line position
		direction = ComputeTurn(lines);

		// Calculate speed state based on direction
		speed = ComputeSpeed(direction);

		// Update servo and motors
		SetVelocity(direction, speed);

#ifdef MONITOR
		// line    turn    speed   accel
		// (0,200) (0,200) (0,100) 0,1,2
		
		if(trans++ == 10)
		{
		  char status[4];
		  status[0] = (char)(lines+100);
      status[1] = (char)(direction+100);
      status[2] = (char)(speed+100);
      status[3] = (char)(ramp+1);
      uart_write(status,4);
      trans = 0;
		}
		if(uart_rx(7)) {
		  char update[7];
		  uart_read(update,7);
	    char p = update[0];
	    char p2 = update[1];
	    char i = update[2];
	    char i2 = update[3];
	    char d = update[4];
	    char d2 = update[5];
      char stopgo = update[6];
      int stop = (int)stopgo;
      if(stop)
      {
        brake_speed =    0;
        turn_speed =     0;
        medium_speed =   0;
        straight_speed = 0;
      } else 
      {
        InitializeControls(0);
      }
	    double newp = (double)p + (double)(p2/10.0);
      double newi = (double)i + (double)(i2/10.0);
      double newd = (double)d + (double)(d2/10.0);
	    TunePID(newp, newi, newd);
		}
#endif
	}
}
