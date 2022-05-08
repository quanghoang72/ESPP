
#ifndef __HCSR04_H
#define __HCSR04_H

#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

#define TRIG_PIN GPIO_PIN_7
#define TRIG_PORT GPIOA


void TIM4_delay (uint16_t time);
void HCSR04_Read (void);
\

 #ifdef __cplusplus
}
#endif

#endif /* __HCSR04_H */