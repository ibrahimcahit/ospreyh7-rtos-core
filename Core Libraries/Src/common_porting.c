#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "stm32g4xx_hal.h"
#include "common_porting.h"
#include "main.h"

#define ADDR 0x76<<1

extern I2C_HandleTypeDef hi2c2;

uint8_t GTXBuffer[512], GRXBuffer[2048];


void DelayUs(uint32_t Delay)
{
	uint32_t i;

	while(Delay--)
	{
		for(i = 0; i < 84; i++)
		{
			;
		}
	}
}

void bmp3_delay_us(uint32_t period, void *intf_ptr)
{
	uint32_t i;

	while(period--)
	{
		for(i = 0; i < 84; i++)
		{
			;
		}
	}
}



#if defined(USE_BOSCH_SENSOR_API)
/*******************************************************************************
* Function Name  : I2C_Read
* Description    : Write data into slave device.
* Input          : I2C1 or I2C2, slave_address7, subaddress, Write Number
* Output         : None
* Return         : number of bytes transmitted
*******************************************************************************/
int8_t SensorAPI_I2Cx_Read(uint8_t subaddress, uint8_t *pBuffer, uint16_t ReadNumbr, void *intf_ptr)
{
	// send register address
	HAL_I2C_Master_Transmit(&I2C_HANDLE, ADDR, &subaddress, 1, BUS_TIMEOUT);
	HAL_I2C_Master_Receive(&I2C_HANDLE, ADDR, pBuffer, ReadNumbr, BUS_TIMEOUT);
	return 0;
}

int8_t SensorAPI_I2Cx_Write(uint8_t subaddress, uint8_t *pBuffer, uint16_t WriteNumbr, void *intf_ptr)
{
	GTXBuffer[0] = subaddress;
	memcpy(&GTXBuffer[1], pBuffer, WriteNumbr);

	// send register address
	HAL_I2C_Master_Transmit(&I2C_HANDLE, ADDR, GTXBuffer, WriteNumbr+1, BUS_TIMEOUT);
	return 0;
}

#endif


