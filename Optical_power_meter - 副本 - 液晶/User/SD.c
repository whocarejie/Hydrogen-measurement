/************************************************************************************
Copyright,  LCE STUDIO  深圳松悦电子技术 
程序名称：串口TF卡/U盘模块数据存储程序
功能说明: 
			1. 晶振频率18.432MHz，适用于STC15系列双串口或四串口单片机。
			2. 更改存储文件名。
			3. 每2秒钟，读取DHT11数据，时钟数据，在OLED显示屏上显示。
			4. 每2秒钟，通过串口2发送温湿度与时钟数据，在5ms内，未再发送新的数据，TF卡/U盘模块将收到的数据存储在TF卡或U盘中。
程序版本：1.0  
阿里旺旺：rob1983
淘宝店铺：https://lces.taobao.com 
************************************************************************************/


#include "SD.h"
#include "usart4.h"
#include "APP.h"
void SD_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启GPIOC的外设时钟*/
		RCC_APB2PeriphClockCmd( SD_ST_GPIO_CLK | SD_BUY_GPIO_CLK, ENABLE); 

		/*选择要控制的GPIOC引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	//ST状态位和busy状态位

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOC*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);		  
	 
	  
}
			 

//extern unsigned char xdata message[35];
//unsigned char year,month,week,date;
//unsigned char hour,minute,second;
unsigned char Data_Buffer[50];
int File_Name_Change = TRUE;
extern  power;
//void	ReadRTC(void);
//void	WriteRTC(void);

//========================================================================
// 函数: void main(void)
// 描述: 主函数
//========================================================================
			
void SD_Save(void)		                              // 主函数     
{

		
		//WriteRTC();                             // 写入时间
  	//SYS_Init();															// 端口初始化
		
    //while (1)		                           	// 主循环 
    //{
			delay_ms(2200);	
			//RH_Report();                          // 读取温度值
			//ReadRTC();                            // 读取时间
			if(File_Name_Change)                  // 更改文件名
			{
				if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)&&GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))
				{
					SD_BUY_LOW;                      //  将BUY端口拉低
					delay_ms(200);  					
					USART_SendData(DEBUG_USARTx,0x24);           				//  发送美元符
					UART4_SendString(DEBUG_USARTx,"20190101");         //  发送8位文件名
					SD_BUY_HIGH;                           //  将BUY端口拉高
					delay_ms(5);		
					File_Name_Change	= FALSE;        //  文件名已更改 				
				}	
			}
			sprintf(Data_Buffer,"%d  ",power.value1);
			if(!File_Name_Change)                 //  文件名已更改
			{	
				if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)&&GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))             //  SD卡已准备好，而且模块在空闲状态
				{
					UART4_SendString(DEBUG_USARTx,Data_Buffer);			// 通过串口2发送时间与温度数据
				}
			}			
    }
//}



/********************** 读RTC函数 ************************/
/*void	ReadRTC(void)
{
	unsigned char xdata	tmp[7];
  float u2 =2*1.2;
	unsigned char *q;
	
	ReadNbyte(2, tmp, 7);
	second = ((tmp[0] >> 4) & 0x07) * 10 + (tmp[0] & 0x0f);
	minute = ((tmp[1] >> 4) & 0x07) * 10 + (tmp[1] & 0x0f);
  hour   = ((tmp[2] >> 4) & 0x03) * 10 + (tmp[2] & 0x0f);
	date   = ((tmp[3] >> 4) & 0x03) * 10 + (tmp[3] & 0x0f);
	week   =   tmp[4] ;
	month  = ((tmp[5] >> 4) & 0x01) * 10 + (tmp[5] & 0x0f);
	year   = ((tmp[6] >> 4) & 0x0f) * 10 + (tmp[6] & 0x0f);
	q = message;
	sprintf(Data_Buffer,"%02d-%02d-%02d %02d:%02d:%02d %s OK\r\n",(int)year,(int)month,(int)date,(int)hour,(int)minute,(int)second,q);
//	LCD_P6x8Str(0,0,TIME);
	SendString(Data_Buffer);	
	
	 
}*/
/********************** 写RTC函数 ************************/

/*void	WriteRTC(void)
{
	unsigned char	xdata tmp[7];
	
	second = 1;
	minute = 1;
	hour   = 1;
	date   = 18;
	week   = 1;
	month  = 11;
	year   = 17;	
	
	tmp[0] = ((second / 10) << 4) + (second % 10);
	tmp[1] = ((minute / 10) << 4) + (minute % 10);
	tmp[2] = ((hour / 10) << 4) + (hour % 10);
	tmp[3] = ((date/ 10) << 4) + (date % 10);
	tmp[4] = week % 10;
	tmp[5] = ((month / 10) << 4) + (month % 10);
	tmp[6] = ((year / 10) << 4) + (year % 10);
	WriteNbyte(2, tmp, 7);
}*/





