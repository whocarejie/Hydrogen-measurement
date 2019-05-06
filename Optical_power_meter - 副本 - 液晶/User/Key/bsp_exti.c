/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   I/O线中断应用bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_exti.h"

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;                  //初始化中断分组函数

	//NVIC_PriorityGroupConfig:设置优先级分组（下面一句）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);        //设置优先级：抢先优先级0个，子优先级4位（16个）
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;        // 使能设置的外部中断通道请求

	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;     //将中断挂到PA1脚外部中断线1上
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;     //设置子优先级1
	NVIC_Init(&NVIC_InitStructure);                      //初始化

	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;     //将中断挂到PA2脚外部中断线2上
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;     //设置子优先级2，要低于前面的子优先级1
	NVIC_Init(&NVIC_InitStructure);                      //初始化
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;     //将中断挂到PA2脚外部中断线2上
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;     //设置子优先级2，要低于前面的子优先级1
	NVIC_Init(&NVIC_InitStructure);                      //初始化
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;     //将中断挂到PA2脚外部中断线2上
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x04;     //设置子优先级2，要低于前面的子优先级1
	NVIC_Init(&NVIC_InitStructure);                      //初始化
  
  /* 配置P[A|B|C|D|E]13为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x05;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  配置 PC13 为线中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void EXTI_PC13_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PC13) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG| RCC_APB2Periph_AFIO,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* EXTI line gpio config*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* config the NVIC(PC13) */
	NVIC_Configuration();

	//EXTI_ClearITPendingBit(EXTI_Line1);                   //清除线1 IO口中断清除挂起位（清除中断标志位）
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;     //设置外部中断触发（另一种是事件触发）
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //设置中断触发方式：下降沿触发方式
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;              //选择中断线路为1（即选择那个IO作为中断输入）
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;               //使能外部中断
	EXTI_Init(&EXTI_InitStructure);                       //初始化
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1); //将GPIOA1挂到中断上

	//EXTI_ClearITPendingBit(EXTI_Line2);                   //清除线2 IO口中断清除挂起位（清除中断标志位）
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;     
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //设置中断触发方式：上升沿触发方式
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;              //选择中断线路为2
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;               //使能外部中断
	EXTI_Init(&EXTI_InitStructure);                       //初始化
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0); //将GPIOA2挂到中断上
	
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;     
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //设置中断触发方式：上升沿触发方式
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;              //选择中断线路为2
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;               //使能外部中断
	EXTI_Init(&EXTI_InitStructure);                       //初始化
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3); //将GPIOA2挂到中断上
	
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;     
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //设置中断触发方式：上升沿触发方式
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;              //选择中断线路为2
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;               //使能外部中断
	EXTI_Init(&EXTI_InitStructure);                       //初始化
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource4); //将GPIOA2挂到中断上

	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
}
/*********************************************END OF FILE**********************/
