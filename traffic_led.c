#include "reg52.h"			

typedef unsigned int u16;	//对数据类型进行声明定义  
typedef unsigned char u8;

sbit LSA=P2^2;//38译码器
sbit LSB=P2^3;
sbit LSC=P2^4;
sbit k2=P3^2;//按键
sbit k1=P3^3;

#define GPIO_DIG   P0
#define GPIO_TRAFFIC P1

sbit RED10   = P1^0;   
sbit GREEN10 = P1^1;   
sbit RED11   = P1^2;
sbit YELLOW11= P1^3;
sbit GREEN11 = P1^4;

sbit RED00   = P3^0;	
sbit GREEN00 = P3^1;	
sbit RED01   = P1^5;
sbit YELLOW01= P1^6;
sbit GREEN01 = P1^7;

u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值

u8 DisplayData[8];
u8 Second,Second2;

void delay(u16 i)//延迟函数
{
	while(i--);	
}


void DigDisplay()////数码管动态显示
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;
			case(1):
				LSA=1;LSB=0;LSC=0; break;
			case(2):
				LSA=0;LSB=1;LSC=0; break;
			case(3):
				LSA=1;LSB=1;LSC=0; break;
			case(4):
				LSA=0;LSB=0;LSC=1; break;
			case(5):
				LSA=1;LSB=0;LSC=1; break;
			case(6):
				LSA=0;LSB=1;LSC=1; break;
			case(7):
				LSA=1;LSB=1;LSC=1; break;
		}
		GPIO_DIG=DisplayData[i];//发送段码
		delay(100); //间隔一段时间扫描	
		GPIO_DIG=0x00;//消隐
	}
}

void Int0Init()//外部中断0初始化
{
	
	IT0=1;
	EX0=1;
	EA=1;
}

void Int1Init()//外部程序1初始化
{
	
	IT1=1;
	EX1=1;
	EA=1;
}


void Int0()	interrupt 0	//中断程序0内容
{
	delay(1000);	//按键软件防抖 
	if(k2==0)
	{   
		 TR0=0;
		 TR1=1;
		Second2=1;
		while(Second2<21)
		{
		  DisplayData[0] = 0x00;
			DisplayData[1] = 0x00;
			DisplayData[2] = smgduan[(21 - Second2) % 100 / 10];
			DisplayData[3] = smgduan[(21 - Second2) %10];
			DisplayData[4] = 0x00;
			DisplayData[5] = 0x00;
			DisplayData[6] = DisplayData[2];
			DisplayData[7] = DisplayData[3];
			DigDisplay();
			GPIO_TRAFFIC = 0xFF;  
			RED00 = 1;
			GREEN00 = 1;

			RED11 = 0;    			
			RED10	= 0;    

			RED01 = 0;      
			RED00 = 0;      
	}
}
		TR0=1;
    TR1=0;
	}

void Int1() interrupt 2//中断程序1内容
{
	delay(1000);//按键软件防抖	 
	if(k1==0)
	{   
		 TR0=0;
		 TR1=1;
		Second2=1;
		while(Second2<16)
		{
		  DisplayData[0] = 0x00;
			DisplayData[1] = 0x00;
			DisplayData[2] = smgduan[(16 - Second2) % 100 / 10];
			DisplayData[3] = smgduan[(16 - Second2) %10];
			DisplayData[4] = 0x00;
			DisplayData[5] = 0x00;
			DisplayData[6] = DisplayData[2];
			DisplayData[7] = DisplayData[3];
			DigDisplay();
	    GPIO_TRAFFIC = 0xFF;
			RED00 = 1;
			GREEN00 = 1;
			RED11 = 0;       			
			RED10 = 0;       

			GREEN01 = 0;     
			GREEN00 = 0;     
			if(k2==0){break;}
		}
	}
	  TR0=1;
	  TR1=0;
}
			

void Timer0Init()//计数器0初始化
{
	TMOD|=0X01;

	TH0=0XFC;	
	TL0=0X18;	
	ET0=1;
	EA=1;
	TR0=1;			
}

void Timer1Init()//计数器1初始化
{
	TMOD|=0X10;
	TH1=0XFC;
	TL1=0X18;
	PT1=1;
	ET1=1;
	EA=1;
	TR1=0;
}
void Timer0() interrupt 1//选择为定时器0模式，工作方式1，仅用TR0打开启动
{
	static u16 i;
	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;
	i++;
	if(i==1000)
	{
		i=0;
		Second ++;	
	}	
}

