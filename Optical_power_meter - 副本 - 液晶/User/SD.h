#ifndef __SD_H
#define __SD_H

#include "stm32f10x.h"
#include "delay.h"		        // 延时函数头文件

#define TRUE  1
#define FALSE 0

#define SD_ST_GPIO_PORT    	GPIOC		              /* GPIO端口 */
#define SD_ST_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define SD_ST_GPIO_PIN			GPIO_Pin_8			        

#define SD_BUY_GPIO_PORT    	GPIOC			              /* GPIO端口 */
#define SD_BUY_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define SD_BUY_GPIO_PIN		    GPIO_Pin_9			        

#define SD_ST_HIGH		GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define SD_ST_LOW 		GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define SD_BUY_HIGH		GPIO_SetBits(GPIOC,GPIO_Pin_9)
#define SD_BUY_LOW 		GPIO_ResetBits(GPIOC,GPIO_Pin_9)
void SD_GPIO_Config(void);
void SD_Save(void);
#endif
