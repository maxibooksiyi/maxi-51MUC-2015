/*********************************************************************
 *
 *                           top51.h
 *
 *********************************************************************
 * 文件名: top51.h
 * 描  述: TOP51开发板头文件
 * 公  司: 爱晶电子
 * 作者              		日期         			注释
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 王伟民		09/04/01				原始文件
 * 王伟民		09/10/20				v1.1
 ********************************************************************/

#ifndef TOP51_H
#define TOP51_H

/*变量类型标识的宏定义,通常做法*/
#define uchar unsigned char
#define uint unsigned int
#define ON 1
#define OFF 0

/*定义晶振频率（单位 Hz）*/
//#define FOSC  11059200 
//#define FOSC  12000000 
#define FOSC  22118400 


/* top51 引脚定义 */
sbit D0  = P0^0;
sbit D1  = P0^1;
sbit D2  = P0^2;
sbit D3  = P0^3;
sbit D4  = P0^4;
sbit D5  = P0^5;
sbit D6  = P0^6;
sbit D7  = P0^7;

sbit RS  = P1^0;   //RS为寄存器选择，1=数据寄存器、0=指令寄存器。
sbit RW  = P1^1;   //RW为读写信号线，1=读操作，0=写操作。
                   //当RS=RW=0可以写入指令或者显示地址，
                   //当RS=0，RW=1可以读忙信号，
                   //当RS=1，RW=0可以写入数据。
sbit IO  = P1^2;
sbit AN  = P1^3;
sbit DAC  = P1^4;
sbit SDI  = P1^5;
sbit SDO  = P1^6;
sbit SCK  = P1^7;

sbit COL0 = P2^0;
sbit COL1= P2^1;
sbit COL2= P2^2;
sbit COL3= P2^3;
sbit CS88= P2^4;
sbit EN	 = P2^5;   //当EN端由高电平跳变成低电平时，液晶模块执行命令。
sbit CS  = P2^6;
sbit RST = P2^7;

sbit RX  = P3^0;
sbit TX  = P3^1;
sbit IR  = P3^2;
sbit SW0 = P3^3;
sbit SW1 = P3^4;
sbit SW2 = P3^5;
sbit B20 = P3^6;
sbit BZ = P3^7;


/*为方便使用，部分管脚的多重定义*/

#define M_0   COL0 
#define M_1   COL1 
#define M_2   COL2 
#define M_3   COL3 

#define CLK SW0
#define DAT SW1

sbit LED0  = P0^0;
sbit LED1  = P0^1;
sbit LED2  = P0^2;
sbit LED3  = P0^3;
sbit LED4  = P0^4;
sbit LED5  = P0^5;
sbit LED6  = P0^6;
sbit LED7  = P0^7;


//系统初始化函数
void top51_init(void);/*top51主板初始化*/


#endif