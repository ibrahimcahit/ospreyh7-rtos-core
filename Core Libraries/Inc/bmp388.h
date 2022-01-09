/*
 * bmp388.h
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

typedef struct {

    float Pressure;
    float Temperature;
    float Altitude;

} BMP388_t;

BMP3_INTF_RET_TYPE bmp3_interface_init(struct bmp3_dev *bmp3, uint8_t intf);
void BMP388_init();
void BMP388_Read(BMP388_t *DataStruct);
