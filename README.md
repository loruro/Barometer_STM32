# Barometer - STM32

Purpose of the project was to connect [STM32F4 Discovery](datasheets/STM32F4_Discovery_User_manual.pdf) board to two pressure sensors:
* [BMP180](datasheets/BMP180_datasheet.pdf) placed on [BOOSTXL-SENSHUB](datasheets/BOOSTXL-SENSHUB_User_manual.pdf) board
* [MPL115A1](datasheets/MPL115A1_datasheet.pdf) placed on [KAmodBAR-SPI](datasheets/KAMODBAR-SPI.pdf) board

perform some measurements and compare both sensor.

STM32F4 Discovery board has STM32F407VGT6 microcontroller with ARM Cortex-M4 core. The board also includes ST-LINK/V2 debugger and multiple sensors which are not used in this project.  
To communicate with PC I used UART interface. Between Discovery board and PC there was a simple [USB/UART converter](datasheets/USB-UART_converter.pdf). All output from board could be easily read in terminal application.
