 /*********************************************************************
 *
 *                           delay.h
 *
 *********************************************************************
 * 描    述: 爱晶电子提供延时函数头文件
 * 开发平台: KEIL C51
 * 公    司: 爱晶电子			
 * 网    址: http://www.ajdz.net
 * 作者			日期			注释
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 王伟民		09/04/01		原始文件
 * 王伟民		09/07/12		v1.1
 * 王伟民		09/10/20		v1.2
 ********************************************************************/

#ifndef   DELAY_H
#define   DELAY_H

void delay1us();
void delay2us();
void delay5us();
void delay10us();
void delay20us();
void delay50us();
void delay100us();
void delay200us();
void delay500us();
void delay1ms();
void delay2ms();
void delay5ms();
void delay10ms();
void delay20ms();
void delay50ms();
void delay100ms();
void delay200ms();
void delay500ms();
void delay1s();
void delay2s();
void delay5s();
void delay10s();


void delay8(unsigned char t);/*1.6us--232us延时函数(fosc=11.0592M时)*/
void delay(unsigned int t);/*2.2us--77ms延时函数(fosc=11.0592M时)*/
void delayms(unsigned char t);/*一个毫秒级可变延时函数*/
void delays(float t);/*一个秒级可变延时函数*/


#endif