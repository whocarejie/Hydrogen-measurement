#include "buzzer.h"

void BUZZER_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����GPIOC������ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIOC����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIOC*/
		GPIO_Init(GPIOG, &GPIO_InitStructure);		  

		/* �ر�����led��	*/
		GPIO_SetBits(GPIOG, GPIO_Pin_11);	 
}
