#include"r61505v.h"

/****************************************************************************
*函数名：TFT_WriteCmd
*输  入：cmd
*输  出：
*功  能：写命令。
****************************************************************************/	  

void TFT_WriteCmd(uint cmd)
{
	uchar cmdH, cmdL;
	
	cmdH = cmd >> 8;
	cmdL = cmd & 0x00FF;  //先进行数据换算，转为8位宽

	TFT_WR = 1;	 		  //初始化WR
	TFT_CS = 0;			  //打开片选
	TFT_RS = 0;			  //选择命令
	TFT_RD = 1;           //选择写

	TFT_DATAPORTH = cmdH; //放置命令数据
	TFT_DATAPORTL = cmdL;

	TFT_WR = 0;			  //写入时序
	TFT_WR = 1;

	TFT_CS = 1;			  //关闭片选
}

/****************************************************************************
*函数名：TFT_WriteData
*输  入：dat
*输  出：
*功  能：写数据。
****************************************************************************/	  

void TFT_WriteData(uint dat)
{
	uchar datH, datL;
	datH = dat >> 8;
	datL = dat & 0x00FF;  //先进行数据换算，转为8位宽

	TFT_WR = 1;	 		  //初始化WR
	TFT_CS = 0;			  //打开片选
	TFT_RS = 1;			  //选择发送数据
	TFT_RD = 1;           //选择写

	TFT_DATAPORTH = datH; //放置命令数据
	TFT_DATAPORTL = datL;

	TFT_WR = 0;			  //写入时序
	TFT_WR = 1;

	TFT_CS = 1;			  //关闭片选
}

/****************************************************************************
*函数名：TFT_WriteCmdData
*输  入：cmd, dat
*输  出：
*功  能：写命令， 写数据。
****************************************************************************/	  

void TFT_WriteCmdData(uint cmd, uint dat)
{
	TFT_WriteCmd(cmd);
	TFT_WriteData(dat);
}

/****************************************************************************
*函数名：TFT_Init
*输  入：
*输  出：
*功  能：初始化TFT。
****************************************************************************/	  

void TFT_Init(void)
{
	uint i;

	TFT_RST = 1;
	for(i=500; i>0; i--);
	TFT_RST = 0;
	for(i=500; i>0; i--);
	TFT_RST = 1;
	for(i=5000; i>0; i--);
	
	TFT_CS = 0;

 	TFT_WriteCmdData(0x0000, 0x0001);
	TFT_WriteCmdData(0x0007, 0x0000);			
	TFT_WriteCmdData(0x0012, 0x0000);       
	TFT_WriteCmdData(0x00A4, 0x0001);

	for(i=5000; i>0; i--);

//--------GAMMA SETTINGS---------
	TFT_WriteCmdData(0x0008, 0x0808); 
	TFT_WriteCmdData(0x0018 ,0x0001);			
	TFT_WriteCmdData(0x0010, 0x11B0);			
	TFT_WriteCmdData(0x0011, 0x0000);
	TFT_WriteCmdData(0x0012, 0x1115);
	TFT_WriteCmdData(0x0013, 0x8B0B);
	TFT_WriteCmdData(0x0012, 0x1135);			
	TFT_WriteCmdData(0x0014, 0x8000);
	TFT_WriteCmdData(0x0001, 0x0100);
	TFT_WriteCmdData(0x0002, 0x0700); 
	TFT_WriteCmdData(0x0003, 0x1030);
	TFT_WriteCmdData(0x0070, 0x1B00);

	TFT_WriteCmdData(0x0071, 0x0001);
	TFT_WriteCmdData(0x0090, 0x0002);           
	TFT_WriteCmdData(0x0091, 0x0000);
	TFT_WriteCmdData(0x0092, 0x0001);
	TFT_WriteCmdData(0x0007, 0x0001);
	for(i=5000; i>0; i--);

	TFT_WriteCmdData(0x0007, 0x0021);
	for(i=5000; i>0; i--);

	TFT_WriteCmdData(0x0012, 0x1135);                  
	TFT_WriteCmdData(0x0007, 0x0233);                 
			
	TFT_WriteCmd(0x0022);
	
}

/****************************************************************************
*函数名：TFT_SetWindow
*输  入：xStart, yStart, xEnd, yEnd
*输  出：
*功  能：设置要操作的窗口。
****************************************************************************/	  

void TFT_SetWindow(uint xStart, uint yStart, uint xEnd, uint yEnd)
{
	

	TFT_WriteCmdData(0x0050, xStart);	 //水平线起始
	TFT_WriteCmdData(0x0052, yStart);	 //垂直线起始
	TFT_WriteCmdData(0x0051, xEnd);	 //水平线结束
	TFT_WriteCmdData(0x0053, yEnd);	 //垂直线结束

	TFT_WriteCmdData(0x0020, xEnd);
	TFT_WriteCmdData(0x0021, yEnd);

	TFT_WriteCmd(0x0022);	
}

