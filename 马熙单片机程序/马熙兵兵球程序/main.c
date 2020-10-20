/*******************************************************************/
/*                      乒乓球游戏演示程序                     */
/*                                                                 */
/* 硬件平台: TOP51+12LE5A60S2   晶振：22.1184MHZ                 */ 
/* 软件平台: KEIL C51                                            */
/* 说明： 用SW0、SW1控制球拍位置，如果球拍不能接到球，就失败*/ 
/* TFT彩屏模块和12LE5A60S2的连接：                   	*/
/* TFT_RS（数据/命令，第4脚）		---- P1.0          	*/
/* TFT_WR（写，第5脚） 				---- P1.1   		*/
/* TFT_RD（读，第6脚）   			---- P4.6   		*/
/* TFT_CS（片选，第15脚）  			---- P4.5         	*/
/* TFT_RST（复位，第17脚） 			---- P4.4       	*/
/* TFT_DATA（8位数据口，第7--14脚）	---- P0   			*/
/* 公司: 爱晶电子                                                   */
/* 网址：http://www.ajdz.net                                        */
/* 作者              		日期         		注释                  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* 王伟民            2010-5-4          原始程序*/

#include "stc.h"/*包含STC单片机头文件*/
#include "top51.h"/*包含TOP51开发板头文件*/
#include "LCM/ILI9325.h"/*包含TFT彩屏驱动函数头文件*/
#include "GUI/gui.h"/*包含爱晶电子提供GUI函数头文件*/
#include "delay.h"/*包含爱晶电子提供延时函数头文件*/
//#include "uart.h"/*包含爱晶电子提供串口相关函数头文件*/

unsigned char x=71,dir=1;
unsigned int y=166;

unsigned char racket_x=20;
unsigned char racket_dir;
#define RACKET_HALF_LENGTH 20/*定义球拍半长度*/

void main(void)
{
	top51_init();/*top51主板初始化*/
	delayms(20);
		
	ILI9325_initial();/*TFT彩屏初始化*/
	delayms(20);

	GUI_clearscreen(0);//清屏

	COL0=1;/*选中4位数码管的最右边一位，并使SW0-SW2可作为独立按键使用*/
	P0=0;/*数码管不显示*/
	
	TMOD=0x11;/*设置定时器0、1为工作方式1*/

	TH0=0xac;/*T0重新置初值*/
	TL0=0;


	ET0=1;/*开定时器0中断*/
	ET1=1;/*开定时器1中断*/
	EA=1;/*开总中断*/

	TR0=1;/*启动定时器0*/
	TR1=1;/*停止定时器1*/

	GUI_line(racket_x-RACKET_HALF_LENGTH,219,racket_x+RACKET_HALF_LENGTH,219,0xffff);/*画出球拍*/

	while(1)
	{
		if (SW1==0) 
		{
			TR1=1;/*启动定时器1*/
			racket_dir=0;/*球拍朝左运动*/
		}
		/*如果SW0按下，*/
		else if (SW2==0)/*如果SW1按下，*/
		{
			TR1=1;/*启动定时器1*/
			racket_dir=1;/*球拍朝右运动*/
		}
		else 
		{
			TR1=0;/*停止定时器1*/
			TH1=0Xff;/*保证按下键时，响应时间最短，以实现对按键的快速响应*/
			TL1=0Xff;
		}
	}
}


/*---球拍运动中断服务程序---*/
void timer1 (void) interrupt 3
{
	TH1=0Xb8;/*T1重新置初值(10ms)*/
	TL1=0X00;
	COL0=0;
	if (racket_dir==0)
	{
		racket_x--;
		if (racket_x<RACKET_HALF_LENGTH) racket_x=RACKET_HALF_LENGTH;
		GUI_Point(racket_x+RACKET_HALF_LENGTH+1, 219, 0);/*消点*/
		GUI_Point(racket_x-RACKET_HALF_LENGTH, 219, 0XFFFF);/*画点*/
	}
	else 
	{
		racket_x++;
		if (racket_x>239-RACKET_HALF_LENGTH) racket_x=175-RACKET_HALF_LENGTH;
		GUI_Point(racket_x-RACKET_HALF_LENGTH-1, 219, 0);/*消点*/
		GUI_Point(racket_x+RACKET_HALF_LENGTH, 219, 0XFFFF);/*画点*/
	}
	P0=0;
	COL0=1;
}


/*---小球运动中断服务程序---*/
void Timer0_Routine(void) interrupt 1
{
	unsigned char n;
	TH0=0xb8;/*T0重新置初值(10ms)*/
	TL0=0;

	COL0=0;
	GUI_Point(x, y, 0);/*消点*/

	/*球运动到下一位置*/
	switch (dir)
	{
		case 1:	x++;y--;break;/*方向1运动*/
		case 2:	x--;y--;break;/*方向2运动*/
		case 3:	x--;y++;break;/*方向3运动*/
		case 4:	x++;y++;break;/*方向4运动*/
	}

	GUI_Point(x, y, 0xffff);/*画点*/
	/*是否碰到边界检测,如碰到边界，则改变运动方向*/
	if (x==0)
	{
		if (dir==2)	dir=1;/*方向1运动*/
			else dir=4;
	}
	if (x==175)
	{
		if (dir==1)	dir=2;/*方向1运动*/
			else dir=3;
	}
	if (y==0)
	{
		if (dir==1)	dir=4;/*方向1运动*/
			else dir=3;
	}
	if (y==218)
	{
		if ((x<racket_x+RACKET_HALF_LENGTH)&&(x>racket_x-RACKET_HALF_LENGTH))/*判断是否被球拍接住*/
		{
			/*被球拍接住*/
			if (dir==3)	dir=2;/*方向1运动*/
			else dir=1;
		}
		else
		{
			/*没有被球拍接住*/
			GUI_sprintf_string(84,140,"GAME OVER",0xcc88,0xBE5F); /*显示游戏结束*/
			while(1);
		}
	}
	P0=0;
	COL0=1;
}



