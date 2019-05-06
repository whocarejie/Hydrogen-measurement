/*********************************************************************************/	 
//STM32����ʾ������
//�������ϣ�www.tftmcu.taobao.com
//��Ƭ��STM32F103VCT6  �ⲿ����8MHZ  �����ʣ�9600									  
/*********************************************************************************/	
#include "hmi.h"
//u8 idfind(u8 *buf,u8 *val,u8 len);

void HMISendstart(void)
{
	 	delay_ms(200);
		HMISendb(0xff);
		delay_ms(200);
}

void HMISends(char *buf1)		  //�ַ������ͺ���
{
	u8 i=0;
	while(1)
	{
	 if(buf1[i]!=0)
	 	{
			USART_SendData(USART1,buf1[i]);  //����һ���ֽ�
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		 	i++;
		}
	 else 
	 return ;

		}
	}
void HMISendb(u8 k)		         //�ֽڷ��ͺ���
{		 
	u8 i;
	 for(i=0;i<3;i++)
	 {
	 if(k!=0)
	 	{
			USART_SendData(USART1,k);  //����һ���ֽ�
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		}
	 else 
	 return ;

	 } 
} 

void HMISendn(u16 k)		         //��������
{		 
	USART_SendData(USART1,k);  //����һ���ֽ�
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
	return ;
} 



