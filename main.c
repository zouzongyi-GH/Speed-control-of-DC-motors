#include <REGX51.H>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned int
sbit P30=P3^0;
sbit P31=P3^1;
sbit P32=P3^2;
sbit P10=P1^0;
unsigned int flag=0,compare=80;
unsigned char temp;
void delay(unsigned int xms)		//@11.0592MHz
{
	unsigned char i, j;
	while(xms)
	{
		_nop_();
		_nop_();
		_nop_();
		i = 11;
		j = 190;
		do
		{
			while (--j);
		} while (--i);
		xms--;
	}
}

timer_send()//*********************定时通信
{
	SCON=0x50;//****************方式2发送接收
	PCON=0x00;
	TMOD=0X21; //******************T0定时方式1   T1串口通信方式2
	TH0=(65536-500)/256; //********定时0.5ms
	TL0=(65536-500)%256;
	TH1=TL1=0XFD;//*****************波特率9600
	EA=1;
	ES=1;
	TR1=1;
	ET0=1;
	TF0=1;
	TR0=0;
	while(1)
	{
		switch(temp)
		{
			case 0x31:compare=80;P0=0x06;delay(200);break;//*********一档（50%）
			case 0x32:compare=50;P0=0x5b;delay(200);break;//*********二档（50%）
			case 0x33:compare=20;P0=0x4f;delay(200);break;//*********三档（80%
		}
		if(!P31||temp==0x31)
		{
			TR0=1;
		}
	};
}
//***************************************主函数
void main()
{
	P3_7=0;delay(20);
	P10=0;delay(20);
	 //...........启动
		timer_send();
}
//************************************************虚拟终端
void fasong()interrupt 4 using 2 //**********串口通信T1方式2 
{
	if(RI==1)
	{
		RI=0;
		temp=SBUF;//**************接收数据
		SBUF=temp;//**************发送数据
		while(!TI);
		TI=0;	
	}
}
//************************************************定时
void timer0() interrupt 1 using 1 //T0定时器中断
{
	TH0=(65536-1000)/256;
	TL0=(65536-1000)%256;
//***********************************生成PWM方波
	 if(flag<=compare)
		{
			P10=0;
		} 
			else
		{
			P10=1;
		}
			if(flag==100)
		{
			flag=0; 	
		}
		flag++;

		if(!P31)//******************一挡（20%）
		{	
			compare=80;delay(20);P0=0x06;
		}
		if(!P30)//******************二档（50%）
		{	
			compare=50;delay(20);P0=0x5b;
		}
		if(!P32)//******************三档（80%）
		{
			compare=20;delay(20);P0=0x4f;
		}
}






