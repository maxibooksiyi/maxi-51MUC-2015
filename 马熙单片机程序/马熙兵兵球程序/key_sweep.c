/***************************************************************/
/*底层驱动名称:   4*4按键翻转式扫描驱动  
/*入 口  参 数：  无  宏定义：Key_Port P2      
/*出 口  参 数：  按键值(十进制）               
/*作        者：  先永     完成时间：2009、2、20	  
/***************************************************************/

#include "stc.h"/*包含STC单片机头文件*/
#include "key_sweep.h"

unsigned char Key_Value=0;
unsigned char Key_flag =0;

//以下为键码							   
unsigned char code Key_code[]=
{
	//p0    p1   p2  p3
	0xee,0xed,0xeb,0xe7,   //p4
	0xde,0xdd,0xdb,0xd7,  //p5
	0xbe,0xbd,0xbb,0xb7,  //p6
	0x7e,0x7d,0x7b,0x77  //p7
};


void Key_Scan(void)
{
	unsigned char 	KeyV_l,KeyV_h,KeyV,i;
			  	
//	if(Key_Port!=0X0f)		  //0000 1111  有按键的时候就可能是0000 1110；
//	{
		//Delay();
		if(Key_Port!=0X0f)		  //0000 1111  有按键的时候就可能是0000 1110；
		{
			KeyV_l=Key_Port&0x0f; //取出键码的 低4位
			Key_Port=0XF0;
			KeyV_h=Key_Port&0xf0; //取出键码的 高4位
			KeyV=KeyV_h+KeyV_l;	  //合并键码
	
			/*************************************
		    按键编码         对应         键值
			Ee   ed   eb  e7		 1	5  9   13
			De   dd   db  d7		 2	6  10  14
			Be   bd   bb  b7		 3	7  11  15
			7e   7d   7b  77		 4	8  12  16
			*************************************/
			for(i=0;i<16;i++)   	//利用键码查找键值
			{
				if(Key_code[i]==KeyV)
				Key_Value=(i+1);
			}
			Key_Port=0X0f;
		}
//	}

	Key_flag =1;
}