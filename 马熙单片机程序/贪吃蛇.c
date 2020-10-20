#include<reg51.h>
#include"gui.h"
#include"R61505V.h"

void delay(uchar z) // 延迟函数
{
uchar y;
for(z;z>0;z--)
   for(y=0;y<110;y++);
}

//这个必须两天之内完成，还有这么多的资料你还怕什么呢
//不同的是我要求在TFT凭上完成，其具体的GUI设计是重点。
/*********************************************************************
 *
 *                          贪吃蛇游戏核心程序
 *
 *********************************************************************
 * 文件名: snake.c
 * 描述: 贪吃蛇游戏核心程序
 * 公司: 爱晶电子
 * 作者              		日期         			注释
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 王伟民				2010/3/20				原始文件
 ********************************************************************/

//#include "stc.h"/*包含STC单片机头文件*/
//#include "top51.h"/*包含TOP51开发板头文件*/
#include "key_sweep.h"
//#include "LCM/ILI9325.h"
//#include "GUI/gui.h"
#include "snake.h"
//#include "delay.h"/*包含爱晶电子提供延时函数头文件*/

//=================================================================
//    使用结构体定义变量,使程序更加清楚
//=================================================================
struct 
{
	uchar X;	//食物位置
	uchar Y;			 
	uchar Yes;
}Food;	//定义食物结构体


struct 
{
	uchar X[SNAKE_MAX_LONG];
	uchar Y[SNAKE_MAX_LONG];
	uchar Long;  			//蛇的长度
	uchar Life; 			//蛇的生命
	uchar Score;			//蛇的分数
	uchar Level;			//蛇的等级              
}Snake;	//定义蛇结构体

uchar Key_V_temp=Key_Right;     	//取出上次按键值
uchar Over;     	//=1表示本轮或全部优秀结束


//=================================================================
//函数名称：void Init_time(void)
//函数功能：定时器0 : 控制蛇的运动速度
//定时器1 : 用于产生随机数种子 ，用于放置食物
//入口参数：无
//出口参数：无
//=================================================================
void Init_time (void)
{
	TMOD=0x11;/*设置定时器0、1为工作方式1*/
	TH0=0xb8;/*T0重新置初值*/
	TL0=0;
//	TH1=0X00;/*T1重新置初值*/
//	TL1=0X00;
	ET0=1;/*开定时器0中断*/
	//ET1=1;/*开定时器1中断*/
	EA=1;/*开总中断*/
	TR0=1;/*启动定时器0*/
	TR1=1;/*启动定时器1*/
}

/*---中断服务程序---*/
void timer0(void) interrupt 1
{
	unsigned int t;
	unsigned char a;
	TH0=0xb8;/*T0重新置初值*/
	TL0=0;
	a++;
	/*每次中断设计为0.01S*/
	/*修改下句可以调整游戏速度*/
	//t=120-Snake.Level*10;
t=100;
	if (a>t)/*设定时间到了吗？*/
	//if (a>100)/*设定时间到了吗？*/
	{
		a=0;/*是，时间到*/
		COL0=0;P0=0;COL1=0;COL2=0;COL3=0;/*暂停键盘扫描，避免数码管上出现乱码显示*/
    	Run_SNAKE();
		P0=0;/*本句作用：在键盘扫描过程中禁止4位数码管显示，避免数码管上出现乱码显示*/
	}
}


//void timer1 (void) interrupt 3
//{
//	TH1=0X00;
//	TL1=0X00;
//}


//=================================================================
//函数名称：Trun_On_Point     Trun_Off_Point (uchar x,uchar y)
//函数功能：点亮或者熄灭一个点。用于显示蛇身体
//入口参数：无
//出口参数：无
//特别说明：此 Trun_On，函数换成其他的就可以兼容其他的液晶
//          入8*8 点阵   12864 等
//=================================================================
void Trun_On_Point (uchar x,uint y,uint color)
{
	GUI_box((8*(x+1)),(8*(y+1)),(8*(x+2)),(8*(y+2)),color);
}

void Trun_Off_Point (uchar x,uint y)
{
	GUI_box((8*(x+1)),(8*(y+1)),(8*(x+2)),(8*(y+2)),0xBE5F);	
}

