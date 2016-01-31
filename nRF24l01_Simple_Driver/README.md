nRF24l01 Driver for STM32, Tested on STM32F103
------------------------------------------------
This driver uses CMSIS libraries
SPI2  is used to connect the Microcontroller to the Radio chip
Examples tested using ARM-GCC-complier using EmBlocks IDE (also CoIDE is tested)
all other compilers should work :) 


-------------------
Pin configurations
-------------------
STM32 -> nRF24
-------------------
PB12 -> CSN,

PB13 -> SCK,

PB14 -> MISO,

PB15 -> MOSI,

-------------------


Need to be added:

1- IRQ handler
2- enable the stand-by mode and "CE" pin
3- Enable Autoacknowledge & CRC
