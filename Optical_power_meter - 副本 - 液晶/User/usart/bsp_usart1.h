#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern uint8_t res;


void USART1_Config(void);
void NVIC_Configuration(void);
//int fputc(int ch, FILE *f);
//int fgetc(FILE *f);

#endif /* __USART1_H */
