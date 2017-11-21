#include "reg52.h"
typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;
int a[8]={2,0,1,5,1,1,8,6};
int timer;
int b[2]={0,0};
int key5_state;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
sbit k4=P2^0;	 //����P10����k1
sbit k5=P2^1;

u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F��ֵ
void DigDisplay();
void jiaotong_led();
void GetKey();
void delay(u16 i);
void jiaotongdeng();

void delay(u16 i)//��ʱ����
{
	while(i--);	
}

void DigDisplay();
void  jiaotong_led();

void keypros()//�����������
{
	do{
	if(k4==0)		  //��ⰴ��K1�Ƿ���
	{	
		delay(1000);   //�������� һ���Լ10ms
		if(k4==0)	 //�ٴ��жϰ����Ƿ���
		{
			DigDisplay();
		}
	}
}	while(!k4);	 //��ⰴ���Ƿ��ɿ�
		
//	do{
	if(k5==0)		  //��ⰴ��K1�Ƿ���
	{	
		delay(1000);   //�������� һ���Լ10ms
		if(k5==0)	 //�ٴ��жϰ����Ƿ���
		{
			jiaotong_led();
			delay(50000); 
		}
	}
//}	while(!k5);	 //��ⰴ���Ƿ��ɿ�
		
}

void DigDisplay()//����ܶ�̬��ʾ
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ
			case(1):
				LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ
			case(2):
				LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ
			case(3):
				LSA=1;LSB=1;LSC=0; break;//��ʾ��3λ
			case(4):
				LSA=0;LSB=0;LSC=1; break;//��ʾ��4λ
			case(5):
				LSA=1;LSB=0;LSC=1; break;//��ʾ��5λ
			case(6):
				LSA=0;LSB=1;LSC=1; break;//��ʾ��6λ
			case(7):
				LSA=1;LSB=1;LSC=1; break;//��ʾ��7λ	
		}
		P0=smgduan[a[i]];//���Ͷ���
		delay(100); //���һ��ʱ��ɨ��	
		//P0=0x00;//����
	}
}

void jiaotongdeng()
{
	if(timer ==0)
	{
		timer = 30;
	}
	delay(100000);
	timer--;
}


void  jiaotong_led()
{
			u8 i;
	//b[0]=0;
	b[1]=(int)timer%10;
	b[0]=(int)timer/10;
	for(i=0;i<2;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ
			case(1):
				LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ
		}
		P0=smgduan[b[i]];//���Ͷ���
		delay(100); //���һ��ʱ��ɨ��	
		//P0=0x00;//����
	}
}




void main()
{	
	timer=30;
	while(1)
	{	
		keypros();  //����������	
		jiaotongdeng();
	}		
}
