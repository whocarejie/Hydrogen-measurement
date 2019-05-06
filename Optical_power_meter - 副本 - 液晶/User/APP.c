#include "APP.h"
#include <math.h>
#include "lcd.h"
#include "usart3.h"
#include "buzzer.h"
#include "usart4.h"
#include "rtc.h"
#include "stdio.h"
#include "hmi.h"
Power power;
Hydrogen hydrogen;
KeyControl keyControl;

unsigned char Data_Buffer[50];  //�洢SD��
unsigned char Time_Buffer[50];
int File_Name_Change = TRUE;
unsigned char year,month,week,date;
unsigned char hour,minute,second;
uint8_t sendData[6] = {0xAA,0x55,0x03,0xFF,0x01,0x02};		//����ֵ

float value1,value2;
int wanwanwan=0;			//shandiao
float ratio[4];

//tiaoshi
uint8_t show_value0[8];

bool flag = 0;
bool flag2 = 1;

void dataInit(void)
{	
	power.value1 = 0.0;
	power.value2 = 0.0;
	power.ratio  = 0.0;
	
	hydrogen.density = 0.0;
	hydrogen.a 			 = 9.008599;
	hydrogen.b 			 = 8.984437;
	hydrogen.c 			 = -148.893738;
	
	keyControl.page  			= page1;
	keyControl.address_a 	= address1;
	keyControl.address_b  = address1;
	keyControl.address_c  = address1;
	
	//menu show
	/*LCD_ShowString(10,50,90,1,16,"POWER:");
	LCD_ShowString(70,50,64,1,16,"CH1:");
	LCD_ShowString(200,50,32,1,16,"UW");
	LCD_ShowString(70,70,64,1,16,"CH2:");
	LCD_ShowString(200,70,32,1,16,"UW");
	LCD_ShowString(70,90,130,1,16,"CH1/CH2:");
	LCD_DrawLine(5, 110, 310, 110);
	LCD_ShowString(10,120,160,1,16,"HYDROGEN:");
	LCD_ShowString(200,120,48,1,16,"ppm");
	
	LCD_DrawLine(5, 150, 310, 150);*/
}

//SD���洢��������
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
//����ʱ��
/*time_t now;
char time_buf [80];
void AcquireTime(void)
{
	
    struct tm * ts;
    now = time(NULL);
    ts = localtime(&now);
    strftime (time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", ts);
    //printf ("%s", time_buf);

}*/

/*void AcquireTime(void)
{
	 time_t timep;
    struct tm *p;
    time (&timep);
    p=gmtime(&timep);
    
}*/

//SD�����ݴ洢
void SD_Save(void)		                              // ������     
{
	    u8 t=0;
    
	    delay_ms(2200);	
      
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
			//AcquireTime();
			if(t!=calendar.sec)
        {
           t=calendar.sec;
					
					sprintf(Data_Buffer,"%d-%d-%d %d:%d:%d	CH1:%4.5f	CH2:%4.5f	CH1/CH2:%2.5f	HYDROGEN:%5.2f\r\n",
					calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec,power.value1,power.value2,power.ratio,hydrogen.density);
				}
			if(!File_Name_Change)                 //  �ļ����Ѹ���
			{	
				if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)&&GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))             //  SD����׼���ã�����ģ���ڿ���״̬
				{
					UART4_SendString(DEBUG_USARTx,Data_Buffer);			// ͨ������4����ʱ�����¶�����
				}
			}			
}


void send_power(void)
{
	unsigned int i;
	for(i=0; i<6; i++){
		printf("%c",sendData[i]);	
	}
}

void receive_power(uint8_t receive)
{
	static uint16_t count = 0;
	uint16_t i;
	uint8_t receiveData[30];
	if(receive == 0xAA)	
		count = 0;
	receiveData[count++] = receive;
	if(count == 30){
		if(receiveData[0]==0xAA && receiveData[1]==0x55){
			__nop();
			for(i=0; i<3; i++){
				power.rxData1[i] = (int) receiveData[i+5];
				power.rxData2[i] = (int) receiveData[i+17];
			}
		}
	}
}

