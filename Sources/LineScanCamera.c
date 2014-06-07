#include "Config.h"

volatile uint16_t *LineScanImageWorkingBuffer;
volatile uint16_t *LineScanImage;
volatile uint16_t LineScanImageBuffer[2][128];
volatile uint8_t LineScanWorkingBuffer;

volatile uint8_t LineScanImageReady = 0;

/*
 * File: LineScanCamera.c
 * Freescale Cup 2014
 * Team: Freesnail
 * Author: Ben Kraines 	<bjk6868@rit.edu>
 * 		   Brent Dimmig <bnd8678@rit.edu>
 * 
 * Description: Line scan camera interface and setup
 */

/*
 * Initialize the camera to perform line scan operations.
 */
void InitLineScanCamera() {
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTD_MASK; //Make sure the clock is enabled for PORTE;
	PORTE_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //Enable GPIO on on the pin for the CLOCK Signal
	PORTD_PCR7 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //Enable GPIO on on the pin for SI signal

	PORTD_PCR5 = PORT_PCR_MUX(0); //Make sure AO signal goes to an analog input
	PORTD_PCR6 = PORT_PCR_MUX(0); //Make sure AO signal goes to an analog input

	//Make sure the Clock and SI pins are outputs
	GPIOD_PDDR |= (1 << 7);
	GPIOE_PDDR |= (1 << 1);

	TAOS_CLK_LOW;
	TAOS_SI_LOW;

	LineScanWorkingBuffer = 0;

	LineScanImageWorkingBuffer = &LineScanImageBuffer[0][0];

	LineScanImage = &LineScanImageBuffer[1][0];
}

//change the frequency of data updates from the camera
void SetLineScanExposureTime(uint32_t TimeIn_uS) {
	float t;
	//Figure out how many Pit ticks we need for for the exposure time
	t = (TimeIn_uS / 1000000.0) * (float) (PERIPHERAL_BUS_CLOCK);
	PIT_LDVAL0 = (uint32_t)t;
}

/* 1D n=3 median filter on LineScanData */
void medianFilter() {
	int i;

	//Endpoints
	if (LineScanData[0] < LineScanData[1])
		LineScanData[0] = LineScanData[1];

	if (LineScanData[99] < LineScanData[98])
		LineScanData[99] = LineScanData[98];

	for (i = 1; i < 99; i++) {
		int a, b, c, med, min, max;
		a = LineScanData[i - 1];
		b = LineScanData[i];
		c = LineScanData[i + 1];

		if (a > b) {
			max = a;
			min = b;
		} else {
			max = b;
			min = a;
		}
		if (c > max)
			med = max;
		else if (c < min)
			med = min;
		else
			med = c;
		LineScanData[i] = med;
	}
}

/* Maximum value of data set */
int maxValue() {
	int i;
	int limit = 0;
	for (i = 1; i < 99; i++)
		if (LineScanData[i] > limit)
			limit = LineScanData[i];
	return limit;
}

/* Minimum value of data set */
int minValue() {
	int i;
	int limit = 4096;
	for (i = 1; i < 99; i++)
		if (LineScanData[i] < limit)
			limit = LineScanData[i];
	return limit;
}

/*
 * Applies a contrast stretching algorithm
 * by making a data histogram and stretching the remaining
 * 90% of the data accross the 0 to 4096 range.
 */
void stretchFilter() {
	int i;
	int sum = 0;
	int low_cut;
	int high_cut;
	double factor;
	int max, min;

	max = maxValue();
	min = minValue();
	factor = (double) max / (double) (max - min);

	// Check if the data is all similar, all white, all black
	if (factor > 2.5) {
		for (i = 0; i < 100; i++)
			LineScanData[i] = 1010;
		return;
	}

	// Make histogram
	for (i = 0; i < 4096; i++)
		histogram[i] = 0;
	for (i = 0; i < 100; i++)
		histogram[LineScanData[i]]++;
	
	// Find 5% cutoff values from histogram
	i = 0;
	// TODO: maybe decrease lower cutoff to 4 or 3%
	while (sum < 5)
		sum += histogram[i++];
	low_cut = i;
	sum = 0;
	i = 4095;
	// Find upper cutoff value
	// TODO: maybe increase upper cutoff to 10%?
	while (sum < 5)
		sum += histogram[i--];
	high_cut = i;

	// Precompute the factor used in stretching
	factor = (4096 / (high_cut - low_cut));

	// Stretch data accross range
	for (i = 0; i < 100; i++)
		if (LineScanData[i] <= low_cut)
			LineScanData[i] = 0;
		else if (LineScanData[i] >= high_cut)
			LineScanData[i] = 4096;
		else
			LineScanData[i] = (LineScanData[i] - low_cut) * factor;
}

