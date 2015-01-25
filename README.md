# Freescale Cup 2014
## Team Freesnail
* Winners of 2014 East Coast Freescale Cup Intelligent Car race
* Competetors in World-wide Freescale Cup final
* This is the code repository for the competition car.

We built an autonomous line following car which placed 2nd Freescale's national competition, advancing us to the world wide competition. The Freedom KL25Z ARM Coretx-M0+ based microcontroller was used to control and interface with the various components. PWM signals controlled two independent DC motors and a hobby servo. The line was detected by using an interrupt based ADC to read linescan camera data. A custom PID control loop was developed and tuned to drive the car around the track.