void powerCalculate(void)
{
	value1 = (float) ((power.rxData1[0]&127)*65536 + power.rxData1[1]*256 + power.rxData1[2])/1000;
	value2 = (float) ((power.rxData2[0]&127)*65536 + power.rxData2[1]*256 + power.rxData2[2])/1000;
	value1 = (power.rxData1[0]>=128)?(-1*value1):(value1);
	value2 = (power.rxData2[0]>=128)?(-1*value2):(value2);
	power.value1 = (pow(10,value1/10))*1000;
	power.value2 = (pow(10,value2/10))*1000;
	power.ratio  = power.value1/power.value2;
}

void hydrogenCalculate(void)
{
	static int i = 0;
	static float vlaue_0 = 0.0;
	
	//�洢power.ratio
	ratio[i++] = power.ratio;
	if(i>3)	//����
	{
		if(flag2)
		if(ratio[0]>ratio[1] && ratio[1]>ratio[2] &&
			ratio[2]>ratio[3])	//�ж�ͻȻͨ������б�ʱ仯
		{
			vlaue_0 = power.ratio;
			flag2 = 0;
			flag = 1;		
		}
		
		if(flag){
			hydrogen.density = (vlaue_0>=power.ratio)?((vlaue_0-power.ratio)*K+B):(0.5);
			if(power.ratio - vlaue_0>Thre){
				flag2 = 1;
				flag = 0;
				vlaue_0 = 0.0;
			}
		}
		else
			hydrogen.density = (abs(ratio[3]-ratio[2]))*K+B;
		
		if(hydrogen.density >= H2_thre)
			BUZZER_ON;
		else
			BUZZER_OFF;
		
		i = 0;	//���
	}
	
	//////////
	sprintf(show_value0,"%5.2f",vlaue_0);
	
	//hydrogen.density = exp(hydrogen.a+hydrogen.b*power.ratio+hydrogen.c*power.ratio*power.ratio);
	
	switch(keyControl.page){
		case page1:
			switch(keyControl.address_a){
				case address1:
					if(keyControl.add){
						hydrogen.a++;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.a--;
						keyControl.sub = 0;		//clear
					}
					break;
				case address2:
					if(keyControl.add){
						hydrogen.a = hydrogen.a+0.1;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.a = hydrogen.a-0.1;
						keyControl.sub = 0;		//clear
					}
					break;
				case address3:
					if(keyControl.add){
						hydrogen.a = hydrogen.a+0.01;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.a = hydrogen.a-0.01;
						keyControl.sub = 0;		//clear
					}
					break;
				case address4:
					if(keyControl.add){
						hydrogen.a = hydrogen.a+0.001;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.a = hydrogen.a-0.001;
						keyControl.sub = 0;		//clear
					}
					break;
				case address5:
					if(keyControl.add){
						hydrogen.a = hydrogen.a+0.0001;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.a = hydrogen.a-0.0001;
						keyControl.sub = 0;		//clear
					}
					break;
				case address6:
					if(keyControl.add){
						hydrogen.a = hydrogen.a+0.00001;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.a = hydrogen.a-0.00001;
						keyControl.sub = 0;		//clear
					}
					break;
			}
			break;
		case page2:
			switch(keyControl.address_b){
				case address1:
					if(keyControl.add){
						hydrogen.b++;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.b--;
						keyControl.sub = 0;		//clear
					}
					break;
				case address2:
					if(keyControl.add){
						hydrogen.b = hydrogen.b+0.1;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.b = hydrogen.b-0.1;
						keyControl.sub = 0;		//clear
					}
					break;
				case address3:
					if(keyControl.add){
						hydrogen.b = hydrogen.b+0.01;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.b = hydrogen.b-0.01;
						keyControl.sub = 0;		//clear
					}
					break;
				case address4:
					if(keyControl.add){
						hydrogen.b = hydrogen.b+0.001;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.b = hydrogen.b-0.001;
						keyControl.sub = 0;		//clear
					}
					break;
				case address5:
					if(keyControl.add){
						hydrogen.b = hydrogen.b+0.0001;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.b = hydrogen.b-0.0001;
						keyControl.sub = 0;		//clear
					}
					break;
				case address6:
					if(keyControl.add){
						hydrogen.b = hydrogen.b+0.00001;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.b = hydrogen.b-0.00001;
						keyControl.sub = 0;		//clear
					}
					break;
			}
			break;
		case page3:
			switch(keyControl.address_c){
				case address1:
					if(keyControl.add){
						hydrogen.c = hydrogen.c+100;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.c = hydrogen.c-100;
						keyControl.sub = 0;		//clear
					}
					break;
				case address2:
					if(keyControl.add){
						hydrogen.c = hydrogen.c+10;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.c = hydrogen.c-10;
						keyControl.sub = 0;		//clear
					}
					break;
				case address3:
					if(keyControl.add){
						hydrogen.c++;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.c--;
						keyControl.sub = 0;		//clear
					}
					break;
				case address4:
					if(keyControl.add){
						hydrogen.c = hydrogen.c+0.1;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.c = hydrogen.c-0.1;
						keyControl.sub = 0;		//clear
					}
					break;
				case address5:
					if(keyControl.add){
						hydrogen.c = hydrogen.c+0.01;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.c = hydrogen.c-0.01;
						keyControl.sub = 0;		//clear
					}
					break;
				case address6:
					if(keyControl.add){
						hydrogen.c = hydrogen.c+0.001;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.c = hydrogen.c-0.001;
						keyControl.sub = 0;		//clear
					}
					break;
				case address7:
					if(keyControl.add){
						hydrogen.c = hydrogen.c+0.0001;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.c = hydrogen.c-0.0001;
						keyControl.sub = 0;		//clear
					}
					break;
				case address8:
					if(keyControl.add){
						hydrogen.c = hydrogen.c+0.00001;
						keyControl.add = 0;		//clear
					}
					else if(keyControl.sub){
						hydrogen.c = hydrogen.c-0.00001;
						keyControl.sub = 0;		//clear
					}
					break;
			}
			break;
	}
}

