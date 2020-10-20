#ifndef __SNAKE_H__
#define __SNAKE_H__

#define uchar unsigned char
#define uint  unsigned int

#define SNAKE_MAX_LONG  20	//定义蛇的最大长度

extern unsigned char Over;
extern struct 
{
	uchar X[SNAKE_MAX_LONG];
	uchar Y[SNAKE_MAX_LONG];
	uchar Long;  			//蛇的长度
	uchar Life; 			//蛇的生命
	uchar Score;			//蛇的分数
	uchar Level;			//蛇的等级              
}Snake;	//定义蛇结构体

void Init_time0 (void);
void Init_SNAKE(void) ;
void Run_SNAKE(void)  ;

#endif