//=================================================================
//函数名称：void Init_SNAKE(void)  
//函数功能：绘制游戏界面，初始化游戏等级等参数
//入口参数：无
//出口参数：无
//=================================================================
void Init_SNAKE(void)   
{
	uchar i;

	Init_time();
	
	//***********初始化蛇参数********************//
	Snake.Long=2;	    			//定义初始化蛇的长度
	Snake.Life=0;	    			//初始化蛇还活着
	Snake.Score=0;
	Food.Yes=1;					
	Key_Value=Key_Right;
	Over=0;



	for(i=0;i<Snake.Long;i++)		//将蛇给赋值
	{
		Snake.X[i]=i+2;
		Snake.Y[i]=2;
	}
	
	//***********绘制游戏界面********************//
   LCD_clearscreen(0xBE5F);//清屏
	GUI_box(0,304,239,319,0x001f);
	GUI_sprintf_HZ(32, 304, 0x07ff,0x001f);//显示预定义汉字字符串
	GUI_Rectangle(8-1,8-1,184+1,288+1,0xaa);//绘制一个矩形框, 0-127
	GUI_sprintf_String(192,8,"SCORE",0x8b00,0xBE5F);
	GUI_sprintf_chartodec(200,24,Snake.Score,0xf800,0xBE5F);
 	GUI_sprintf_String(192,81,"LEVEL",0x8b00,0xBE5F);
	GUI_sprintf_chartodec(200,97,Snake.Level,0xf800,0xBE5F);
					
	for(i=0;i<Snake.Long;i++)		//将蛇给画出来
	{
		Trun_On_Point(Snake.X[i],Snake.Y[i],0x001f);
	}							
}
//=================================================================
//函数名称：void Display_dead(uchar i) 
//函数功能：显示死亡界面
//入口参数：无
//出口参数：无
//=================================================================
void Display_dead(void)
{
	GUI_sprintf_String(60,140,"GAME OVER",0xcc88,0xBE5F); 
	Over=1;
	TR0=0;/*停止定时器0，蛇停止运动*/
	P0=0;/*本句作用：在键盘扫描过程中禁止4位数码管显示，避免数码管上出现乱码显示*/
}
//=================================================================
//函数名称：void Display_Pass(uchar i)
//函数功能：显示过关界面
//入口参数：无
//出口参数：无
//=================================================================
void Display_Pass(void)
{
	Snake.Level++;	
	GUI_sprintf_String(68,124,"You are",0xcc88,0xBE5F); 
	GUI_sprintf_String(80,156,"WIN!",0xcc88,0xBE5F); 
	Over=1;
	TR0=0;/*停止定时器0，蛇停止运动*/
	P0=0;/*本句作用：在键盘扫描过程中禁止4位数码管显示，避免数码管上出现乱码显示*/
}
//=================================================================
//函数名称：void Run_SNAKE(void)
//函数功能：蛇运动函数 （游戏精华）
//入口参数：无
//出口参数：无
//=================================================================
void Run_SNAKE(void)
{
	uchar i=0;

	/*********************根据按键选择蛇的运动方向*******************/
	if((Key_Value==Key_Right&&Key_V_temp!=Key_Left)||(Key_Value==Key_Left&&Key_V_temp==Key_Right))
	{
		Trun_Off_Point(Snake.X[0],Snake.Y[0]); 	       //先把蛇尾给熄灭掉 

		for(i=0;i<Snake.Long-1;i++)                    //蛇身都是它前一段的蛇身的位置
		{
			Snake.X[i]=Snake.X[i+1];
			Snake.Y[i]=Snake.Y[i+1];
		} 

		Snake.X[Snake.Long-1]=Snake.X[Snake.Long-2]+1;
		Snake.Y[Snake.Long-1]=Snake.Y[Snake.Long-2]+0; //蛇头向你要求的方向移动
		
		for(i=0;i<Snake.Long;i++)			            //这个让蛇显示出来的函数
		{
			Trun_On_Point(Snake.X[i],Snake.Y[i],0x001f);
		}
		Key_V_temp=Key_Right;
	}

	if((Key_Value==Key_Left&&Key_V_temp!=Key_Right)||(Key_Value==Key_Right&&Key_V_temp==Key_Left))
	{

		Trun_Off_Point(Snake.X[0],Snake.Y[0]); 		   //先把蛇尾给熄灭掉 
		
		for(i=0;i<Snake.Long-1;i++)                     //蛇身都是它前一段的蛇身的位置
		{
			Snake.X[i]=Snake.X[i+1];
			Snake.Y[i]=Snake.Y[i+1];
		} 
		
		Snake.X[Snake.Long-1]=Snake.X[Snake.Long-2]-1;
		Snake.Y[Snake.Long-1]=Snake.Y[Snake.Long-2]+0; //蛇头向你要求的方向移动
		
		for(i=0;i<Snake.Long;i++)			            //这个让蛇显示出来的函数
		{
			Trun_On_Point(Snake.X[i],Snake.Y[i],0x001f);
		}
		Key_V_temp=Key_Left;
	}
	if(( Key_Value==Key_Up&&Key_V_temp!=Key_Down)||( Key_Value==Key_Down&&Key_V_temp==Key_Up))
	{
		Trun_Off_Point(Snake.X[0],Snake.Y[0]); 		  //先把蛇尾给熄灭掉 
		
		for(i=0;i<Snake.Long-1;i++)                   //蛇身都是它前一段的蛇身的位置
		{
			Snake.X[i]=Snake.X[i+1];
			Snake.Y[i]=Snake.Y[i+1];
		} 
		
		Snake.X[Snake.Long-1]=Snake.X[Snake.Long-2]+0;
		Snake.Y[Snake.Long-1]=Snake.Y[Snake.Long-2]-1; //蛇头向你要求的方向移动
		
		for(i=0;i<Snake.Long;i++)			           //这个让蛇显示出来的函数
		{
			Trun_On_Point(Snake.X[i],Snake.Y[i],0x001f);
		}
		Key_V_temp=Key_Up;
	}
	if(( Key_Value==Key_Down&&Key_V_temp!=Key_Up)||( Key_Value==Key_Up&&Key_V_temp==Key_Down))
	{
		Trun_Off_Point(Snake.X[0],Snake.Y[0]); 		 //先把蛇尾给熄灭掉 
		
		for(i=0;i<Snake.Long-1;i++)                  //蛇身都是它前一段的蛇身的位置
		{
			Snake.X[i]=Snake.X[i+1];
			Snake.Y[i]=Snake.Y[i+1];
		} 
		
		Snake.X[Snake.Long-1]=Snake.X[Snake.Long-2]+0;
		Snake.Y[Snake.Long-1]=Snake.Y[Snake.Long-2]+1; //蛇头向你要求的方向移动
		
		for(i=0;i<Snake.Long;i++)			           //这个让蛇显示出来的函数
		{
			Trun_On_Point(Snake.X[i],Snake.Y[i],0x001f);
		}
		Key_V_temp=Key_Down;
	}
	

	/****************根据按键选择蛇的运动程序结束*******************/
	/////////////////////////////////////////////////////////////////

	
	/************************判断蛇是否死亡***********************/
	if(Snake.X[Snake.Long-1]>=22||Snake.Y[Snake.Long-1]>=35)//判蛇头是否撞到墙壁
	{
		Snake.Life=1;
	}
	for(i=3;i<Snake.Long;i++)//从第三节开始判断蛇头是否咬到自己
	{
		if(Snake.X[i]==Snake.X[0]&&Snake.Y[i]==Snake.Y[0])
		{
			Snake.Life=1;
		}
	}
	if(Snake.Life==1)	  //if the snake over
	{
		Key_V_temp=Key_Value;
		Display_dead();
	}
	/******************判断蛇是否死亡程序结束***********************/
	/////////////////////////////////////////////////////////////////


	/************************判断蛇是否最长了**********************/
	if(Snake.Long==SNAKE_MAX_LONG)
	{
		Key_V_temp=Key_Value;
		Display_Pass();
	}
	/////////////////////////////////////////////////////////////////										   

	/*************************判蛇是否吃到食物**********************/		
	if(Snake.X[Snake.Long-1]==Food.X&&Snake.Y[Snake.Long-1]==Food.Y)//
	{
		//Trun_Off_Point(Food.X,Food.Y); 		 //消隐食物
		Snake.Long++;//蛇节数加1
		Snake.X[Snake.Long-1]=Food.X;
		Snake.Y[Snake.Long-1]=Food.Y;
		Snake.Score+=10;
		GUI_sprintf_chartodec(200,24,Snake.Score,0xcc88,0xBE5F);
		Food.Yes=1;  //食物标志置1
	}
	/////////////////////////////////////////////////////////////////
	

	/***************************放果子程序*************************/
	if(Food.Yes==0)
	{
		Trun_On_Point(Food.X,Food.Y,0xf81f);
	}											   
	if(Food.Yes==1)
	{
		while(1)
		{
			Food.X=TL1/12;      			//获得随机数
			if (Food.X>21)Food.Y=Food.Y-21;
			Food.Y=TL1/7;
			if (Food.Y>34)Food.Y=Food.Y-34;

		    for(i=0;i<Snake.Long;i++)				//判断产生的食物坐标是否和蛇身重合
			{
				if((Food.X==Snake.X[i])&&(Food.X==Snake.Y[i]))
					break;
			}
			if(i==Snake.Long)
			{
				Food.Yes=0;
				break;								//产生有效的食物坐标
			}
		}
	}
}


//以上是王伟民写的贪吃蛇的核心程序