void HMISendCleen(void)          //Ϊȷ������HMI����ͨ��
{ 
	  HMISends("cls RED");  //���ʹ���ָ��
		HMISendb(0xff);		  //���ͽ����� 0XFF 0XFF 0XFF
		delay_ms(1000);
	  HMISends("page 0");
		HMISendb(0xff);
	  delay_ms(2000);
}
void GUI_HMI(void)
{
	  char Value1[1] ,Value2[1],Ratio[1],Density[1];
	  char buff1[64];
	  int IntValue1 ,IntValue2,IntRatio,IntDensity;
	  int Quxian_Value;
	  
//	  hydrogen.density=1.0;
//	  power.value1=2.123;
//	  power.value2=2.3;
//	  power.ratio=3.4;
	  
	  IntValue1=(int)(power.value1*10000);
	  IntValue2=(int)(power.value2*10000);
	  IntRatio=(int)(power.ratio*10000);
	  IntDensity=(int)(hydrogen.density*1000);
	  Quxian_Value = IntDensity/10;
	  sprintf(buff1,"add 1,0,%d",Quxian_Value); //��������Ҫ���㴦��
		HMISends(buff1);
	 	HMISendb(0xff);
	  //delay_ms(1000);
	  sprintf(Value1,"%d",IntValue1);
	  HMISends("x3.val="); //ch1
	  HMISends(Value1);
	  HMISendb(0xff);
		sprintf(Value2,"%d",IntValue2);
	  HMISends("x2.val="); //ch2
	  HMISends(Value2);
	  HMISendb(0xff);
		sprintf(Ratio,"%d",IntRatio);
	  HMISends("x1.val="); //ratio
	  HMISends(Ratio);
	  HMISendb(0xff);
		sprintf(Density,"%d",IntDensity);
	  HMISends("x0.val="); //destiny
	  HMISends(Density);
	  HMISendb(0xff);
		
}

//��λ
extern char Rec_Buffer[];
extern char SendData[];

