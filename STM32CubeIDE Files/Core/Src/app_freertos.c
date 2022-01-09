/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "GNSS.h"
#include "bmp388.h"
#include "mpu9255.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

// create BMP388 instance
BMP388_t BMP388;

// create MPU9255 instance
MPU9255_t MPU9255;

// Variables to store BMP388 Data
float temperature, pressure, baroAltitude;

// Variables to store MPU925 Data
float pitch, yaw, roll;

// Variables to store GPS Data
float latitude, longitude, gpsAltitude, gpsAltitudeMSL, speed, fix, satellites, day, month, year;
int fixPrint;

// Timer for printer
int printTimer = 0;

// Time holder for GPS
int gpsTimer = 0;

/* USER CODE END Variables */
/* Definitions for AHRS_Update */
osThreadId_t AHRS_UpdateHandle;
const osThreadAttr_t AHRS_Update_attributes = {
  .name = "AHRS_Update",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for BMP388 */
osThreadId_t BMP388Handle;
const osThreadAttr_t BMP388_attributes = {
  .name = "BMP388",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 128 * 4
};
/* Definitions for GNSS */
osThreadId_t GNSSHandle;
const osThreadAttr_t GNSS_attributes = {
  .name = "GNSS",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 128 * 4
};
/* Definitions for MPU9250 */
osThreadId_t MPU9250Handle;
const osThreadAttr_t MPU9250_attributes = {
  .name = "MPU9250",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartAHRS_UpdateTask(void *argument);
void StartBMP388Task(void *argument);
void StartGNSSTask(void *argument);
void StartMPU9250Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of AHRS_Update */
  AHRS_UpdateHandle = osThreadNew(StartAHRS_UpdateTask, NULL, &AHRS_Update_attributes);

  /* creation of BMP388 */
  BMP388Handle = osThreadNew(StartBMP388Task, NULL, &BMP388_attributes);

  /* creation of GNSS */
  GNSSHandle = osThreadNew(StartGNSSTask, NULL, &GNSS_attributes);

  /* creation of MPU9250 */
  MPU9250Handle = osThreadNew(StartMPU9250Task, NULL, &MPU9250_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartAHRS_UpdateTask */
/**
  * @brief  Function implementing the AHRS_Update thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartAHRS_UpdateTask */
void StartAHRS_UpdateTask(void *argument)
{
  /* USER CODE BEGIN StartAHRS_UpdateTask */
  /* Infinite loop */
  for(;;)
  {
	  //Update variables from BMP388
	  pressure = BMP388.Pressure;
	  temperature = BMP388.Temperature;
	  baroAltitude = BMP388.Altitude;

	  //Update variables from MPU9255
	  pitch = MPU9255.pitch;
	  yaw = MPU9255.yaw;
	  roll= MPU9255.roll;

	  //Update variables from GPS
	  latitude = GNSS_Handle.fLat;
	  longitude = (GNSS_Handle.lon / 10000000.0);
	  gpsAltitude = (GNSS_Handle.height / 1000.0);
	  gpsAltitudeMSL = (GNSS_Handle.hMSL / 1000.0);
	  speed = GNSS_Handle.gSpeed;
	  fix = GNSS_Handle.fixType;
	  fixPrint = fix;
	  satellites = GNSS_Handle.numSV;
	  day = GNSS_Handle.day;
	  month = GNSS_Handle.month;
	  year = GNSS_Handle.year;

	  /*
	  if ((HAL_GetTick() - printTimer) > 1001) {

		  printf("Pressure: %f\r\n", pressure);
		  printf("Temperature: %f\r\n", temperature);
		  printf("Barometer Altitude: %f\r\n", baroAltitude);
		  printf("- - - - -\r\n");

		  printf("Pitch: %f\r\n", pitch);
		  printf("Yaw: %f\r\n", yaw);
		  printf("Roll: %f\r\n", roll);
		  printf("- - - - -\r\n");

		  printf("Latitude: %f\r\n", latitude);
		  printf("Longitude: %f\r\n", longitude);
		  printf("GPS Altitude: %f\r\n", gpsAltitude);
		  printf("GPS Altitude MSL: %f\r\n", gpsAltitudeMSL);
		  printf("Velocity: %f\r\n", speed);
		  printf("GPS Fix Type: %dD Fix \r\n", fixPrint);
		  printf("Num. of Satellites: %f\r\n", satellites);
		  printf("Day: %f\r\n", day);
		  printf("Month: %f\r\n", month);
		  printf("Year: %f\r\n", year);

		  printf("********************************\r\n");
		  printTimer = HAL_GetTick();
	  }
	*/

	  printTimer++;

    osDelay(1);
  }
  /* USER CODE END StartAHRS_UpdateTask */
}

/* USER CODE BEGIN Header_StartBMP388Task */
/**
* @brief Function implementing the BMP388 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBMP388Task */
void StartBMP388Task(void *argument)
{
  /* USER CODE BEGIN StartBMP388Task */
  /* Infinite loop */
  for(;;)
  {
	  BMP388_Read(&BMP388);

    osDelay(1);
  }
  /* USER CODE END StartBMP388Task */
}

/* USER CODE BEGIN Header_StartGNSSTask */
/**
* @brief Function implementing the GNSS thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGNSSTask */
void StartGNSSTask(void *argument)
{
  /* USER CODE BEGIN StartGNSSTask */
  /* Infinite loop */
  for(;;)
  {
	if ((HAL_GetTick() - gpsTimer) > 300) {

		GNSS_GetUniqID(&GNSS_Handle);
		GNSS_ParseBuffer(&GNSS_Handle);
		HAL_Delay(250);

		GNSS_GetPVTData(&GNSS_Handle);
		GNSS_ParseBuffer(&GNSS_Handle);

		gpsTimer = HAL_GetTick();
	}
	gpsTimer++;
    osDelay(1);
  }
  /* USER CODE END StartGNSSTask */
}

/* USER CODE BEGIN Header_StartMPU9250Task */
/**
* @brief Function implementing the MPU9250 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMPU9250Task */
void StartMPU9250Task(void *argument)
{
  /* USER CODE BEGIN StartMPU9250Task */
  /* Infinite loop */
  for(;;)
  {
	  readAll(&hi2c2, &MPU9255);

    osDelay(1);
  }
  /* USER CODE END StartMPU9250Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

