#ifndef USER_DEFINE_H__
#define USER_DEFINE_H__

#define DEBUG_EN

#define USE_BOSCH_SENSOR_API

#define USE_I2C_INTERFACE
//#define USE_SPI_INTERFACE

#define USE_BMP390


#if defined(USE_BMP390)
//#define SELF_TEST
#define READ_SENSOR_DATA


#endif

#endif
