#ifndef __HMI_H
#define __HMI_H

#include "delay.h"
#include "sys.h"
#include "bsp_usart1.h"

void HMISends(char *buf1);
void HMISendb(u8 buf);
void HMISendstart(void);
void HMISendb(u8 k);

#endif 