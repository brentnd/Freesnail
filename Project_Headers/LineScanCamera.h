#ifndef LINESCANCAMERA_H_
#define LINESCANCAMERA_H_

void InitLineScanCamera();
void LineScanImageCamera();
void SetLineScanExposureTime(uint32_t TimeIn_uS);
void medianFilter();
void stretchFilter();
int maxValue();
int minValue();
void filterData();
int isFinish();
int GetLineIndexCenter();
int GetLineIndexEdge();
int GetLineIndexRight();

// Return value for (L)ine (N)ot (F)ound
#define LNF			151
#define FINISH		152

#define TAOS_CLK_HIGH  GPIOE_PSOR = (1<<1)  
#define TAOS_CLK_LOW   GPIOE_PCOR = (1<<1)  
#define TAOS_SI_HIGH   GPIOD_PSOR = (1<<7)
#define TAOS_SI_LOW    GPIOD_PCOR =	(1<<7)

extern volatile uint16_t *LineScanImageWorkingBuffer;
extern volatile uint16_t *LineScanImage;
extern volatile uint16_t LineScanImageBuffer[2][128];
extern volatile uint8_t LineScanWorkingBuffer;
extern volatile uint8_t LineScanImageReady;
uint16_t LineScanData[100];
uint8_t histogram[4096];

#endif /* LINESCANCAMERA_H_ */
