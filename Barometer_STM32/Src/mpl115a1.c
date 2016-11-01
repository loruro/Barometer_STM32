/**
  ******************************************************************************
  * @file    mpl115a1.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    14-August-2016
  * @brief   MPL115A1 sensor driver.
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

#include "mpl115a1.h"

/**
  * @brief  Reads raw pressure and temperature data from MPL115A1 sensor.
  * @param  device Pointer to a mpl115a1Device structure that contains
  *         the configuration information for MPL115A1 sensor.
  * @param  pressureRaw Sampled raw pressure data.
  * @param  temperatureRaw Sampled raw temperature data.
  * @retval HAL status.
  */
HAL_StatusTypeDef mpl115a1ReadRawMeasurements(mpl115a1Device *device,
                                              uint16_t *pressureRaw,
                                              uint16_t *temperatureRaw);

/**
  * @brief  Calculates real pressure value using sampled raw pressure and temperature data.
  *         Calculations are based upon datasheet.
  * @param  device Pointer to a mpl115a1Device structure that contains
  *         the configuration information for MPL115A1 sensor.
  * @param  pressureRaw Sampled raw pressure data.
  * @param  temperatureRaw Sampled raw temperature data.
  * @retval HAL status.
  */
void mpl115a1calculatePressure(mpl115a1Device *device, uint16_t pressureRaw,
                               uint16_t temperatureRaw);

HAL_StatusTypeDef mpl115a1ReadRawMeasurements(mpl115a1Device *device,
                                              uint16_t *pressureRaw,
                                              uint16_t *temperatureRaw)
{
  HAL_StatusTypeDef status;
  uint8_t zero = 0;
  uint8_t start = MPL115A1_START_CONV;

  HAL_GPIO_WritePin(device->csPort, device->csPin, GPIO_PIN_RESET);
  status = HAL_SPI_Transmit(device->handle, &start, sizeof(start), 1000);
  if (status != HAL_OK)
    return status;

  status = HAL_SPI_Transmit(device->handle, &zero, sizeof(zero), 1000);
  if (status != HAL_OK)
    return status;
  HAL_GPIO_WritePin(device->csPort, device->csPin, GPIO_PIN_SET);
  HAL_Delay (3);

  uint8_t data[4];
  HAL_GPIO_WritePin(device->csPort, device->csPin, GPIO_PIN_RESET);
  for (int i = 0; i < 4; i++)
  {
    uint8_t address = (i + MPL115A1_MEASUR_START_ADDRES) << 1;
    address |= 0x80;
    status = HAL_SPI_Transmit(device->handle, &address, sizeof(address), 1000);
    if (status != HAL_OK)
      return status;

    status = HAL_SPI_TransmitReceive(device->handle, &zero, &data[i],
                                     sizeof(zero), 1000);
    if (status != HAL_OK)
      return status;
  }
  HAL_SPI_Transmit(device->handle, &zero, sizeof(zero), 1000);
  HAL_GPIO_WritePin(device->csPort, device->csPin, GPIO_PIN_SET);

  *pressureRaw = data[0] << 8 | data[1];
  *temperatureRaw = data[2] << 8 | data[3];

  return HAL_OK;
}

void mpl115a1calculatePressure(mpl115a1Device *device, uint16_t pressureRaw,
                               uint16_t temperatureRaw)
{
  int32_t c12x2, a1, a1x1, y1, a2x2, PComp;
  int16_t c12 = device->coefficients.c12;
  int16_t b1 = device->coefficients.b1;
  int16_t a0 = device->coefficients.a0;
  int16_t b2 = device->coefficients.b2;

  pressureRaw >>= 6;
  temperatureRaw >>= 6;

  c12x2 = (((int32_t)c12) * temperatureRaw) >> 11;
  a1 = (int32_t)b1 + c12x2;
  a1x1 = a1 * pressureRaw;
  y1 = (((int32_t)a0) << 10) + a1x1;
  a2x2 = (((int32_t)b2) * temperatureRaw) >> 1;
  PComp = (y1 + a2x2) >> 9;

  device->pressure = (PComp * 650 ) / (16368.0) + 500;
}

HAL_StatusTypeDef mpl115a1ReadMeasurements(mpl115a1Device *device)
{
  HAL_StatusTypeDef status;
  uint16_t pressureRaw, temperatureRaw;

  status = mpl115a1ReadRawMeasurements(device, &pressureRaw, &temperatureRaw);
  if (status != HAL_OK)
    return status;

  mpl115a1calculatePressure(device, pressureRaw, temperatureRaw);

  return HAL_OK;
}

HAL_StatusTypeDef mpl115a1ReadCoefficients(mpl115a1Device *device)
{
  HAL_StatusTypeDef status;
  uint8_t coefficientRegisters[MPL115A1_COEFF_NUMBER];

  HAL_GPIO_WritePin(device->csPort, device->csPin, GPIO_PIN_RESET);
  for (int i = 0; i < MPL115A1_COEFF_NUMBER; i++)
  {
    uint8_t zero = 0;
    uint8_t address = (i + MPL115A1_COEFF_START_ADDRESS) << 1;
    address |= 0x80;
    status = HAL_SPI_Transmit(device->handle, &address, sizeof(address), 1000);
    if (status != HAL_OK)
      return status;

    status = HAL_SPI_TransmitReceive(device->handle, &zero,
                                     &coefficientRegisters[i], sizeof(zero),
                                     1000);
    if (status != HAL_OK)
      return status;
  }
  HAL_GPIO_WritePin(device->csPort, device->csPin, GPIO_PIN_SET);

  device->coefficients.a0 = coefficientRegisters[0] << 8
      | coefficientRegisters[1];
  device->coefficients.b1 = coefficientRegisters[2] << 8
      | coefficientRegisters[3];
  device->coefficients.b2 = coefficientRegisters[4] << 8
      | coefficientRegisters[5];
  device->coefficients.c12 = coefficientRegisters[6] << 8
      | coefficientRegisters[7];

  return HAL_OK;
}
