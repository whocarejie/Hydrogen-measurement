#ifndef __APP_H
#define __APP_H

#include "stm32f10x.h"
#include "bsp_usart1.h"

#define K 			9872.5
#define B 			0.5
#define Thre		0.0004
//#define Probe1 	0.7
#define H2_thre	600

//SD���洢��ʼ��
#define TRUE  1
#define FALSE 0

#define SD_ST_GPIO_PORT    	GPIOC		              /* GPIO�˿� */
#define SD_ST_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define SD_ST_GPIO_PIN			GPIO_Pin_8			        

#define SD_BUY_GPIO_PORT    	GPIOC			              /* GPIO�˿� */
#define SD_BUY_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define SD_BUY_GPIO_PIN		    GPIO_Pin_9			        

#define SD_ST_HIGH		GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define SD_ST_LOW 		GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define SD_BUY_HIGH		GPIO_SetBits(GPIOC,GPIO_Pin_9)
#define SD_BUY_LOW 		GPIO_ResetBits(GPIOC,GPIO_Pin_9)

typedef struct{
	uint16_t rxData1[3],rxData2[3];								//�������ֵ
	float value1,value2;													//����ֵ
	bool uint[2];																	//��λ
	uint8_t valueStr1[10],valueStr2[10];					//����ֵ��ʾ
	float ratio;																	//���ʱ�ֵ
	uint8_t ratioStr[10];													//��ֵ��ʾ
}Power;

typedef struct{
	float density;											//����Ũ��	
	uint8_t densityStr[8];
	float a,b,c;												//�궨����ֵ
	uint8_t aStr[10],bStr[10],cStr[10];	//����������ʾ
}Hydrogen;

typedef enum{
	page1 = 0,
	page2,
	page3
}Page;

typedef enum{
	address1 = 0,
	address2,
	address3,
	address4,
	address5,
	address6,
	address7,
	address8,
	address9,
}Address;

typedef struct{
	Page page;
	Address address_a;
	Address address_b;
	Address address_c;
	uint16_t add,sub;
}KeyControl;

void AcquireTime(void);
void SD_GPIO_Config(void);
void SD_Save(void);

void dataInit(void);
void send_power(void);
void receive_power(uint8_t receive);
void powerCalculate(void);
void hydrogenCalculate(void);
void dataShow(void);
void Reset(void);
#endif
