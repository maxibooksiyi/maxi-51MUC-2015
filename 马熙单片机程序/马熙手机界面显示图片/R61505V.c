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
