#include "LPC8xx.h"
#include "type.h"
#include "lpc8xx_i2c.h"

volatile uint8_t I2CMasterTXBuffer[I2C_BUFSIZE];
volatile uint8_t I2CMasterRXBuffer[I2C_BUFSIZE];
volatile uint8_t I2CSlaveTXBuffer[I2C_BUFSIZE];
volatile uint8_t I2CSlaveRXBuffer[I2C_BUFSIZE];
volatile uint32_t I2CMonBuffer[I2C_MONBUFSIZE];
volatile uint32_t I2CReadLength, I2CWriteLength;

int main (void) 
{
	uint32_t i, regVal;
	unsigned char value, decimals;

	SystemCoreClockUpdate();

	/*connect the I2C SCL and SDA sigals to port pins(P0.0-P0.1)*/
	regVal = LPC_SWM->PINASSIGN7 & ~(0xFFUL<<24);
	LPC_SWM->PINASSIGN7 = regVal | (0 << 24);	/* P0.0 is I2C SDA, ASSIGN0(31:24) */
	regVal = LPC_SWM->PINASSIGN8 & ~(0xFF<<0);
	LPC_SWM->PINASSIGN8 = regVal | (1 << 0);	/* P0.1 is I2C SCL. ASSIGN0(7:0) */
	
	/* Enable I2C clock */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<5);
	/* Toggle peripheral reset control to I2C, a "1" bring it out of reset. */
	LPC_SYSCON->PRESETCTRL &= ~(0x1<<6);
	LPC_SYSCON->PRESETCTRL |= (0x1<<6);
  
  	// 通信スピード 100kHz
	// Pre-divider should be 36000000/(100000*4)-1 = 90-1 = 89
    I2C_MstInit(LPC_I2C, I2C_SMODE_PRE_DIV, CFG_MSTENA, 0x00);

	I2C_CheckIdle(LPC_I2C);

    I2CMasterTXBuffer[0] = 0xfe;
	I2CMasterRXBuffer[0] = 0;
    I2C_MstSendRcv(LPC_I2C, 0x72, (uint8_t *)I2CMasterTXBuffer, 1, (uint8_t *)I2CMasterRXBuffer, 1);

    // Configuration register
    I2CMasterTXBuffer[0] = 0x03;
    I2CMasterTXBuffer[1] = 0b10001100;
    I2C_MstSend(LPC_I2C, 0x72, (uint8_t *)I2CMasterTXBuffer, 2);

    // conversation rate
    I2CMasterTXBuffer[0] = 0x04;
    I2CMasterTXBuffer[1] = 5;
    I2C_MstSend(LPC_I2C, 0x72, (uint8_t *)I2CMasterTXBuffer, 2);

    // Temperature value high byte
    I2CMasterTXBuffer[0] = 0x00;
	I2CMasterRXBuffer[0] = 0;
    I2C_MstSendRcv(LPC_I2C, 0x72, (uint8_t *)I2CMasterTXBuffer, 1, (uint8_t *)I2CMasterRXBuffer, 1);
    value = I2CMasterRXBuffer[0];

    // Temperature value low byte
    I2CMasterTXBuffer[0] = 0x02;
	I2CMasterRXBuffer[0] = 0;
    I2C_MstSendRcv(LPC_I2C, 0x72, (uint8_t *)I2CMasterTXBuffer, 1, (uint8_t *)I2CMasterRXBuffer, 1);
    decimals = I2CMasterRXBuffer[0] >> 4;

    float temperature = (value * 10 + (decimals*10 + 8) / 16) / 10.0;

	while ( 1 );
}