/*
 * Uses median filter and a contract stretching algorithm
 * before thresholding the data.
 */
void filterData() {
	int i;

	int threshold = 0;

	// Copy from image to data for modification
	for (i = 14; i < 114; i++)
		LineScanData[i - 14] += LineScanImage[i];

	// Get rid of dark edges
	for (i = 0; i < 4; i++) {
		LineScanData[i] = LineScanData[6];
		LineScanData[(99 - i)] = LineScanData[(93)];
	}
  
	// Median filter on raw data
	medianFilter();
	
#ifdef MONITOR
  if(transmit == 2)
  {
    transmit = 0;
      for(i=0; i<100; i++)
        //uart_putchar((uint8)((LineScanData[i]/17)+1));
        printf("%03d,",(uint8)(LineScanData[i]/17));
    uart_putchar('$');
  }
#endif

	// Contrast stretching.
	stretchFilter();
	
	

	threshold = 1000;

	// 1 for black, 0 for white
	for (i = 0; i < 100; i++)
		LineScanData[i] = (LineScanData[i] >= threshold) ? 0 : 1;

	// Median filter on 1,0 data to filter outliers
	medianFilter();
}


/*
 * Finds finish by counting two small white bars
 */
int isFinish() {
	int width, i;
	int wcount = 0;
	width = -100;
	for (i = 1; i < 100; i++) {
		if (LineScanData[i] == 1) {
			// Start new black bar
			if ((LineScanData[i - 1] == 0))
				width = 1;
			else
				width++;
		} else {
			// TODO: specify min and max width of finish lines
			if ((width > 10) && (width < 12))
				wcount++;
			width = 0;
		}
	}
	return (wcount == 2);
}

/*
 * get the value in range (-100,100) for where in the spectrum the line center current lies
 * returns Line Not Found (LNF) value if the line is not currently readable
 * returns FINISH if finish line is passed
 */
int GetLineIndexCenter() {
	int position = 0;
	int i, left;
	int save_l = -1;
	int save_r = -1;
	int wcount = 0;
	int width;

	filterData();

	// Reset camera ready signal
	LineScanImageReady = FALSE;

	width = -100;
	// Find center of the widest black bar
	// Does not count darkness connected to edges
	for (i = 1; i < 100; i++) {
		if (LineScanData[i] == 1) {
			// Start new black bar
			if ((LineScanData[i - 1] == 0)) {
				left = i;
				width = 1;
			} else
				width++;
		} else {
			if ((width < 22) && (width > 4)) {
				save_l = left;
				save_r = i;
				wcount++;
			}
			width = 0;
		}
	}

	if (isFinish())
		position = FINISH;
	else if (wcount == 1)
		position = (save_r + save_l) - 100;
	else
		position = LNF;
	return position;
}

/*
 * LIGHT SURFACE
 * Find the position of the center of the track when
 * the track is white with dark edges and a light surface.
 */
int GetLineIndexEdgeLight() {
	int position = 0;
	int left_edge = 0, right_edge = 0;

	// TODO: Code from GetLineIndexCenter to find the line
	
	// Then code from GetLineIndexEdge to make line position into center position
	//Found finish
	if (isFinish())
		position = FINISH;
	else if ((left_edge > right_edge) & (left_edge < 50))
		position = left_edge * 2;
	else if ((right_edge > left_edge) & (right_edge < 50))
		position = -right_edge * 2;
	else
		position = LNF;
	return position;
}

/*
 * DARK SURFACE
 * Find the position of the center of the track when
 * the track is white with dark edges and dark surface.
 */
int GetLineIndexEdge() {
	int position = 0;
	int left_edge, right_edge;
	int i = 0;

	filterData();

	// Reset camera ready signal
	LineScanImageReady = FALSE;

	while (i < 50)
		if (LineScanData[i++] == 0)
			break;
	left_edge = i;
	i = 99;
	while (i > 50)
		if (LineScanData[i--] == 0)
			break;
	right_edge = (100 - i);

	//Found finish
	if (isFinish())
		position = FINISH;
	else if ((left_edge > right_edge) & (left_edge < 50))
		position = left_edge * 2;
	else if ((right_edge > left_edge) & (right_edge < 50))
		position = -right_edge * 2;
	else
		position = LNF;
	return position;
}
