#include "bsp_usart1.h"
#include "delay.h"
#include "APP.h"
#include "lcd.h"
#include "bsp_exti.h"
#include "usart3.h"
#include "buzzer.h"
#include "usart4.h"
#include "rtc.h"
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */

extern char Rec_Buffer[];
extern char SendData[];

extern u8 USART_REC_Finish_FLAG;

int a = 0;
int main(void)
{
	
	USART1_Config();
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	USART_Config();										//����3�ĳ�ʼ��
	USART_NVIC_Config();							//����3���ж�����	
	EXTI_PC13_Config();
	BUZZER_GPIO_Config();
	
	delay_init();
	//lcd
//	LCD_Init();
//	POINT_COLOR=BRED;      //������ɫ
//	BACK_COLOR=WHITE;      //��������ɫ
//	LCD_Display_Dir(0);		 	//Ĭ��Ϊ����
//	LCD_Clear(YELLOW);	

	dataInit();
	
	UART4_Config();								   						// ���ڳ�ʼ��
  SD_GPIO_Config();                        //�˿ڳ�ʼ��
  RTC_Init();
	HMISendstart();
	HMISendCleen();  //����ˢ��
	HMISendb(0xff);
  HMISends("page 1");
  HMISendb(0xff); 
  delay_ms(1000);	
	while(1)
	{
		//Power
//		send_power();		//���Ͷ�ȡ����ָ��
//		delay_ms(200);
//		powerCalculate();	//��ȡ����
//		hydrogenCalculate();	//��������
//		__nop();
//		
//		//Show
//		dataShow();
//		SD_Save();
		GUI_HMI();
		Reset();
	
	}
}
/*********************************************END OF FILE**********************/
