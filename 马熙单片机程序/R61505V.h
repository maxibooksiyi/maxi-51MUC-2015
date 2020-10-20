#ifndef __R61505V_H
#define __R61505V_H

//---包含头文件---//
#include<reg51.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

//---定义使用的IO口---//
sbit	  TFT_RST = P3^3;  	//复位
sbit	  TFT_CS  = P2^7;	//片选
sbit	  TFT_RS  = P2^6;	//数据命令选择端
sbit	  TFT_WR  = P2^5;	//读写控制
sbit      TFT_RD  = P3^2;   //读写控制

#define   TFT_DATAPORTH  P1 //放置高8位数据
#define   TFT_DATAPORTL  P0 //放置低8位数据

//---定义屏的大小---//
#define TFT_XMAX 175//		//设置TFT屏的大小
#define TFT_YMAX 219//

//---定义全局变量---//
void TFT_WriteCmd(uint cmd);
void TFT_WriteData(uint dat);
void TFT_WriteCmdData(uint cmd, uint dat);
void TFT_Init(void);
void TFT_SetWindow(uint xStart, uint yStart, uint xEnd, uint yEnd);
void TFT_ClearScreen(uint color);

#endif

