#include "Config.h"

//ADC globals
static uint8_t CurrentADC_State = ADC_STATE_INIT;
volatile uint8_t CurrentLineScanPixel = 0;
volatile uint8_t CurrentLineScanChannel = 0;
volatile uint16_t BatSenseADC_Value = 0; 

/******************************************************************************
 Function 1. Name	AUTO CAL ROUTINE   

 Parameters		ADC module pointer points to adc0 or adc1 register map 
 base address.
 Returns			Zero indicates success.
 Notes         		Calibrates the ADC16. Required to meet specifications 
 after reset and before a conversion is initiated.
 ******************************************************************************/
unsigned char ADC_Cal(ADC_MemMapPtr adcmap)
{

	unsigned short cal_var;

	ADC_SC2_REG(adcmap) &= ~ADC_SC2_ADTRG_MASK; // Enable Software Conversion Trigger for Calibration Process    - ADC0_SC2 = ADC0_SC2 | ADC_SC2_ADTRGW(0);   
	ADC_SC3_REG(adcmap) &= (~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK); // set single conversion, clear avgs bitfield for next writing
	ADC_SC3_REG(adcmap) |= (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(AVGS_32)); // Turn averaging ON and set at max value ( 32 )

	ADC_SC3_REG(adcmap) |= ADC_SC3_CAL_MASK;      // Start CAL
	while ((ADC_SC1_REG(adcmap,A)& ADC_SC1_COCO_MASK ) == COCO_NOT ); // Wait calibration end

	if ((ADC_SC3_REG(adcmap) & ADC_SC3_CALF_MASK) == CALF_FAIL)
	{
		return (1);    // Check for Calibration fail error and return 
	}
	// Calculate plus-side calibration
	cal_var = 0x00;

	cal_var = ADC_CLP0_REG(adcmap);
	cal_var += ADC_CLP1_REG(adcmap);
	cal_var += ADC_CLP2_REG(adcmap);
	cal_var += ADC_CLP3_REG(adcmap);
	cal_var += ADC_CLP4_REG(adcmap);
	cal_var += ADC_CLPS_REG(adcmap);

	cal_var = cal_var / 2;
	cal_var |= 0x8000; // Set MSB

	ADC_PG_REG(adcmap) = ADC_PG_PG(cal_var);

	// Calculate minus-side calibration
	cal_var = 0x00;

	cal_var = ADC_CLM0_REG(adcmap);
	cal_var += ADC_CLM1_REG(adcmap);
	cal_var += ADC_CLM2_REG(adcmap);
	cal_var += ADC_CLM3_REG(adcmap);
	cal_var += ADC_CLM4_REG(adcmap);
	cal_var += ADC_CLMS_REG(adcmap);

	cal_var = cal_var / 2;

	cal_var |= 0x8000; // Set MSB

	ADC_MG_REG(adcmap) = ADC_MG_MG(cal_var);

	ADC_SC3_REG(adcmap) &= ~ADC_SC3_CAL_MASK; /* Clear CAL bit */

	return (0);
}

/******************************************************************************
 Function 2 Name 	ADC_Config_Alt 
 Parameters		the register values to be set in the adc in a structure
 Returns			NONE
 Notes         		Configures ADC0 or ADC1 depending on adcmap
 Prior to calling this function populate the structure
 elements with the desired ADC configuration.
 ******************************************************************************/

void ADC_Config_Alt(ADC_MemMapPtr adcmap, tADC_ConfigPtr ADC_CfgPtr)
{
	ADC_CFG1_REG(adcmap) = ADC_CfgPtr->CONFIG1;
	ADC_CFG2_REG(adcmap) = ADC_CfgPtr->CONFIG2;
	ADC_CV1_REG(adcmap) = ADC_CfgPtr->COMPARE1;
	ADC_CV2_REG(adcmap) = ADC_CfgPtr->COMPARE2;
	ADC_SC2_REG(adcmap) = ADC_CfgPtr->STATUS2;
	ADC_SC3_REG(adcmap) = ADC_CfgPtr->STATUS3;
	//ADC_PGA_REG(adcmap)  = ADC_CfgPtr->PGA;
	ADC_SC1_REG(adcmap,A)= ADC_CfgPtr->STATUS1A;
	ADC_SC1_REG(adcmap,B)= ADC_CfgPtr->STATUS1B;
}

