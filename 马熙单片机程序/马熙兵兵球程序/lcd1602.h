/*********************************************************************
 *
 *                           LCD1602.H
 *
 *********************************************************************
 * 文件名: LCD1602.H
 * 描述: 
 * 公司: 爱晶电子
 *
 * 作者              		日期         			注释
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 王伟民					08/04/01				原始文件
 ********************************************************************/
#ifndef LCD1602_H
#define LCD1602_H

/*变量类型标识的宏定义,通常做法*/
#define uchar unsigned char
#define uint unsigned int

/*注：液晶的管脚在LCD1602.H中定义*/
/*单片机与LCD1602连接数据线定义*/
sbit LCD_RS  = P1^0;     //RS为寄存器选择，1=数据寄存器、0=指令寄存器。
sbit LCD_RW  = P1^1;      //RW为读写信号线，1=读操作，0=写操作。
                   //当RS=RW=0可以写入指令或者显示地址，
                   //当RS=0，RW=1可以读忙信号，
                   //当RS=1，RW=0可以写入数据。
sbit LCD_EN	 = P2^5;      //当en端由高电平跳变成低电平时，液晶模块执行命令。

sbit LCD_BUSY=P0^7;    //忙指示
#define LCD_DATA P0  //命令或数据线

#define LCD_DATA_IN  {\ 
P0M1=0xff;\ 
P0M0=0x00;\ 
} /*单片机的LCD数据口设置为全输入*/

	
#define LCD_DATA_OUT  {\ 
P0M1=0x00;\ 
P0M0=0xff;\ 
} /*单片机的LCD数据口设置为全输出*/	

#define LCD_CTR_OUT   {\ 
P1M1&=0xfc;/*0b11111100*/\
P1M0|=0x03;/*0b00000011*/\
P2M1&=0xdf;/*0b11011111*/\
P2M0|=0x20;/*0b00100000*/\
} /*设置单片机LCD控制引脚RW\RS\EN全为输出*/

#define SOME_NOP   {\ 
   	_nop_();\
	_nop_();\
	_nop_();\
	_nop_();\
} /*设置单片机LCD控制引脚RW\RS\EN全为输出*/

	

/*-------------------1602液晶命令定义-------------------*/

/*初始化命令*/
#define DISP_CLR		0x01/*0b00000001.清显示,光标复位到第一行首(地址00H）*/
#define CUR_HOME		0x02/*0b00000010.光标复位，光标返回到第一行首*/

/*光标和显示模式设置*/
#define CUR_AUTO_R		0x06/*0b00000110.设置光标在文字插入后自动右移*/
#define CUR_AUTO_L		0x04/*0b00000100.设置光标在文字插入后自动左移*/
#define DISP_AUTO_R		0x07/*0b00000111.设置显示在文字插入后自动右移*/
#define DISP_AUTO_L		0x05/*0b00000101.设置显示在文字插入后自动左移*/

/*显示开关控制*/
#define DISP_ON			0x0f/*0b00001111.显示开*/
#define DISP_OFF		0x0b/*0b00001011.显示关*/
#define CUR_ON			0x0f/*0b00001111.光标开*/
#define CUR_OFF			0x0d/*0b00001101.光标关*/
#define BLINK_ON		0x0f/*0b00001111.光标闪烁开*/
#define BLINK_OFF		0x0e/*0b00001110.光标闪烁关*/

/*光标或显示移位*/
#define CUR_SHIFT_L		0x10/*0b00010000.光标左移*/
#define CUR_SHIFT_R		0x14/*0b00010100.光标右移*/
#define DISP_SHIFT_L		0x18/*0b00011000.显示左移*/
#define DISP_SHIFT_R		0x1c/*0b00011100.显示右移*/


/*工作模式设置*/
#define LCD_MODE_STD		0x38		/*1602标准工作模式：8位数据传送，2行显示，字符点阵5*7*/


/*字符发生器RAM地址设置*/
#define SET_ADDR_CGRAM		0x40	/*设置CGRAM地址*/

/*数据存储器地址设置*/
#define SET_ADDR_DDRAM		0x80	/*设置DDRAM地址*/

/*自定义字符*/
#define USER_CHAR1				0x00/*自定义字符1:年*/
#define USER_CHAR2				0x01/*自定义字符2:月*/
#define USER_CHAR3				0x02/*自定义字符3:日*/
#define USER_CHAR4				0x03/*自定义字符4:￥*/
#define USER_CHAR5				0x04/*自定义字符5:元*/
#define USER_CHAR6				0x05/*自定义字符6:℃*/
#define USER_CHAR7				0x06/*自定义字符7:↑*/
#define USER_CHAR8				0x07/*自定义字符8:小喇叭图标*/

/* 函数原型*/
void LCD_init(void)			;/*初始化*/
void wait_until_LCDready(void)	;/*检测忙标志，忙则等待*/
void LCD_wrcmd(unsigned char cmd)		;/*写入控制命令*/
void LCD_setxy(char x,char y)	;/*设定显示位置，行x=1/2,列y=1~16的任意整数*/
void LCD_wrchar(char str)		;/*写入要显示的字符*/
void LCD_wrstr(char code *s);/*写入要显示的字符串常量，该函数相当于C18 库中的putrsXLCD函数，从程序存储器写一个字符串到LCD*/
void LCD_wrul(unsigned long num);/*写入要显示的无符号长整型数*/
void LCD_wrlval(unsigned long num,unsigned char bits,unsigned char dp);/*写入要显示的长变量*/
void CG_Write(void);/*建立自定义字符块*/


#endif