/****************************************************************************
*函数名：GUI_Clearcreen
*输  入：backColor
*输  出：
*功  能：清屏并涂上颜色。
****************************************************************************/
	  
void TFT_ClearScreen(uint color)
{
  	uint i, j;

	TFT_SetWindow(0, 0, TFT_XMAX, TFT_YMAX);	 //作用区域
  	for(i=0; i<TFT_XMAX+1; i++)
	{
		for (j=0; j<TFT_YMAX+1; j++)
		{
			TFT_WriteData(color);
		}

	}
}
#include<reg51.h>
#include"gui.h"

void main(void)
{
	TFT_Init();
	TFT_ClearScreen(0x0000);
	while(1)
	{
		GUI_Write32CnChar(20, 24, "普中科技", 0xF800, 0x0000);
		GUI_Line(20, 54, 140, 54, 0xF800);
		GUI_Line(20, 54, 140, 54, 0xF800);
		GUI_WriteASCII(127, 0, "RST", 0xF800, 0x0000);
		GUI_WriteASCII(0, 65, "2013", 0xF800, 0x0000);
		GUI_Write32CnChar(64, 60, "年", 0xF800, 0x0000);
		GUI_WriteASCII(93, 65, "12", 0xF800, 0x0000);
		GUI_Write32CnChar(120, 60, "月", 0xF800, 0x0000);
		GUI_WriteASCII(0, 100, "31", 0xF800, 0x0000);
		GUI_Write32CnChar(32, 95, "日", 0xF800, 0x0000);
		GUI_WriteASCII(0, 135, "12-00-00", 0xF800, 0x0000);
		GUI_Write32CnChar(64, 95, "星期", 0xF800, 0x0000);
		GUI_WriteASCII(128, 100, "2", 0xF800, 0x0000);
		while(1);
	}
}
 #include"gui.h"

//---汉字的字库头文件---//
#include"charcode.h"

//---如果要显示图片就添加这个头文件---//
#ifdef PICTURE_SHOW

#include"picture.h"

#endif

/****************************************************************************
*函数名：GUI_Dot
*输  入：x：点的X坐标；
*      * y：点的Y坐标
*      * color：点的颜色
*输  出：
*功  能：给单个像素涂上颜色。
****************************************************************************/	  

void GUI_Dot(uint x, uint y, uint color)
{  
	uchar i;

	TFT_SetWindow(x-1, y, x+2, y+2);  //单个像素

	for(i=0; i<16; i++)
	{
		TFT_WriteData(color);
	}
}

///****************************************************************************
//*函数名：GUI_Box
//*输  入：sx：起始X坐标, sy：其实Y坐标, 
//*      * ex：终止X坐标, ey：终止Y坐标,
//*      * color：方框的颜色
//*输  出：
//*功  能：给一个区域涂上颜色。
//****************************************************************************/	  
//void GUI_Box(uint sx, uint sy, uchar ex, uint ey, uint color)
//{ 
//	uint temp;   
//	TFT_SetWindow(sx, sy, ex, ey); 
//  sx = ex - sx + 1;
//	sy = ey - sy + 1;
//	while (sx--)
//	{
//	 	temp = sy;
//		while (temp--)
//	 	{	
//			TFT_WriteData(color);
//		}
//	}
//}

/****************************************************************************
*函数名：GUI_Line 
*输  入：xStart：线的起始X坐标,
*      * yStart：线的其实Y坐标,
*      * xEnd：线的终止X坐标,
*      * yEnd：线的终止Y坐标,
*      * color：线条的颜色
*输  出：
*功  能：画一条直线
****************************************************************************/	  

void GUI_Line(uint xStart, uint yStart, uchar xEnd, uint yEnd, uint color)
{
	uint t;  
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;  
	int incx, incy;  
	uint row, col;  
	delta_x = xEnd - xStart;//计算坐标增量  
	delta_y = yEnd - yStart;  
	col = xStart;  
	row = yStart;  
	if (delta_x > 0)
	{
		incx=1;//设置单步方向
	} 	  
	else    
	{  
	    if (delta_x == 0)
		{
			incx = 0;//垂直线 
		} 		 
	    else 
			{
				incx = -1;
				delta_x = -delta_x;
			}  
	}  
	if (delta_y > 0)
	{
		incy = 1;
	}	  
	else  
	{  
	    if (delta_y == 0)
		{
			incy = 0;//水平线  
		} 
	    else 
		{
			incy = -1;
			delta_y = -delta_y;
		}  
	}  
	if (delta_x > delta_y)
	{ 
		distance = delta_x;//选取基本增量坐标轴  
	}
	else
	{
		distance = delta_y; 
	} 	
	for (t=0; t<=distance+1; t++)  
	{                                     //画线输出  
	    GUI_Dot(col, row, color);
	    xerr += delta_x;  
	    yerr += delta_y;  
	  	if(xerr > distance)  
	    {  
	        xerr -= distance;  
	        col += incx;  
	    }  
	    if(yerr > distance)  
	    {  
	        yerr -= distance;  
	        row += incy;  
	    }  
	}  
}