void ADC_Read_Cal(ADC_MemMapPtr adcmap, tADC_Cal_Blk *blk)
{
	blk->OFS = ADC_OFS_REG(adcmap);
	blk->PG = ADC_PG_REG(adcmap);
	blk->MG = ADC_MG_REG(adcmap);
	blk->CLPD = ADC_CLPD_REG(adcmap);
	blk->CLPS = ADC_CLPS_REG(adcmap);
	blk->CLP4 = ADC_CLP4_REG(adcmap);
	blk->CLP3 = ADC_CLP3_REG(adcmap);
	blk->CLP2 = ADC_CLP2_REG(adcmap);
	blk->CLP1 = ADC_CLP1_REG(adcmap);
	blk->CLP0 = ADC_CLP0_REG(adcmap);
	blk->CLMD = ADC_CLMD_REG(adcmap);
	blk->CLMS = ADC_CLMS_REG(adcmap);
	blk->CLM4 = ADC_CLM4_REG(adcmap);
	blk->CLM3 = ADC_CLM3_REG(adcmap);
	blk->CLM2 = ADC_CLM2_REG(adcmap);
	blk->CLM1 = ADC_CLM1_REG(adcmap);
	blk->CLM0 = ADC_CLM0_REG(adcmap);

}

void InitADC0()
{
	tADC_Config Master_Adc0_Config;

	SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK);

	//Lets calibrate the ADC. 1st setup how the channel will be used.

	disable_irq(INT_ADC0 - 16);

	Master_Adc0_Config.CONFIG1 = ADLPC_NORMAL 			//No low power mode
	| ADC_CFG1_ADIV(ADIV_4) //divide input by 4
			| ADLSMP_LONG 			//long sample time
			| ADC_CFG1_MODE(MODE_12) 			//single ended 8-bit conversion
			| ADC_CFG1_ADICLK(ADICLK_BUS);

	Master_Adc0_Config.CONFIG2 = MUXSEL_ADCA // select the A side of the ADC channel.
	| ADACKEN_DISABLED | ADHSC_HISPEED | ADC_CFG2_ADLSTS(ADLSTS_2); //Extra long sample Time (20 extra clocks)

	Master_Adc0_Config.COMPARE1 = 00000; // Comparators don't matter for calibration
	Master_Adc0_Config.COMPARE1 = 0xFFFF;

	Master_Adc0_Config.STATUS2 = ADTRG_HW //hardware triggers for calibration
	| ACFE_DISABLED //disable comparator
			| ACFGT_GREATER | ACREN_ENABLED | DMAEN_DISABLED //Disable DMA
			| ADC_SC2_REFSEL(REFSEL_EXT); //External Reference

	Master_Adc0_Config.STATUS3 = CAL_OFF | ADCO_SINGLE | AVGE_ENABLED
			| ADC_SC3_AVGS(AVGS_4);

	Master_Adc0_Config.PGA = 0; // Disable the PGA

	// Configure ADC as it will be used, but because ADC_SC1_ADCH is 31,
	// the ADC will be inactive.  Channel 31 is just disable function.
	// There really is no channel 31.

	Master_Adc0_Config.STATUS1A = AIEN_ON | DIFF_SINGLE | ADC_SC1_ADCH(31);

	ADC_Config_Alt(ADC0_BASE_PTR, &Master_Adc0_Config);  // config ADC

	// Calibrate the ADC in the configuration in which it will be used:
	ADC_Cal(ADC0_BASE_PTR );                    // do the calibration

	Master_Adc0_Config.STATUS2 = ACFE_DISABLED //disable comparator
	| ACFGT_GREATER | ACREN_ENABLED | DMAEN_DISABLED //Disable DMA
			| ADC_SC2_REFSEL(REFSEL_EXT); //External Reference

	Master_Adc0_Config.STATUS3 = CAL_OFF | ADCO_SINGLE;

	ADC_Config_Alt(ADC0_BASE_PTR, &Master_Adc0_Config);
}

