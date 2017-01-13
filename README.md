# Freescale Cup 2014
## Team Freesnail
* Winners of 2014 East Coast Freescale Cup Intelligent Car race
* Competitors in World-wide Freescale Cup final
* This is the code repository for the competition car.

## Videos
[![Demo Video](https://i.vimeocdn.com/video/471342576_350x250.jpg)](https://vimeo.com/91821297)
[![Freescale Cup Final Run](https://i.vimeocdn.com/video/472144993_350x250.jpg)](https://vimeo.com/92422930)

We built an autonomous line following car which placed 2nd Freescale's national competition, advancing us to the world wide competition. The Freedom KL25Z ARM Coretx-M0+ based microcontroller was used to control and interface with the various components. PWM signals controlled two independent DC motors and a hobby servo. The line was detected by using an interrupt based ADC to read linescan camera data. A custom PID control loop was developed and tuned to drive the car around the track.
