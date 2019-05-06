/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_exti.h"

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;                  //��ʼ���жϷ��麯��

	//NVIC_PriorityGroupConfig:�������ȼ����飨����һ�䣩
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);        //�������ȼ����������ȼ�0���������ȼ�4λ��16����
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;        // ʹ�����õ��ⲿ�ж�ͨ������

	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;     //���жϹҵ�PA1���ⲿ�ж���1��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;     //���������ȼ�1
	NVIC_Init(&NVIC_InitStructure);                      //��ʼ��

	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;     //���жϹҵ�PA2���ⲿ�ж���2��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;     //���������ȼ�2��Ҫ����ǰ��������ȼ�1
	NVIC_Init(&NVIC_InitStructure);                      //��ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;     //���жϹҵ�PA2���ⲿ�ж���2��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;     //���������ȼ�2��Ҫ����ǰ��������ȼ�1
	NVIC_Init(&NVIC_InitStructure);                      //��ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;     //���жϹҵ�PA2���ⲿ�ж���2��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x04;     //���������ȼ�2��Ҫ����ǰ��������ȼ�1
	NVIC_Init(&NVIC_InitStructure);                      //��ʼ��
  
  /* ����P[A|B|C|D|E]13Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x05;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  ���� PC13 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_PC13_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PC13) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG| RCC_APB2Periph_AFIO,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* EXTI line gpio config*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* config the NVIC(PC13) */
	NVIC_Configuration();

	//EXTI_ClearITPendingBit(EXTI_Line1);                   //�����1 IO���ж��������λ������жϱ�־λ��
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;     //�����ⲿ�жϴ�������һ�����¼�������
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //�����жϴ�����ʽ���½��ش�����ʽ
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;              //ѡ���ж���·Ϊ1����ѡ���Ǹ�IO��Ϊ�ж����룩
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;               //ʹ���ⲿ�ж�
	EXTI_Init(&EXTI_InitStructure);                       //��ʼ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1); //��GPIOA1�ҵ��ж���

	//EXTI_ClearITPendingBit(EXTI_Line2);                   //�����2 IO���ж��������λ������жϱ�־λ��
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;     
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //�����жϴ�����ʽ�������ش�����ʽ
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;              //ѡ���ж���·Ϊ2
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;               //ʹ���ⲿ�ж�
	EXTI_Init(&EXTI_InitStructure);                       //��ʼ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0); //��GPIOA2�ҵ��ж���
	
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;     
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //�����жϴ�����ʽ�������ش�����ʽ
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;              //ѡ���ж���·Ϊ2
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;               //ʹ���ⲿ�ж�
	EXTI_Init(&EXTI_InitStructure);                       //��ʼ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3); //��GPIOA2�ҵ��ж���
	
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;     
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //�����жϴ�����ʽ�������ش�����ʽ
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;              //ѡ���ж���·Ϊ2
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;               //ʹ���ⲿ�ж�
	EXTI_Init(&EXTI_InitStructure);                       //��ʼ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource4); //��GPIOA2�ҵ��ж���

	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
}
/*********************************************END OF FILE**********************/
