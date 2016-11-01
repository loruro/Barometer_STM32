/**
  ******************************************************************************
  * @file    mpl115a1.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    14-August-2016
  * @brief   Header file of MPL115A1 sensor driver.
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

#ifndef INC_MPL115A1_H_
#define INC_MPL115A1_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define MPL115A1_COEFF_START_ADDRESS 0x04
#define MPL115A1_COEFF_END_ADDRESS 0x0B
#define MPL115A1_COEFF_NUMBER \
  (MPL115A1_COEFF_END_ADDRESS - MPL115A1_COEFF_START_ADDRESS + 1)
#define MPL115A1_START_CONV 0x24
#define MPL115A1_MEASUR_START_ADDRES 0x00

/**
  * @brief  MPL115A1 coefficients struct definition.
  */
typedef struct mpl115a1Coefficients
{
  int16_t a0;
  int16_t b1;
  int16_t b2;
  int16_t c12;
} mpl115a1Coefficients;

/**
  * @brief  MPL115A1 config struct definition.
  */
typedef struct mpl115a1Device
{
  SPI_HandleTypeDef *handle;         /**< Pointer to a SPI_HandleTypeDef structure.*/
  mpl115a1Coefficients coefficients; /**< Structure containing MPL115A1 sensor coefficients.*/
  GPIO_TypeDef* csPort;              /**< Pointer to a GPIO_TypeDef structure of SPI CS pin.*/
  uint16_t csPin;                    /**< SPI CS pin.*/
  float pressure;                    /**< Measured pressure.*/
} mpl115a1Device;

/**
  * @brief  Reads coefficients from MPL115A1 sensor and stores them in device struct.
  * @param  device Pointer to a mpl115a1Device structure that contains
  *         the configuration information for MPL115A1 sensor.
  * @retval HAL status.
  */
HAL_StatusTypeDef mpl115a1ReadCoefficients(mpl115a1Device *device);

/**
  * @brief  Reads pressure from MPL115A1 sensor and stores it in device struct.
  * @param  device Pointer to a mpl115a1Device structure that contains
  *         the configuration information for MPL115A1 sensor.
  * @retval HAL status.
  */
HAL_StatusTypeDef mpl115a1ReadMeasurements(mpl115a1Device *device);

#endif /* INC_MPL115A1_H_ */
