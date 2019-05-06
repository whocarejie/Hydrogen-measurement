/************************************************************************************
Copyright,  LCE STUDIO  �������õ��Ӽ��� 
�������ƣ�����TF��/U��ģ�����ݴ洢����
����˵��: 
			1. ����Ƶ��18.432MHz��������STC15ϵ��˫���ڻ��Ĵ��ڵ�Ƭ����
			2. ���Ĵ洢�ļ�����
			3. ÿ2���ӣ���ȡDHT11���ݣ�ʱ�����ݣ���OLED��ʾ������ʾ��
			4. ÿ2���ӣ�ͨ������2������ʪ����ʱ�����ݣ���5ms�ڣ�δ�ٷ����µ����ݣ�TF��/U��ģ�齫�յ������ݴ洢��TF����U���С�
����汾��1.0  
����������rob1983
�Ա����̣�https://lces.taobao.com 
************************************************************************************/


#include "SD.h"
#include "usart4.h"
#include "APP.h"
void SD_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����GPIOC������ʱ��*/
		RCC_APB2PeriphClockCmd( SD_ST_GPIO_CLK | SD_BUY_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIOC����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	//ST״̬λ��busy״̬λ

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIOC*/
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
// ����: void main(void)
// ����: ������
//========================================================================
			
void SD_Save(void)		                              // ������     
{

		
		//WriteRTC();                             // д��ʱ��
  	//SYS_Init();															// �˿ڳ�ʼ��
		
    //while (1)		                           	// ��ѭ�� 
    //{
			delay_ms(2200);	
			//RH_Report();                          // ��ȡ�¶�ֵ
			//ReadRTC();                            // ��ȡʱ��
			if(File_Name_Change)                  // �����ļ���
			{
				if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)&&GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))
				{
					SD_BUY_LOW;                      //  ��BUY�˿�����
					delay_ms(200);  					
					USART_SendData(DEBUG_USARTx,0x24);           				//  ������Ԫ��
					UART4_SendString(DEBUG_USARTx,"20190101");         //  ����8λ�ļ���
					SD_BUY_HIGH;                           //  ��BUY�˿�����
					delay_ms(5);		
					File_Name_Change	= FALSE;        //  �ļ����Ѹ��� 				
				}	
			}
			sprintf(Data_Buffer,"%d  ",power.value1);
			if(!File_Name_Change)                 //  �ļ����Ѹ���
			{	
				if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)&&GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))             //  SD����׼���ã�����ģ���ڿ���״̬
				{
					UART4_SendString(DEBUG_USARTx,Data_Buffer);			// ͨ������2����ʱ�����¶�����
				}
			}			
    }
//}



/********************** ��RTC���� ************************/
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
/********************** дRTC���� ************************/

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