void Timer1() interrupt 3//选择为定时器1模式，工作方式1，仅用TR1打开启动
{
	static u16 j;
	TH1=0XFC;
	TL1=0X18;
	j++;
	if(j==1000)
	{
		j=0;
		Second2 ++;
	}
}

void main()//主程序
{	
	Second = 1;

	Timer0Init();
	Timer1Init();
	Int0Init();
	Int1Init();
	while(1)
	{
		if(Second == 37)
		{
			Second = 1;
		}

		//--宝田路通行，30秒--//
		if(Second < 11)
		{
			DisplayData[0] = 0x00;
			DisplayData[1] = 0x00;
			DisplayData[2] = smgduan[(10 - Second) % 100 / 10];
			DisplayData[3] = smgduan[(10 - Second) %10];
			DisplayData[4] = 0x00;
			DisplayData[5] = 0x00;
			DisplayData[6] = DisplayData[2];
			DisplayData[7] = DisplayData[3];
			DigDisplay();

			//--宝田路通行--//

			GPIO_TRAFFIC = 0xFF;  //将所有的灯熄灭
			RED00 = 1;
			GREEN00 = 1;

			GREEN11 = 0;    //宝田路绿灯亮			
			GREEN10	= 0;    //宝田路人行道绿灯亮

			RED01 = 0;      //前进路红灯亮
			RED00 = 0;      //前进路人行道红灯亮
		}

		//--黄灯等待切换状态，5秒--//
		else if(Second < 13) 
		{
			DisplayData[0] = 0x00;
			DisplayData[1] = 0x00;
			DisplayData[2] = smgduan[(12 - Second) % 100 / 10];
			DisplayData[3] = smgduan[(12 - Second) %10];
			DisplayData[4] = 0x00;
			DisplayData[5] = 0x00;
			DisplayData[6] = DisplayData[2];
			DisplayData[7] = DisplayData[3];
			DigDisplay();

			//--黄灯阶段--//

			GPIO_TRAFFIC = 0xFF;  //将所有的灯熄灭
			RED00 = 1;
			GREEN00 = 1;

			YELLOW11 = 0;    //宝田路黄灯亮			
			RED10	= 0;     //宝田路人行道红灯亮

			YELLOW01 = 0;    //前进路红灯亮
			RED00 = 0;       //前进路人行道红灯亮
		}

		//--前进路通行--//
		else if(Second < 33) 
		{
			DisplayData[0] = 0x00;
			DisplayData[1] = 0x00;
			DisplayData[2] = smgduan[(32 - Second) % 100 / 10];
			DisplayData[3] = smgduan[(32 - Second) %10];
			DisplayData[4] = 0x00;
			DisplayData[5] = 0x00;
			DisplayData[6] = DisplayData[2];
			DisplayData[7] = DisplayData[3];
			DigDisplay();

			//--黄灯阶段--//

			GPIO_TRAFFIC = 0xFF;  //将所有的灯熄灭
			RED00 = 1;
			GREEN00 = 1;

			RED11 = 0;       //宝田路红灯亮			
			RED10 = 0;       //宝田路人行道红灯亮

			GREEN01 = 0;     //前进路绿灯亮
			GREEN00 = 0;     //前进路人行道绿灯亮
		}

		//--黄灯等待切换状态，5秒--//
		else 
		{
			DisplayData[0] = 0x00;
			DisplayData[1] = 0x00;
			DisplayData[2] = smgduan[(37 - Second) % 100 / 10];
			DisplayData[3] = smgduan[(37 - Second) %10];
			DisplayData[4] = 0x00;
			DisplayData[5] = 0x00;
			DisplayData[6] = DisplayData[2];
			DisplayData[7] = DisplayData[3];
			DigDisplay();

			//--黄灯阶段--//

			GPIO_TRAFFIC = 0xFF;  //将所有的灯熄灭
			RED00 = 1;
			GREEN00 = 1;

			YELLOW11 = 0;    //宝田路黄灯亮			
			RED10	= 0;     //宝田路人行道红灯亮

			YELLOW01 = 0;    //前进路红灯亮
			RED00 = 0;       //前进路人行道红灯亮
		}
	}
}


