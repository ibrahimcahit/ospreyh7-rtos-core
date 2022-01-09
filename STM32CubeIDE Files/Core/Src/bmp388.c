/*
 * bmp388.c
 *
 *  Created on: Jan 09, 2022
 *      Author: Ibrahim Ozdemir
 *      GitHub: ibrahimcahit   
 */

#include "common_porting.h"
#include "stm32g4xx_hal.h"
#include "bmp3.h"
#include <stdint.h>
#include <stdio.h>
#include "user_define.h"
#include "bmp3_defs.h"

#include "bmp388.h"
#include "math.h"

#define SEALEVELPRESSURE_HPA (1013.25)

static uint8_t dev_addr = 0;

int8_t rslt = 0;
uint8_t settings_sel;

struct bmp3_dev dev;
struct bmp3_data data = { 0 };
struct bmp3_settings settings = { 0 };
struct bmp3_status status = { { 0 } };

BMP3_INTF_RET_TYPE bmp3_interface_init(struct bmp3_dev *bmp3, uint8_t intf){
    int8_t rslt = BMP3_OK;

    if(bmp3 != NULL){

        /* Bus configuration : I2C */
        if (intf == BMP3_I2C_INTF){

            dev_addr = BMP3_ADDR_I2C_SEC;
            bmp3->read = SensorAPI_I2Cx_Read;
            bmp3->write = SensorAPI_I2Cx_Write;
            bmp3->intf = BMP3_I2C_INTF;
        }

        bmp3->delay_us = bmp3_delay_us;
        bmp3->intf_ptr = &dev_addr;
    }

    else{rslt = BMP3_E_NULL_PTR;}

    return rslt;
}

void BMP388_init() {
    
    rslt = bmp3_interface_init(&dev, BMP3_I2C_INTF);
    rslt = bmp3_init(&dev);

    settings.int_settings.drdy_en = BMP3_ENABLE;
    settings.press_en = BMP3_ENABLE;
    settings.temp_en = BMP3_ENABLE;

    settings.odr_filter.press_os = BMP3_OVERSAMPLING_8X;
    settings.odr_filter.temp_os = BMP3_NO_OVERSAMPLING;
    settings.odr_filter.iir_filter = BMP3_IIR_FILTER_COEFF_3;
    settings.odr_filter.odr = BMP3_ODR_50_HZ;

    settings_sel = BMP3_SEL_PRESS_EN | BMP3_SEL_TEMP_EN | BMP3_SEL_PRESS_OS | BMP3_SEL_TEMP_OS | BMP3_SEL_ODR |
    BMP3_SEL_DRDY_EN;

    rslt = bmp3_set_sensor_settings(settings_sel, &settings, &dev);

    settings.op_mode = BMP3_MODE_NORMAL;

    rslt = bmp3_set_op_mode(&settings, &dev);
}

void BMP388_Read(BMP388_t *DataStruct){
    rslt = bmp3_get_status(&status, &dev);

    /* Read temperature and pressure data iteratively based on data ready interrupt */
    //printf("Check %d\r\n", rslt);
    
    if ((rslt == BMP3_OK) && (status.intr.drdy == BMP3_ENABLE)){
        /*
        * First parameter indicates the type of data to be read
        * BMP3_PRESS_TEMP : To read pressure and temperature data
        * BMP3_TEMP    : To read only temperature data
        * BMP3_PRESS       : To read only pressure data
        */
        rslt = bmp3_get_sensor_data(BMP3_PRESS_TEMP, &data, &dev);

        /* NOTE : Read status register again to clear data ready interrupt status */
        rslt = bmp3_get_status(&status, &dev);

        // Get pressure and temperature
        float pressure = data.pressure;
        float temperature = data.temperature;

        // Calculate altitude
        float atmospheric = pressure / 100.0f;
        float altitude = 44330.0 * (1.0 - pow(atmospheric / SEALEVELPRESSURE_HPA, 0.1903));

        DataStruct->Pressure = pressure;
        DataStruct->Temperature = temperature;
        DataStruct->Altitude = altitude;

    }
}



