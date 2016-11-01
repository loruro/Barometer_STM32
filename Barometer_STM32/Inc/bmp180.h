/**
  ******************************************************************************
  * @file    bmp180.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    14-August-2016
  * @brief   Header file of BMP180 sensor driver.
  ******************************************************************************
  * @attention
  *
  * This file is part of Barometer_STM32.
  *
  * Barometer_STM32 is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * Barometer_STM32 is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with Barometer_STM32.  If not, see <http://www.gnu.org/licenses/>.
  *
  ******************************************************************************
  */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define BMP180_I2C_ADDRESS 0xEE
#define BMP180_COEFF_START_ADDRESS 0xAA
#define BMP180_COEFF_END_ADDRESS 0xBF
#define BMP180_COEFF_NUMBER \
  (BMP180_COEFF_END_ADDRESS - BMP180_COEFF_START_ADDRESS + 1)
#define BMP180_CONTROL_ADDRESS 0xF4
#define BMP180_CONTROL_TEMP_CONV 0x2E
#define BMP180_CONTROL_PRESS_CONV 0x34
#define BMP180_MEASUR_START_ADDRES 0xF6

/**
  * @brief  BMP180 coefficients struct definition.
  */
typedef struct bmp180Coefficients
{
  int16_t ac1;
  int16_t ac2;
  int16_t ac3;
  uint16_t ac4;
  uint16_t ac5;
  uint16_t ac6;
  int16_t b1;
  int16_t b2;
  int16_t mb;
  int16_t mc;
  int16_t md;
} bmp180Coefficients;

/**
  * @brief  BMP180 config struct definition.
  */
typedef struct bmp180Device
{
  I2C_HandleTypeDef *handle;       /**< Pointer to a I2C_HandleTypeDef structure. */
  bmp180Coefficients coefficients; /**< Structure containing BMP180 sensor coefficients.*/
  uint8_t oversampling;            /**< Parameter for sampling accuracy. */
  float pressure;                  /**< Measured pressure. */
  float temperature;               /**< Measured temperature. */
} bmp180Device;

/**
  * @brief  Reads coefficients from BMP180 sensor and stores them in device struct.
  * @param  device Pointer to a bmp180Device structure that contains
  *         the configuration information for BMP180 sensor.
  * @retval HAL status.
  */
HAL_StatusTypeDef bmp180ReadCoefficients(bmp180Device *device);

/**
  * @brief  Reads pressure and temperature from BMP180 sensor and stores them
  *         in device struct.
  * @param  device Pointer to a bmp180Device structure that contains
  *         the configuration information for BMP180 sensor.
  * @retval HAL status.
  */
HAL_StatusTypeDef bmp180ReadMeasurements(bmp180Device *device);

#endif /* INC_BMP180_H_ */
