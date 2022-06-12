# Time-Of-Flight project
This project is inspired base on Time-of-Flight (ToF) sensors and during Embedded System Programming Project course. It counts the number of people who enters or exits a "room" with slave node. A 16x2 LCD displays the received information on master node. Moreover, the light will automatically be turned on or off depends on the number inside the room.  
This project aims to learn applying RTOS (uC-III) on MCU and Scrum framework.  
## Table of Contents
<!-- TOC -->
- [Hardware](#hardware)
- [Toolchain](#toolchain) 
- [Slave node](#slave-node)    
- [Master node](#master-node)  
- [Installation Instruction](#installation-instruction)  
<!-- TOC -->

## Hardware
- [STM32 Nucleo-L152RE](https://www.st.com/en/evaluation-tools/nucleo-l152re.html)  
- [HC-SR04 Distance Sensor](https://datasheetspdf.com/pdf/1380136/ETC/HC-SR04/1)  
- [DS18B20 Water-proof Digital Temperature sensor](https://datasheetspdf.com/pdf/1447575/MaximIntegrated/DS18B20/1)  
- [LCD 16x2](https://www.dfrobot.com/product-135.html)  
- [MAX3485 Transceiver](https://pdf1.alldatasheet.com/datasheet-pdf/view/73221/MAXIM/MAX3485.html)  
## Toolchain
- [STM32CubeMX](https://aur.archlinux.org/packages/stm32cubemx/)  
- [Platformio](https://docs.platformio.org/en/latest/core/installation.html) 
## Slave node
The Slave node presents as an STM32 Nucleo comprising distance sensor and temperature sensor. Slave is used to fetch data and transmit it to Master through MAX3485.  

---
>Note: 
To count human comming in the room, the ultrasonic sensor must be triggered prior to DS18B20 sensor and in reverse order to subtract people inside. 
---  
## Master node
The Master first receives the data which is transfered by slave and displays the nubmer of people in the room and inform the LED on or off.  
## Installation Instruction
To replicate this project, all the information is found in `ToF_USER_INSTALL_DOC.pdf`  
