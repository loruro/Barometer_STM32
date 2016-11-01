/**
  ******************************************************************************
  * @file    bmp180.c
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    14-August-2016
  * @brief   BMP180 sensor driver.
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

#include "bmp180.h"

/**
  * @brief  Reads raw temperature data from BMP180 sensor.
  * @param  device Pointer to a bmp180Device structure that contains
  *         the configuration information for BMP180 sensor.
  * @param  temperatureRaw Sampled raw temperature data.
  * @retval HAL status.
  */
HAL_StatusTypeDef bmp180ReadRawTemperature(bmp180Device *device,
                                           int32_t *temperatureRaw);

/**
  * @brief  Reads raw pressure data from BMP180 sensor.
  * @param  device Pointer to a bmp180Device structure that contains
  *         the configuration information for BMP180 sensor.
  * @param  pressureRaw Sampled raw pressure data.
  * @retval HAL status.
  */
HAL_StatusTypeDef bmp180ReadRawPressure(bmp180Device *device,
                                        int32_t *pressureRaw);

/**
  * @brief  Reads raw pressure and temperature data from BMP180 sensor.
  * @param  device Pointer to a bmp180Device structure that contains
  *         the configuration information for BMP180 sensor.
  * @param  pressureRaw Sampled raw pressure data.
  * @param  temperatureRaw Sampled raw temperature data.
  * @retval HAL status.
  */
HAL_StatusTypeDef bmp180ReadRawMeasurements(bmp180Device *device,
                                            int32_t *pressureRaw,
                                            int32_t *temperatureRaw);

/**
  * @brief  Calculates real temperature value using sampled raw temperature data.
  *         Calculations are based upon datasheet.
  * @param  device Pointer to a bmp180Device structure that contains
  *         the configuration information for BMP180 sensor.
  * @param  temperatureRaw Sampled raw temperature data.
  * @param  b5 Coefficient which is used for calculating real pressure value.
  * @retval HAL status.
  */
void bmp180calculateTemperature(bmp180Device *device, int32_t temperatureRaw,
                                int32_t *b5);

/**
  * @brief  Calculates real pressure value using sampled raw pressure data and
  *         coefficient calculated during real temperature value calculation.
  *         Calculations are based upon datasheet.
  * @param  device Pointer to a bmp180Device structure that contains
  *         the configuration information for BMP180 sensor.
  * @param  pressureRaw Sampled raw pressure data.
  * @param  b5 Coefficient which is used for calculating real pressure value.
  * @retval HAL status.
  */
void bmp180calculatePressure(bmp180Device *device, int32_t pressureRaw,
                             int32_t b5);

HAL_StatusTypeDef bmp180ReadRawTemperature(bmp180Device *device,
                                           int32_t *temperatureRaw)
{
  HAL_StatusTypeDef status;
  uint8_t convCmd = BMP180_CONTROL_TEMP_CONV;

  status = HAL_I2C_Mem_Write(device->handle, BMP180_I2C_ADDRESS,
                             BMP180_CONTROL_ADDRESS, 1, &convCmd,
                             sizeof(convCmd), 1000);
  if (status != HAL_OK)
    return status;
  // Waiting for sampling to finish.
  HAL_Delay (5);

  uint8_t data[2];
  status = HAL_I2C_Mem_Read(device->handle, BMP180_I2C_ADDRESS,
                            BMP180_MEASUR_START_ADDRES, 1, data, sizeof(data),
                            1000);
  if (status != HAL_OK)
    return status;
  *temperatureRaw = (uint32_t)data[0] << 8 | data[1];

  return HAL_OK;
}

HAL_StatusTypeDef bmp180ReadRawPressure(bmp180Device *device,
                                        int32_t *pressureRaw)
{
  HAL_StatusTypeDef status;
  uint8_t oss = device->oversampling;
  uint8_t convCmd = BMP180_CONTROL_PRESS_CONV + (oss << 6);

  status = HAL_I2C_Mem_Write(device->handle, BMP180_I2C_ADDRESS,
                             BMP180_CONTROL_ADDRESS, 1, &convCmd,
                             sizeof(convCmd), 1000);
  if (status != HAL_OK)
    return status;
  // Waiting for sampling to finish.
  switch(oss)
  {
    case 0:
      HAL_Delay (5);
      break;
    case 1:
      HAL_Delay (8);
      break;
    case 2:
      HAL_Delay (14);
      break;
    case 3:
      HAL_Delay (26);
      break;
  }

  uint8_t data[3];
  status = HAL_I2C_Mem_Read(device->handle, BMP180_I2C_ADDRESS,
                            BMP180_MEASUR_START_ADDRES, 1, data, sizeof(data),
                            1000);
  if (status != HAL_OK)
    return status;
  *pressureRaw = ((uint32_t)data[0] << 16 | data[1] << 8 | data[2]) >> (8 - oss);

  return HAL_OK;
}

