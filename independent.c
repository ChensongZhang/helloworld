#include "reg52.h"
typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;
int a[8]={2,0,1,5,1,1,8,6};
int timer;
int b[2]={0,0};
int key5_state;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
sbit k4=P2^0;	 //定义P10口是k1
sbit k5=P2^1;

u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值
void DigDisplay();
void jiaotong_led();
void GetKey();
void delay(u16 i);
void jiaotongdeng();

void delay(u16 i)//延时函数
{
	while(i--);	
}

void DigDisplay();
void  jiaotong_led();

void keypros()//按键软件防抖
{
	do{
	if(k4==0)		  //检测按键K1是否按下
	{	
		delay(1000);   //消除抖动 一般大约10ms
		if(k4==0)	 //再次判断按键是否按下
		{
			DigDisplay();
		}
	}
}	while(!k4);	 //检测按键是否松开
		
//	do{
	if(k5==0)		  //检测按键K1是否按下
	{	
		delay(1000);   //消除抖动 一般大约10ms
		if(k5==0)	 //再次判断按键是否按下
		{
			jiaotong_led();
			delay(50000); 
		}
	}
//}	while(!k5);	 //检测按键是否松开
		
}

void DigDisplay()//数码管动态显示
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=0; break;//显示第1位
			case(2):
				LSA=0;LSB=1;LSC=0; break;//显示第2位
			case(3):
				LSA=1;LSB=1;LSC=0; break;//显示第3位
			case(4):
				LSA=0;LSB=0;LSC=1; break;//显示第4位
			case(5):
				LSA=1;LSB=0;LSC=1; break;//显示第5位
			case(6):
				LSA=0;LSB=1;LSC=1; break;//显示第6位
			case(7):
				LSA=1;LSB=1;LSC=1; break;//显示第7位	
		}
		P0=smgduan[a[i]];//发送段码
		delay(100); //间隔一段时间扫描	
		//P0=0x00;//消隐
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
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=0; break;//显示第1位
		}
		P0=smgduan[b[i]];//发送段码
		delay(100); //间隔一段时间扫描	
		//P0=0x00;//消隐
	}
}




void main()
{	
	timer=30;
	while(1)
	{	
		keypros();  //按键处理函数	
		jiaotongdeng();
	}		
}
