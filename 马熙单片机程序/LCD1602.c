#include<reg51.h>
sbit LCD1602_E=P2^7; //定义引脚
sbit LCD1602_RS=P2^6;
sbit LCD1602_RW=P2^5;



/*******************************************************************************
* 函数名         : Delay1us
* 函数功能		   : 延时函数，延时1us
* 输入           : a
* 输出         	 : 无
*******************************************************************************/

void Delay1us(unsigned int a)
{
	unsigned int b,c;
	for(c=a;c>0;c--)
		for(b=110;b>0;b--);
}
/*******************************************************************************
* 函数名         : LCD1602_WriteCom
* 函数功能		   : 向LCD写入一个字节的命令
* 输入           : com
* 输出         	 : 无
*******************************************************************************/
	 //当没有定义这个LCD1602_4PINS时
void LCD1602_WriteCom(unsigned char com)	  //写入命令
{
	LCD1602_E=0;
	LCD1602_RS=0;
	LCD1602_RW=0;
	 Delay1us(20);
	P0=com;
	Delay1us(20);
	LCD1602_E=1;
	Delay1us(20);
	LCD1602_E=0;
}
/*******************************************************************************
* 函数名         : LCD1602_WriteData
* 函数功能		   : 向LCD写入一个字节的数据
* 输入           : dat
* 输出         	 : 无
*******************************************************************************/		   
		   
void LCD1602_WriteData(unsigned char dat)			//写入数据
{
	LCD1602_E=0;
	LCD1602_RS=1;
	LCD1602_RW=0;
	Delay1us(20);
	P0=dat;
	Delay1us(20);
	LCD1602_E=1;
	Delay1us(20);
	LCD1602_E=0;
}

/*******************************************************************************
* 函数名         : LCD1602_Init()
* 函数功能		   : 初始化LCD屏
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/		   

void LCD1602_Init()						  //LCD初始化子程序
{
 	LCD1602_WriteCom(0x38);  //开显示
	LCD1602_WriteCom(0x0c);  //开显示不显示光标
	LCD1602_WriteCom(0x06);  //写一个指针加1
	LCD1602_WriteCom(0x01);  //清屏
	LCD1602_WriteCom(0x80);  //设置数据指针起点
}


unsigned char code PuZh[] ="   Hello World  ";
unsigned char code PuZh1[]="I'm MaXi        ";
void main()
{
unsigned char i;
 LCD1602_Init();
 		Delay1us(0);

 
 
		   LCD1602_WriteCom(0x80);
		   													 
for	(i=0;i<16;i++)
	   {
	  
		LCD1602_WriteData(PuZh[i]);	
		Delay1us(2000);
		}
		   while(1);
	LCD1602_WriteCom(0xc0);
			for	(i=0;i<16;i++)
	   {LCD1602_WriteData(PuZh1[i]);
		 Delay1us(2000);
		 }
		 
		 
		 
	  	

}

 