void InitADCs()
{

	InitADC0();

	//All Adc processing of the linescan will be done in the ADC0 IRQ!
	//A state machine will scan through the channels. More can be added if needed.
	//This is done to automate the linescan capture on Channel 0 to ensure that timing is very even
	CurrentADC_State = ADC_STATE_INIT;

	//The pump will be primed with the PIT interrupt.  upon timeout/interrupt it will set the SI signal high
	//for the camera and then start reading.

	//Enable clock to the PIT
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;

	//We will use PIT0
	SetLineScanExposureTime(DEFAULT_LINESCAN_EXPOSURE_TIME_uS);
	//enable PIT0 and its interrupt
	PIT_TCTRL0 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;

	PIT_MCR |= PIT_MCR_FRZ_MASK; // stop the pit when in debug mode
	//Enable the PIT module
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;

	enable_irq(INT_PIT - 16);
	enable_irq(INT_ADC0 - 16);
}

void PIT_IRQHandler()
{
	PIT_TFLG0 = PIT_TFLG_TIF_MASK; //Turn off the Pit 0 Irq flag 
	CurrentADC_State = ADC_STATE_CAPTURE_BATTERY_LEVEL;
	ADC0_CFG2  |= ADC_CFG2_MUXSEL_MASK; //Select the B side of the mux
	ADC0_SC1A  =  BAT_SENSE_CHANNEL| ADC_SC1_AIEN_MASK;


}

void ADC0_IRQHandler()
{
	uint8_t Junk;
	switch (CurrentADC_State)
	//choose state for current ADC operation
	{
	default:
		Junk = ADC0_RA ;
		break;
		
		case ADC_STATE_CAPTURE_BATTERY_LEVEL:
			
			BatSenseADC_Value = ADC0_RA;
			
			TAOS_SI_HIGH; //set SI high
			CurrentADC_State = ADC_STATE_CAPTURE_LINE_SCAN; //line up state machine to linescan next
			CurrentLineScanPixel = 0; //start at the leftmost pixel
			TAOS_CLK_HIGH; //toggle the clock to read first val

			for (Junk = 0; Junk < 50; Junk++)
				;

			TAOS_SI_LOW;

			ADC0_CFG2 |= ADC_CFG2_MUXSEL_MASK; //Select the B side of the mux
			ADC0_SC1A  =  LINESCAN0_CHANNEL| ADC_SC1_AIEN_MASK;
			
		break;
			
			
		case ADC_STATE_CAPTURE_LINE_SCAN:

		if(CurrentLineScanPixel<128)
		{
			LineScanImageWorkingBuffer[CurrentLineScanPixel] = ADC0_RA; //get a pixel value
				ADC0_SC1A |= ADC_SC1_AIEN_MASK;//clear the interrupt flag
				CurrentLineScanPixel++;//move to next pixel

				TAOS_CLK_LOW;//cycle the clock to read out the next one
				for(Junk = 0;Junk<50;Junk++)
				{}
				TAOS_CLK_HIGH;

			}
			else
			{
				// done with the capture sequence.  we can wait for the PIT0 IRQ to restart

				TAOS_CLK_HIGH;//cycle the clock to end the sequence

				for(Junk = 0;Junk<50;Junk++)
				{
				}

				TAOS_CLK_LOW;
				CurrentADC_State = ADC_STATE_INIT; //reset the ADC state machine

				//swap the buffer

				if(LineScanWorkingBuffer == 0)//switch the working and reading buffers
				{
					LineScanWorkingBuffer = 1;
					LineScanImageWorkingBuffer = &LineScanImageBuffer[1][0];
					LineScanImage = &LineScanImageBuffer[0][0];
				}
				else
				{
					LineScanWorkingBuffer = 0;
					LineScanImageWorkingBuffer = &LineScanImageBuffer[0][0];
					LineScanImage = &LineScanImageBuffer[1][0];
				}

				LineScanImageReady = TRUE; //set flag indicating data is ready
			}

			break;

		}

	}

float ReadBatteryVoltage()
{
    return (((float)BatSenseADC_Value/(float)(ADC_MAX_CODE)) * 3.0);// * ((47000.0+10000.0)/10000.0);
}

