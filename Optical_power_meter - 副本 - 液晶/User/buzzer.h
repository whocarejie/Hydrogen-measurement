#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"

#define BUZZER_OFF		GPIO_SetBits(GPIOG,GPIO_Pin_11)
#define BUZZER_ON 		GPIO_ResetBits(GPIOG,GPIO_Pin_11)

void BUZZER_GPIO_Config(void);

#endif
