#ifndef COMMON_PORTING_H__
#define COMMON_PORTING_H__

#include <stdint.h>
#include "user_define.h"


#define I2C_HANDLE	(hi2c2)

#define BUS_TIMEOUT 1000


void DelayUs(uint32_t Delay);
void bmp3_delay_us(uint32_t period, void *intf_ptr);


#if defined(USE_BOSCH_SENSOR_API)
int8_t SensorAPI_I2Cx_Read(uint8_t subaddress, uint8_t *pBuffer, uint16_t ReadNumbr, void *intf_ptr);
int8_t SensorAPI_I2Cx_Write(uint8_t subaddress, uint8_t *pBuffer, uint16_t WriteNumbr, void *intf_ptr);

#endif

#endif