extern u8 USART_REC_Finish_FLAG;
void Reset(void)
{
	char ch;
	char reset[] = "$RESET!\r";				//������ص��ַ���     \n ���Ե���
	while(USART_REC_Finish_FLAG == 1)						//�ȴ����ڽ�����ɱ�־λ��1
		{	
			delay_ms(1000);			//��ʱ1ms
			
			USART_REC_Finish_FLAG = 0;		//ʹ����ɺ󽫴��ڱ�־λ�é�			
			if(strcmp(SendData, reset)==0)						//�Ƚ��ַ��� 
			{
				ch = '1';
				printf("LED1 ON\r\n");
				__set_FAULTMASK(1); // �ر��ж�
        NVIC_SystemReset(); // ��λ
			}
			else
			{ch = '3';	
       printf("NO");
			}	
		}
}

void dataShow(void)
{
	//power
	sprintf(power.valueStr1,"%4.5f",power.value1);
	LCD_ShowString(100,50,100,1,16,power.valueStr1);
	UART3_SendString("A");
	UART3_SendString(power.valueStr1);
	sprintf(power.valueStr2,"%4.5f",power.value2);
	LCD_ShowString(100,70,100,1,16,power.valueStr2);
	UART3_SendString("B");
	UART3_SendString(power.valueStr2);
	sprintf(power.ratioStr,"%2.5f",power.ratio);
	LCD_ShowString(132,90,100,1,16,power.ratioStr);
	
	//hydrogen
	sprintf(hydrogen.densityStr,"%5.2f",hydrogen.density);
	LCD_ShowString(100,120,64,1,16,hydrogen.densityStr);
	UART3_SendString("C");
	UART3_SendString(hydrogen.densityStr);
	sprintf(hydrogen.aStr,"%4.5f",hydrogen.a);
	LCD_ShowString(50,160,160,1,16,hydrogen.aStr);
	sprintf(hydrogen.bStr,"%4.5f",hydrogen.b);
	LCD_ShowString(50,180,160,1,16,hydrogen.bStr);
	sprintf(hydrogen.cStr,"%4.5f",hydrogen.c);
	LCD_ShowString(50,200,160,1,16,hydrogen.cStr);
	
	LCD_ShowString(150,200,160,1,16,show_value0);
	
	switch(keyControl.page){
		case page1:
			LCD_ShowString(20,160,48,1,16,">>>");
			LCD_ShowString(20,180,48,1,16,"   ");
			LCD_ShowString(20,200,48,1,16,"   ");
			switch(keyControl.address_a){
				case address1:
				  LCD_Fill(50,176,58,176,BRED);
					LCD_Fill(66,176,74,176,WHITE);
					LCD_Fill(74,176,82,176,WHITE);
					LCD_Fill(82,176,90,176,WHITE);
					LCD_Fill(90,176,98,176,WHITE);
					LCD_Fill(98,176,106,176,WHITE);
					break;
				case address2:
					LCD_Fill(50,176,58,176,WHITE);
					LCD_Fill(66,176,74,176,BRED);
					LCD_Fill(74,176,82,176,WHITE);
					LCD_Fill(82,176,90,176,WHITE);
					LCD_Fill(90,176,98,176,WHITE);
					LCD_Fill(98,176,106,176,WHITE);
					break;
				case address3:
					LCD_Fill(50,176,58,176,WHITE);
					LCD_Fill(66,176,74,176,WHITE);
					LCD_Fill(74,176,82,176,BRED);
					LCD_Fill(82,176,90,176,WHITE);
					LCD_Fill(90,176,98,176,WHITE);
					LCD_Fill(98,176,106,176,WHITE);
					break;
				case address4:
					LCD_Fill(50,176,58,176,WHITE);
					LCD_Fill(66,176,74,176,WHITE);
					LCD_Fill(74,176,82,176,WHITE);
					LCD_Fill(82,176,90,176,BRED);
					LCD_Fill(90,176,98,176,WHITE);
					LCD_Fill(98,176,106,176,WHITE);
					break;
				case address5:
					LCD_Fill(50,176,58,176,WHITE);
					LCD_Fill(66,176,74,176,WHITE);
					LCD_Fill(74,176,82,176,WHITE);
					LCD_Fill(82,176,90,176,WHITE);
					LCD_Fill(90,176,98,176,BRED);
					LCD_Fill(98,176,106,176,WHITE);
					break;
				case address6:
					LCD_Fill(50,176,58,176,WHITE);
					LCD_Fill(66,176,74,176,WHITE);
					LCD_Fill(74,176,82,176,WHITE);
					LCD_Fill(82,176,90,176,WHITE);
					LCD_Fill(90,176,98,176,WHITE);
					LCD_Fill(98,176,106,176,BRED);
					break;
			}
			break;
		case page2:
			LCD_ShowString(20,160,48,1,16,"   ");
			LCD_ShowString(20,180,48,1,16,">>>");
			LCD_ShowString(20,200,48,1,16,"   ");
			switch(keyControl.address_b){
				case address1:
					LCD_Fill(50,196,58,196,BRED);
					LCD_Fill(66,196,74,196,WHITE);
					LCD_Fill(74,196,82,196,WHITE);
					LCD_Fill(82,196,90,196,WHITE);
					LCD_Fill(90,196,98,196,WHITE);
					LCD_Fill(98,196,106,196,WHITE);
					break;
				case address2:
					LCD_Fill(50,196,58,196,WHITE);
					LCD_Fill(66,196,74,196,BRED);
					LCD_Fill(74,196,82,196,WHITE);
					LCD_Fill(82,196,90,196,WHITE);
					LCD_Fill(90,196,98,196,WHITE);
					LCD_Fill(98,196,106,196,WHITE);
					break;
				case address3:
					LCD_Fill(50,196,58,196,WHITE);
					LCD_Fill(66,196,74,196,WHITE);
					LCD_Fill(74,196,82,196,BRED);
					LCD_Fill(82,196,90,196,WHITE);
					LCD_Fill(90,196,98,196,WHITE);
					LCD_Fill(98,196,106,196,WHITE);
					break;
				case address4:
					LCD_Fill(50,196,58,196,WHITE);
					LCD_Fill(66,196,74,196,WHITE);
					LCD_Fill(74,196,82,196,WHITE);
					LCD_Fill(82,196,90,196,BRED);
					LCD_Fill(90,196,98,196,WHITE);
					LCD_Fill(98,196,106,196,WHITE);
					break;
				case address5:
					LCD_Fill(50,196,58,196,WHITE);
					LCD_Fill(66,196,74,196,WHITE);
					LCD_Fill(74,196,82,196,WHITE);
					LCD_Fill(82,196,90,196,WHITE);
					LCD_Fill(90,196,98,196,BRED);
					LCD_Fill(98,196,106,196,WHITE);
					break;
				case address6:
					LCD_Fill(50,196,58,196,WHITE);
					LCD_Fill(66,196,74,196,WHITE);
					LCD_Fill(74,196,82,196,WHITE);
					LCD_Fill(82,196,90,196,WHITE);
					LCD_Fill(90,196,98,196,WHITE);
					LCD_Fill(98,196,106,196,BRED);
					break;
			}
			break;
		case page3:
			LCD_ShowString(20,160,48,1,16,"   ");
			LCD_ShowString(20,180,48,1,16,"   ");
			LCD_ShowString(20,200,48,1,16,">>>");
			switch(keyControl.address_c){
				case address1:
					LCD_Fill(58,216,66,216,BRED);
					LCD_Fill(66,216,74,216,WHITE);
					LCD_Fill(74,216,82,216,WHITE);
					LCD_Fill(90,216,98,216,WHITE);
					LCD_Fill(98,216,106,216,WHITE);
					LCD_Fill(106,216,114,216,WHITE);
					LCD_Fill(114,216,122,216,WHITE);
					LCD_Fill(122,216,130,216,WHITE);
					break;
				case address2:
					LCD_Fill(58,216,66,216,WHITE);
					LCD_Fill(66,216,74,216,BRED);
					LCD_Fill(74,216,82,216,WHITE);
					LCD_Fill(90,216,98,216,WHITE);
					LCD_Fill(98,216,106,216,WHITE);
					LCD_Fill(106,216,114,216,WHITE);
					LCD_Fill(114,216,122,216,WHITE);
					LCD_Fill(122,216,130,216,WHITE);
					break;
				case address3:
					LCD_Fill(58,216,66,216,WHITE);
					LCD_Fill(66,216,74,216,WHITE);
					LCD_Fill(74,216,82,216,BRED);
					LCD_Fill(90,216,98,216,WHITE);
					LCD_Fill(98,216,106,216,WHITE);
					LCD_Fill(106,216,114,216,WHITE);
					LCD_Fill(114,216,122,216,WHITE);
					LCD_Fill(122,216,130,216,WHITE);
					break;
				case address4:
					LCD_Fill(58,216,66,216,WHITE);
					LCD_Fill(66,216,74,216,WHITE);
					LCD_Fill(74,216,82,216,WHITE);
					LCD_Fill(90,216,98,216,BRED);
					LCD_Fill(98,216,106,216,WHITE);
					LCD_Fill(106,216,114,216,WHITE);
					LCD_Fill(114,216,122,216,WHITE);
					LCD_Fill(122,216,130,216,WHITE);
					break;
				case address5:
					LCD_Fill(58,216,66,216,WHITE);
					LCD_Fill(66,216,74,216,WHITE);
					LCD_Fill(74,216,82,216,WHITE);
					LCD_Fill(90,216,98,216,WHITE);
					LCD_Fill(98,216,106,216,BRED);
					LCD_Fill(106,216,114,216,WHITE);
					LCD_Fill(114,216,122,216,WHITE);
					LCD_Fill(122,216,130,216,WHITE);
					break;
				case address6:
					LCD_Fill(58,216,66,216,WHITE);
					LCD_Fill(66,216,74,216,WHITE);
					LCD_Fill(74,216,82,216,WHITE);
					LCD_Fill(90,216,98,216,WHITE);
					LCD_Fill(98,216,106,216,WHITE);
					LCD_Fill(106,216,114,216,BRED);
					LCD_Fill(114,216,122,216,WHITE);
					LCD_Fill(122,216,130,216,WHITE);
					break;
				case address7:
					LCD_Fill(58,216,66,216,WHITE);
					LCD_Fill(66,216,74,216,WHITE);
					LCD_Fill(74,216,82,216,WHITE);
					LCD_Fill(90,216,98,216,WHITE);
					LCD_Fill(98,216,106,216,WHITE);
					LCD_Fill(106,216,114,216,WHITE);
					LCD_Fill(114,216,122,216,BRED);
					LCD_Fill(122,216,130,216,WHITE);
					break;
				case address8:
					LCD_Fill(58,216,66,216,WHITE);
					LCD_Fill(66,216,74,216,WHITE);
					LCD_Fill(74,216,82,216,WHITE);
					LCD_Fill(90,216,98,216,WHITE);
					LCD_Fill(98,216,106,216,WHITE);
					LCD_Fill(106,216,114,216,WHITE);
					LCD_Fill(114,216,122,216,WHITE);
					LCD_Fill(122,216,130,216,BRED);
					break;
			}
			break;	
	}
	
}

///////////////////�����жϷ���/////////////////////
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		keyControl.page++;
		if(keyControl.page>page3) keyControl.page = page1;
		EXTI_ClearITPendingBit(EXTI_Line1);     //����жϱ�־λ
	}  
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		switch(keyControl.page){
			case page1:
				keyControl.address_a++;
				if(keyControl.address_a>address6)	keyControl.address_a = address1;
				break;
			case page2:	
				keyControl.address_b++;
				if(keyControl.address_b>address6)	keyControl.address_b = address1;
				break;
			case page3:
				keyControl.address_c++;
				if(keyControl.address_c>address9)	keyControl.address_c = address1;
				break;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);     //����жϱ�־λ
	}  
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		keyControl.add = 1;
		EXTI_ClearITPendingBit(EXTI_Line3);     //����жϱ�־λ
	}  
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		keyControl.sub = 1;
		EXTI_ClearITPendingBit(EXTI_Line4);     //����жϱ�־λ
	}  
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		wanwanwan++;
		if(wanwanwan%2)
			BUZZER_ON;
		else
			BUZZER_OFF;
		EXTI_ClearITPendingBit(EXTI_Line13);
	}  
}

