#include<reg51.h> //包含头文件，这个嘛，就不用多说了～～ 
#define uint unsigned int//预定义一下 
#define uchar unsigned char 
sbit rs=P2^6;//1602的数据/指令选择控制线 
sbit rw=P2^5;//1602的读写控制线 
sbit en=P2^7;//1602的使能控制线 
/*P2口接1602的D0~D7，注意不要接错了顺序，我以前可在这上面吃过亏～*/ 
uchar code table[]="LCD1602 check ok"; //要显示的内容1放入数组tablel 
uchar code table1[]="study up";//要显示的内容2放入数组table1 
void delay(uint n)//延时函数
{ 
uint x,y;  
 for(x=n;x>0;x--) 
for(y=110;y>0;y--); 
} 
void lcd_wcom(uchar com)//1602写命令函数
{ 
 rs=0;//选择指令寄存器
 rw=0;//选择写
 delay(5);
  en=1;//使能线电平变化，命令送入1602的8位数据口
 delay(5); 
 P0=com;//把命令字送入P2 
 delay(5);//延时一小会儿，让1602准备接收数据 
 en=0; 
} 
void lcd_wdat(uchar dat)//1602写数据函数
{ 
 rs=1;//选择数据寄存器 
 rw=0;//选择写
 delay(5);
 en=1;//使能线电平变化，数据送入1602的8位数据口
 delay(5);  
 P0=dat;//把要显示的数据送入P2 
 delay(5);//延时一小会儿，让1602准备接收数据 
 
 en=0; 
} 
void lcd_init()//1602初始化函数 
{ 
 lcd_wcom(0x38); //8位数据，双列，5*7字形
 lcd_wcom(0x0c);//开启显示屏，关光标，光标不闪烁 
 lcd_wcom(0x06); //显示地址递增，即写一个数据后，显示位置右移一位 
 lcd_wcom(0x01); //清屏 
} 
void main()//主函数 
{
 uchar n,m=0; 
 lcd_init();//液晶初始化
 lcd_wcom(0x80);//显示地址设为80H（即00H，）上排第一位 
 for(m=0;m<16;m++)//将table[]中的数据依次写入1602显示 
 { 
lcd_wdat(table[m]);
delay(200); 
 } 
 lcd_wcom(0x80+0x44); //重新设定显示地址为0xc4,即下排第5位 
 for(n=0;n<8;n++)//将table1[]中的数据依次写入1602显示 
 {
lcd_wdat(table1[n]); 
delay(200); 
 } 
 while(1);//动态停机 
} 
