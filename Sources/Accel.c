#include "Config.h"

/*
 * Initialize the onboard accelerometer
 */
void InitAccel() {
	Accel_port_init();
	Accel_reg_init();
	Accel_calibrate();
}

/*
 * MCU initialization function
 */
void Accel_port_init(void) {
	//I2C0 module initialization
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;		// Turn on clock to I2C0 module 
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;		// Turn on clock to Port E module 
	PORTE_PCR24 = PORT_PCR_MUX(5);			// PTE24 pin is I2C0 SCL line 
	PORTE_PCR25 = PORT_PCR_MUX(5);			// PTE25 pin is I2C0 SDA line 
	I2C0_F = 0x14; // SDA hold time = 2.125us, SCL start hold time = 4.25us, SCL stop hold time = 5.125us *
	I2C0_C1 = I2C_C1_IICEN_MASK;    		// Enable I2C0 module 

	//Configure the PTA14 pin (connected to the INT1 of the MMA8451Q) for falling edge interrupts
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;		// Turn on clock to Port A module 
	PORTA_PCR14 |= (0|PORT_PCR_ISF_MASK|	// Clear the interrupt flag 
			PORT_PCR_MUX(0x1)|// PTA14 is configured as GPIO 
			PORT_PCR_IRQC(0xA));// PTA14 is configured for falling edge interrupts 

	//Enable PORTA interrupt on NVIC
	NVIC_ICPR |= 1 << ((INT_PORTA - 16) % 32);
	NVIC_ISER |= 1 << ((INT_PORTA - 16) % 32);
}

/*
 * Accelerometer initialization function
 */
void Accel_reg_init(void) {
	unsigned char reg_val = 0;

	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG2, 0x40);// Reset all registers to POR values

	do		// Wait for the RST bit to clear 
	{
		reg_val = I2C_ReadRegister(MMA845x_I2C_ADDRESS, CTRL_REG2) & 0x40;
	} while (reg_val);

	I2C_WriteRegister(MMA845x_I2C_ADDRESS, XYZ_DATA_CFG_REG, 0x00);	// +/-2g range -> 1g = 16384/4 = 4096 counts 
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG2, 0x02);// High Resolution mode
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG1, 0x3D);// ODR = 1.56Hz, Reduced noise, Active mode	
}

/*
 * Simple offset calibration
 */
void Accel_calibrate(void) {
	unsigned char reg_val = 0;

	while (!reg_val)		// Wait for a first set of data		 
	{
		reg_val = I2C_ReadRegister(MMA845x_I2C_ADDRESS, STATUS_REG) & 0x08;
	}

	I2C_ReadMultiRegisters(MMA845x_I2C_ADDRESS, OUT_X_MSB_REG, 6, AccData);	// Read data output registers 0x01-0x06  

	Xout = ((short) (AccData[0] << 8 | AccData[1])) >> 2;// Compute 14-bit X-axis output value
	Yout = ((short) (AccData[2] << 8 | AccData[3])) >> 2;// Compute 14-bit Y-axis output value
	Zout = ((short) (AccData[4] << 8 | AccData[5])) >> 2;// Compute 14-bit Z-axis output value

	Xoffset = Xout / 8 * (-1);		// Compute X-axis offset correction value
	Yoffset = Yout / 8 * (-1);		// Compute Y-axis offset correction value
	Zoffset = (Zout - SENSITIVITY_2G) / 8 * (-1);// Compute Z-axis offset correction value

	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG1, 0x00);// Standby mode to allow writing to the offset registers	
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, OFF_X_REG, Xoffset);
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, OFF_Y_REG, Yoffset);
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, OFF_Z_REG, Zoffset);
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG3, 0x00);// Push-pull, active low interrupt 
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG4, 0x01);// Enable DRDY interrupt 
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG5, 0x01);// DRDY interrupt routed to INT1 - PTA14 
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG1, 0x3D);// ODR = 1.56Hz, Reduced noise, Active mode	
}

/*
 * PORT A Interrupt handler
 */
void PORTA_IRQHandler() {
	// Clear the interrupt flag
	PORTA_PCR14 |= PORT_PCR_ISF_MASK;

	// Read data output registers 0x01-0x06
	I2C_ReadMultiRegisters(MMA845x_I2C_ADDRESS, OUT_X_MSB_REG, 6, AccData);

	// Compute 14-bit X, Y, and Z output values
	Xout = ((short) (AccData[0]<<8 | AccData[1])) >> 2;
	Yout = ((short) (AccData[2]<<8 | AccData[3])) >> 2;
	Zout = ((short) (AccData[4]<<8 | AccData[5])) >> 2;

	ramp = 0;
	// Set ramp variable for up or down based on accelerometer data
	if(Zout < 3950)
		if(Xout < -1000)
			ramp = -1;
		else if(Xout > 1000)
			ramp = 1;
}
