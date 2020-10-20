#include<reg51.h>   //头文件
#define uint unsigned int // 
#define uchar unsigned char

sbit lcden=P2^0; //定义引脚
sbit rs=P2^3;
sbit rw=P2^4;
sbit busy=P0^7;

char i,j,temp,num,num_1;
long a,b,c;     //a,第一个数 b,第二个数 c,得数
float a_c,b_c;
uchar flag,fuhao;//flag表示是否有符号键按下，fuhao表征按下的是哪个符号

uchar code table[]={
7,8,9,0,
4,5,6,0,
1,2,3,0,
0,0,0,0};
uchar code table1[]={
7,8,9,0x2f-0x30,
4,5,6,0x2a-0x30,
1,2,3,0x2d-0x30,
0x01-0x30,0,0x3d-0x30,0x2b-0x30};

void delay(uchar z) // 延迟函数
{
uchar y;
for(z;z>0;z--)
   for(y=0;y<110;y++);
}

void check() // 判断忙或空闲
{
do{
    P0=0xFF;
    rs=0;     //指令
    rw=1;     //读
    lcden=0;     //禁止读写
    delay(1); //等待，液晶显示器处理数据
    lcden=1;     //允许读写
    }while(busy==1); //判断是否为空闲，1为忙，0为空闲
}
void write_com(uchar com) // 写指令函数
{
P0=com;    //com指令付给P0口
rs=0;
rw=0;
lcden=0;
check();
lcden=1;
}

void write_date(uchar date) // 写数据函数
{
    P0=date;
rs=1;
rw=0;
lcden=0;
check();
lcden=1;
}

