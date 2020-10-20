#ifndef ILI9325_H
#define ILI9325_H
//============================================================
//根据芯片资料定义
#define WINDOW_XADDR_START	0x0050 // Horizontal Start Address Set
#define WINDOW_XADDR_END	0x0051 // Horizontal End Address Set
#define WINDOW_YADDR_START	0x0052 // Vertical Start Address Set
#define WINDOW_YADDR_END	0x0053 // Vertical End Address Set
#define GRAM_XADDR		0x0020 // GRAM Horizontal Address Set
#define GRAM_YADDR		0x0021 // GRAM Vertical Address Set
#define GRAMWR 			0x0022 // memory write
#define GUI_LCM_YMAX 219
#define GUI_LCM_XMAX 175

//=============================================================
//定义TFT彩屏模块接口
sbit	  TFT_RST = P3^3;  	//复位
sbit	  TFT_CS  = P2^7;	//片选
sbit	  TFT_RS  = P2^6;	//数据命令选择端
sbit	  TFT_WR  = P2^5;	//读写控制
sbit      TFT_RD  = P3^2;   

#define   TFT_DATAPORTH  P1 //放置高8位数据
#define   TFT_DATAPORTL  P0 //放置低8位数据



//=============================================================
//函数声明
void ILI9325_initial(void);//TFT初始化
void write_cmd(unsigned char CH,unsigned char CL);//写命令
void write_data(unsigned char DH,unsigned char DL);//写数据
void write_data_u16(unsigned int dat);//写16位数据
void write_cmd_data (unsigned char cmd,unsigned int dat);//写命令数据
void LCD_setwindow(unsigned char x0,unsigned int y0,unsigned char x1,unsigned int y1);//设置窗口
void LCD_setxy(unsigned char x,unsigned int y);//设定当前显示坐标




#endif











