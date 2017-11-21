#include "reg52.h"
typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;
int a[8]={2,0,1,5,1,1,8,6};//学号
int timer;//交通灯计时
int b[2]={0,0};//把timer转化为二位数组
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
#define GPIO_KEY P1
int k4=1,k5=1;

u8 KeyValue;	//用来存放读取到的键值

u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值

void DigDisplay();
void jiaotong_led();
void GetKey();
void delay(u16 i);
void jiaotongdeng();
void KeyDown();


void delay(u16 i)//延时函数
{
	while(i--);	
}

void keypros()//按键软件防抖
{
	KeyDown();
	GetKey();
	do{
	if(k4==0)		  //检测按键K1是否按下
	{	
		delay(1000);   //消除抖动 一般大约10ms
		GetKey();
		if(k4==0)	 //再次判断按键是否按下
		{
			DigDisplay();
		}
	}
}	while(!k4);	 //检测按键是否松开
		
//	do{
	if(k5==0)		  //检测按键K1是否按下
	{	
		delay(100);   //消除抖动 一般大约10ms
		GetKey();
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
		P0=0x00;//消隐
	}
}

void jiaotongdeng()//交通灯计时程序
{
	timer--;
	delay(20000);
	if(timer ==0)
	{
		timer = 30;
	}
}


void jiaotong_led()//交通灯亮灯程序
{
	u8 i;
	//b[0]=(int)timer/10;
	b[0]=(int)timer%10;
	for(i=0;i<1;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			//case(1):
				//LSA=1;LSB=0;LSC=0; break;//显示第1位
		}
		P0=smgduan[b[i]];//发送段码
		delay(10000); //间隔一段时间扫描	
		//P0=0x00;
	}
}

void KeyDown(void)//矩阵键盘程序
{
	char a=0;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)//读取按键是否按下
	{
		delay(1000);//延时10ms进行消抖
		if(GPIO_KEY!=0x0f)//再次检测键盘是否按下
		{	
			//测试列
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=1;break;
				case(0X0d): KeyValue=2;break;
				case(0X0e):	KeyValue=3;break;
			}
			//测试行
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
			}
			while((a<50)&&(GPIO_KEY!=0xf0))	 //检测按键松手检测
			{
				delay(1000);
				a++;
			}
		}
	}
}
void GetKey()
{
	KeyDown();
	if(KeyValue==4)
		k4=0;
		else k4=1;
	KeyDown();
	if(KeyValue==5)
		k5=0;
		else k5=1;
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