/****************************************************************************
*函数名：GUI_WriteCnChar 
*输  入：x：显示起始X坐标
*      * y：显示起始Y坐标
*      * *cn：要显示的字符串
*      * wordColor：字体的颜色
*      * backColor：背景颜色
*输  出：
*功  能：写二号楷体汉字
****************************************************************************/	  

#ifdef CHAR32_SHOW

void GUI_Write32CnChar(uint x, uint y, uchar *cn, uint wordColor, uint backColor)	 
{  
	uchar i, j, wordNum;
	uint color;
	while (*cn != '\0')
	{
		TFT_SetWindow(x, y, x+31, y+28);
		for (wordNum=0; wordNum<20; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar32x29[wordNum].Index[0]==*cn)
			     &&(CnChar32x29[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<116; i++) 
				{	//MSK的位数
					color=CnChar32x29[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							TFT_WriteData(wordColor);
						} 						
						else
						{
							TFT_WriteData(backColor);
						} 
						color<<=1;
					}//for(j=0;j<8;j++)结束
				}    
			}
		} //for (wordNum=0; wordNum<20; wordNum++)结束 	
		cn += 2;
		x += 32;
	}
}
#endif

/****************************************************************************
*函数名：GUI_WriteEnChar 
*输  入：x：显示起始X坐标
*      * y：显示起始Y坐标
*      * *cn：要显示的字符串
*      * wordColor：字体的颜色
*      * backColor：背景颜色
*输  出：
*功  能：写四号字字母
****************************************************************************/	  
#ifdef CHAR14_SHOW

void GUI_Write14CnChar(uint x,uint y,uchar *cn,uint wordColor,uint backColor)	 
{  
	uchar i, j, wordNum;
	uint color;
	while (*cn != '\0')
	{
		TFT_SetWindow(x, y, x+23, y+18);
		for (wordNum=0; wordNum<20; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar19x24[wordNum].Index[0]==*cn)
			     &&(CnChar19x24[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<57; i++) 
				{	//MSK的位数
					color=CnChar19x24[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							TFT_WriteData(wordColor);
						} 						
						else
						{
							TFT_WriteData(backColor);
						} 
						color<<=1;
					}//for(j=0;j<8;j++)结束
				}    
			}
		} //for (wordNum=0; wordNum<20; wordNum++)结束 	
		cn += 2;
		x += 24;
	}
}

#endif 
/****************************************************************************
*函数名：GUI_WriteASCII 
*输  入：x：显示起始X坐标
*      * y：显示起始Y坐标
*      * *p：要显示的字符串
*      * wordColor：字体的颜色
*      * backColor：背景颜色
*输  出：
*功  能：写16X24的ASCII字符
****************************************************************************/	  

#ifdef USE_ASCII

void GUI_WriteASCII(uint x, uint y, uchar *p, uint wordColor, uint backColor)
{
	uchar j, wordByte,wordNum;
	uint color;
	while(*p != '\0')
	{
		wordNum = *p - 32;
		TFT_SetWindow(x,y,x+15, y+23);
		for (wordByte=0; wordByte<48; wordByte++)
		{
			color = ASCII16x24[wordNum][wordByte];
			for (j=0; j<8; j++) 
			{
				if ((color&0x80) == 0x80)
				{
					TFT_WriteData(wordColor);
				} 						
				else
				{
					TFT_WriteData(backColor);
				} 	
				color <<= 1;
			}
		}
		p++;
		x +=16;
	}
}
#endif

/****************************************************************************
*函数名：GUI_ShowPicture 
*输  入：x：显示起始X坐标
*      * y：显示起始Y坐标
*      * wide：图片的宽度
*      * high：图片的高度
*输  出：
*功  能：画一条直线
****************************************************************************/	  
#ifdef PICTURE_SHOW

void GUI_ShowPicture(uint x, uint y, uchar wide, uint high)
{
	uint temp = 0, tmp = 0, num = 0;
	TFT_SetWindow(x, y, x+wide-1, y+high-1);
	num = wide * high * 2;
	do
	{  
		temp = pic[tmp + 1];
		temp = temp << 8;
		temp = temp | pic[tmp];
		TFT_WriteData(temp);//逐点显示
		tmp += 2;
	}
	while(tmp < num);	
}

#endif