HAL_StatusTypeDef bmp180ReadRawMeasurements(bmp180Device *device,
                                            int32_t *pressureRaw,
                                            int32_t *temperatureRaw)
{
  HAL_StatusTypeDef status;
  status = bmp180ReadRawTemperature(device, temperatureRaw);
  if (status != HAL_OK)
    return status;

  status = bmp180ReadRawPressure(device, pressureRaw);
  if (status != HAL_OK)
    return status;

  return HAL_OK;
}

void bmp180calculateTemperature(bmp180Device *device, int32_t temperatureRaw,
                                int32_t *b5)
{
  int32_t x1, x2;
  uint16_t ac6 = device->coefficients.ac6;
  uint16_t ac5 = device->coefficients.ac5;
  int16_t mc = device->coefficients.mc;
  int16_t md = device->coefficients.md;

  x1 = ((temperatureRaw - ac6) * ac5) >> 15;
  x2 = ((int32_t) mc << 11) / (x1 + md);
  *b5 = x1 + x2;
  device->temperature = (*b5 + 8) / 160.0;
}

void bmp180calculatePressure(bmp180Device *device, int32_t pressureRaw,
                             int32_t b5)
{
  int32_t x1, x2, x3, b6, b3, p;
  uint32_t b4, b7;
  int16_t b2 = device->coefficients.b2;
  int16_t ac2 = device->coefficients.ac2;
  int16_t ac1 = device->coefficients.ac1;
  int16_t ac3 = device->coefficients.ac3;
  int16_t b1 = device->coefficients.b1;
  uint16_t ac4 = device->coefficients.ac4;
  uint8_t oss = device->oversampling;

  b6 = b5 - 4000;
  x1 = (b2 * ((b6 * b6) >> 12)) >> 11;
  x2 = (ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((ac1 * 4 + x3) << device->oversampling) + 2) >> 2;
  x1 = (ac3 * b6) >> 13;
  x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = (x1 + x2 + 2) >> 2;
  b4 = ((uint32_t) ac4 * (x3 + 32768)) >> 15;
  b7 = ((uint32_t) pressureRaw - b3) * (50000 >> oss);
  if (b7 < 0x80000000)
    p = (b7 * 2) / b4;
  else
    p = (b7 / b4) * 2;
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  device->pressure = (p + ((x1 + x2 + 3791) / 16.0)) / 100.0;
}

HAL_StatusTypeDef bmp180ReadMeasurements(bmp180Device *device)
{
  HAL_StatusTypeDef status;
  int32_t pressureRaw, temperatureRaw, b5;

  status = bmp180ReadRawMeasurements(device, &pressureRaw, &temperatureRaw);
  if (status != HAL_OK)
    return status;

  bmp180calculateTemperature(device, temperatureRaw, &b5);
  bmp180calculatePressure(device, pressureRaw, b5);

  return HAL_OK;
}

HAL_StatusTypeDef bmp180ReadCoefficients(bmp180Device *device)
{
  HAL_StatusTypeDef status;
  uint8_t coefficientRegisters[BMP180_COEFF_NUMBER];

  status = HAL_I2C_Mem_Read(device->handle, BMP180_I2C_ADDRESS,
                            BMP180_COEFF_START_ADDRESS, 1,
                            coefficientRegisters,
                            sizeof(coefficientRegisters), 1000);
  if (status != HAL_OK)
    return status;

  // Checking if values of coefficients are correct.
  for(uint8_t i = 0; i < BMP180_COEFF_NUMBER / 2; i++)
  {
    uint16_t coefficient = coefficientRegisters[2 * i] << 8
        | coefficientRegisters[2 * i + 1];
    if (coefficient == 0 || coefficient == 0xFFFF)
      return HAL_ERROR;
  }

  device->coefficients.ac1 = coefficientRegisters[0] << 8
      | coefficientRegisters[1];
  device->coefficients.ac2 = coefficientRegisters[2] << 8
      | coefficientRegisters[3];
  device->coefficients.ac3 = coefficientRegisters[4] << 8
      | coefficientRegisters[5];
  device->coefficients.ac4 = coefficientRegisters[6] << 8
      | coefficientRegisters[7];
  device->coefficients.ac5 = coefficientRegisters[8] << 8
      | coefficientRegisters[9];
  device->coefficients.ac6 = coefficientRegisters[10] << 8
      | coefficientRegisters[11];
  device->coefficients.b1 = coefficientRegisters[12] << 8
      | coefficientRegisters[13];
  device->coefficients.b2 = coefficientRegisters[14] << 8
      | coefficientRegisters[15];
  device->coefficients.mb = coefficientRegisters[16] << 8
      | coefficientRegisters[17];
  device->coefficients.mc = coefficientRegisters[18] << 8
      | coefficientRegisters[19];
  device->coefficients.md = coefficientRegisters[20] << 8
      | coefficientRegisters[21];

  return HAL_OK;
}
