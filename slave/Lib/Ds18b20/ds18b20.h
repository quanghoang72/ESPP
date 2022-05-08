#ifndef __DS18B20_H__
#define __DS18B20_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "main.h"


#define DS18B20_PORT GPIOA
#define DS18B20_PIN GPIO_PIN_1

uint8_t DS18B20_Start (void);
void DS18B20_Write (uint8_t data);
uint8_t DS18B20_Read (void);
uint16_t DS18B20_Operation (void);
void TIM6_delay (uint16_t time);

#ifdef __cplusplus
}
#endif
#endif