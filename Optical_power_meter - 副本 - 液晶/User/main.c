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
  * @brief  主函数
  * @param  无
  * @retval 无
  */

extern char Rec_Buffer[];
extern char SendData[];

extern u8 USART_REC_Finish_FLAG;

int a = 0;
int main(void)
{
	
	USART1_Config();
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级

	USART_Config();										//串口3的初始化
	USART_NVIC_Config();							//串口3的中断配置	
	EXTI_PC13_Config();
	BUZZER_GPIO_Config();
	
	delay_init();
	//lcd
//	LCD_Init();
//	POINT_COLOR=BRED;      //字体颜色
//	BACK_COLOR=WHITE;      //开窗口颜色
//	LCD_Display_Dir(0);		 	//默认为横屏
//	LCD_Clear(YELLOW);	

	dataInit();
	
	UART4_Config();								   						// 串口初始化
  SD_GPIO_Config();                        //端口初始化
  RTC_Init();
	HMISendstart();
	HMISendCleen();  //开机刷屏
	HMISendb(0xff);
  HMISends("page 1");
  HMISendb(0xff); 
  delay_ms(1000);	
	while(1)
	{
		//Power
//		send_power();		//发送读取功率指令
//		delay_ms(200);
//		powerCalculate();	//读取功率
//		hydrogenCalculate();	//氢气计算
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
