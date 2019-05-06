/*********************************************************************************/	 
//STM32串口示例程序
//更多资料：www.tftmcu.taobao.com
//单片机STM32F103VCT6  外部晶振8MHZ  波特率：9600									  
/*********************************************************************************/	
#include "hmi.h"
//u8 idfind(u8 *buf,u8 *val,u8 len);

void HMISendstart(void)
{
	 	delay_ms(200);
		HMISendb(0xff);
		delay_ms(200);
}

void HMISends(char *buf1)		  //字符串发送函数
{
	u8 i=0;
	while(1)
	{
	 if(buf1[i]!=0)
	 	{
			USART_SendData(USART1,buf1[i]);  //发送一个字节
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
		 	i++;
		}
	 else 
	 return ;

		}
	}
void HMISendb(u8 k)		         //字节发送函数
{		 
	u8 i;
	 for(i=0;i<3;i++)
	 {
	 if(k!=0)
	 	{
			USART_SendData(USART1,k);  //发送一个字节
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
		}
	 else 
	 return ;

	 } 
} 

void HMISendn(u16 k)		         //发送数据
{		 
	USART_SendData(USART1,k);  //发送一个字节
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
	return ;
} 



