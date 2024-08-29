#ifndef __BSP_BUZZER_H_
#define __BSP_BUZZER_H_
#include "main.h"

//#define BUZZER_PIN      GPIO_PIN_1
//#define BUZZER_GPIO     GPIOB

extern void(*buzzer_sound)(void);

void buzzer_init(void);

void Buzzer_KeySound(void);


void BuzzerSound_Handler(void(*buzzer_sound_handler)(void));



#endif 