void init() //初始化
{
    num=-1;
lcden=1; //使能信号为高电平
write_com(0x38); //8位，2行
write_com(0x0c); //显示开，光标关，不闪烁*/
write_com(0x06); //增量方式不移位 显竟獗暌贫 柚?
write_com(0x80); //检测忙信号
write_com(0x01); //显示开，光标关，不闪烁
num_1=0;
i=0;
j=0;
a=0;     //第一个参与运算的数
b=0;     //第二个参与运算的数
c=0;
flag=0; //flag表示是否有符号键按下， 
fuhao=0; // fuhao表征按下的是哪个符号
}
void keyscan() // 键盘扫描程序
{

P3=0xfe; 
if(P3!=0xfe)
{
   delay(20); // 延迟20ms
   if(P3!=0xfe)
   {
    temp=P3&0xf0;
    switch(temp)
    {
     case 0xe0:num=0;
       break;
     case 0xd0:num=1;
       break; 
     case 0xb0:num=2;
       break;
     case 0x70:num=3;
       break;
    }
   }
   while(P3!=0xfe);
   if(num==0||num==1||num==2)//如果按下的是'7','8'或'9
   { 
       if(j!=0)
        { 
          write_com(0x01);
          j=0;
         }
       if(flag==0)//没有按过符号键
    {
     a=a*10+table[num]; 
    }
    else//如果按过符号键
    {
     b=b*10+table[num]; 
    }
   }
   else//如果按下的是'/'
   {
    flag=1;
    fuhao=4;//4表示除号已按
   }
   i=table1[num];
   write_date(0x30+i);
}

P3=0xfd;
if(P3!=0xfd)
{
   delay(5);
   if(P3!=0xfd)
   {
    temp=P3&0xf0;
    switch(temp)
    {
     case 0xe0:num=4;
         break;
      
     case 0xd0:num=5;
         break;
      
     case 0xb0:num=6;
         break;
      
     case 0x70:num=7;
         break;
    }
   }
   while(P3!=0xfd);
   if(num==4||num==5||num==6&&num!=7)//如果按下的是'4','5'或'6'
   { 
    if(j!=0)
        { 
          write_com(0x01);
          j=0;
         }
       if(flag==0)//没有按过符号键
    {
     a=a*10+table[num]; 
    }
    else//如果按过符号键
    {
     b=b*10+table[num]; 
    }
   }
   else//如果按下的是'/'
   {
    flag=1;
    fuhao=3;//3表示乘号已按
   }
   i=table1[num];
   write_date(0x30+i);
}

P3=0xfb;
if(P3!=0xfb)
{
   delay(5);
   if(P3!=0xfb)
   {
    temp=P3&0xf0;
    switch(temp)
    {
     case 0xe0:num=8;
         break;
      
     case 0xd0:num=9;
         break;
      
     case 0xb0:num=10;
         break;
      
     case 0x70:num=11;
         break;
    }
   }
   while(P3!=0xfb);
   if(num==8||num==9||num==10)//如果按下的是'1','2'或'3'
   { 
    if(j!=0)
        { 
          write_com(0x01);
          j=0;
         }
      if(flag==0)//没有按过符号键
    {
     a=a*10+table[num]; 
    }
    else//如果按过符号键
    {
     b=b*10+table[num]; 
    }
   }
   else if(num==11)//如果按下的是'-'
   {
    flag=1;
    fuhao=2;//2表示减号已按
   }
   i=table1[num];
   write_date(0x30+i);
}

P3=0xf7;
if(P3!=0xf7)
{
   delay(5);
   if(P3!=0xf7)
   {
    temp=P3&0xf0;
    switch(temp)
    {
     case 0xe0:num=12;
         break;
      
     case 0xd0:num=13;
         break;
      
     case 0xb0:num=14;
         break;
      
     case 0x70:num=15;
         break;
    }
   }
   while(P3!=0xf7);
   switch(num)
   {
    case 12:{write_com(0x01);a=0;b=0;flag=0;fuhao=0;}//按下的是"清零"
     break;
    case 13:{                //按下的是"0"
       if(flag==0)//没有按过符号键
       {
        a=a*10;
        write_date(0x30);
        P1=0;
       }
       else if(flag==1)//如果按过符号键
       {
        b=b*10;
        write_date(0x30);
        }
      }
     break;
    case 14:{j=1;
           if(fuhao==1){write_com(0x80+0x4f);//按下等于键，光标前进至第二行最后一个显示处
           write_com(0x04);     //设置从后住前写数据，每写完一个数据，光标后退一格
           c=a+b;
           while(c!=0)
           { 
             write_date(0x30+c%10);
             c=c/10;
           } 
           write_date(0x3d);     //再写"="
           a=0;b=0;flag=0;fuhao=0;
           }
      else if(fuhao==2){write_com(0x80+0x4f);//光标前进至第二行最后一个显示处
            write_com(0x04);     //设置从后住前写数据，每写完一个数据，光标后退一格(这个照理说顺序不对，可显示和上段一样)
           if(a-b>0)
             c=a-b;
           else 
             c=b-a;
           while(c!=0)
           { 
             write_date(0x30+c%10);
             c=c/10;
           } 
           if(a-b<0)
             write_date(0x2d);
           write_date(0x3d);     //再写"="           
           a=0;b=0;flag=0;fuhao=0;
          }
      else if(fuhao==3){write_com(0x80+0x4f);
            write_com(0x04);
             c=a*b;
             while(c!=0)
             { 
             write_date(0x30+c%10);
             c=c/10;
             } 
             write_date(0x3d);     
          a=0;b=0;flag=0;fuhao=0;
             }
      else if(fuhao==4){write_com(0x80+0x4f);
            write_com(0x04);
            i=0;
            c=(long)(((float)a/b)*1000);
            while(c!=0)
             { 
               write_date(0x30+c%10);
               c=c/10;
           i++;
           if(i==3)
               write_date(0x2e);
             } 
            if(a/b<=0)
              write_date(0x30);
            write_date(0x3d);                              
            a=0;b=0;flag=0;fuhao=0;
                    }
      }
     break;
     case 15:{write_date(0x30+table1[num]);flag=1;fuhao=1;}
     break;
   }
}
}

main()
{
init();
while(1)
{
   keyscan();
}
}
