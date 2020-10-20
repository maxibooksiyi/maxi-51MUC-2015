#include "reg52.h"
#include <stdlib.h>        //包含atof(),字符串->实数
#include <stdio.h>         //包含sprintf(),实数->字符串
#include <string.h>        //包含strcpy(),字符串复制
#include <math.h>		   //数学函数头文件
#define uchar unsigned char
#define uint unsigned int
uchar num;
/*************lcd****************/
sbit lcdrs=P2^6; //LCD控制脚
sbit lcdrw=P2^5;
sbit lcden=P2^7;
bit  form;            //lcd显示方式切换
uchar idata state;         //lcd状态字存储
/************计算器*************/
sbit reset=P2^0;           //计算器复位
sbit back=P2^1;            //撤销上次键入键
sbit save=P2^2;            //存储键，存储有上次的结果和另外某次计算值
bit eqsign;          //按等号标志
bit press;           //键按标志
bit savesign;        //可存储结果标志
bit sc;			   //复用标志
uchar idata process[30],proc; //记忆计算过程，以便错误恢复和撤销输入
uchar idata continu=0;        //连续计算运算符号存储
uchar idata pai[2][2];        //π 前有数据标志
uchar idata ferror;        //计算检错标志
uchar idata ywei;		   //屏幕移位
uchar idata count=0;       //输入计数
uchar idata count_num=0;   //组号计数
uchar idata result[15];    //计算结果立存，save
uchar idata saveresult[15];//存储结果数组, sc+save
uchar idata jieguo[15];    //结果字符串
uchar idata bdate[2][14];  //待计算字符串二维组
uchar idata on_symbol;     //运算符号
uchar idata fsym[2];       //函数前符号
uchar idata ssym[2];       //存储组前符号
uchar idata bfun[2];       //计算值调用函数选择
uchar idata futojiao[2];   //幅度 to 角度变换
double idata date[2];      //计算值双精度变量组
double idata resultdate;   //双精度结果值
sbit sfyong=P2^3;           //计算器复用键
/******************函数声明**********************/

/***************延时******************/
void delay(uchar z);
/*************lcd写命令***************/
void write_com(uchar com);
/*************lcd写数据***************/
void write_date(uchar date);
/*************lcd读状态***************/
void read_date(void);
/*************lcd写字符串*************/
void write_str(uchar *str);
/************液晶初始化***************/
void init(void );

/**********移屏*******************/
void write();
/**********复用提示开关***************/
void shift(void);
/**************键盘扫描***************/
uchar keyscan(void);
/*************计算器复位**************/
void fuwei();
/************error处理****************/
void callerror(void);
/***************撤销键入**************/
void huifu(void);
/**********函数组前符号处理***********/
uchar funqian(void);
/************运算符预处理*************/
void  cullars(uchar);
/***********输出存储数据预处理********/
char  memory(void);
/***********按键功能主处理************/
void process_date(uchar press_date);
/***********按键功能子处理************/
void calculator(uchar press_date);
/************************************************/
void write()
{
   for(num=0;num<16;num++)
      {
       write_com(0x18);
       delay(200);
      }
}
/*************LCD程序****************/
/**************延时******************/
void delay(uchar z)			   
{
      uchar  x,y;
      for(x=z;x>0;x--)
      for(y=100;y>0;y--);
}

/**************写命令*****************/
void write_com(uchar com)
{
      lcdrs=0;
      lcdrw=0;
      lcden=0;
      P0=com;
      delay(5);
      lcden=1;
      delay(5);
      lcden=0;
}
/*************写数据******************/
void write_date(uchar date)
{
      lcdrs=1;
      lcdrw=0;
      lcden=0;
      P0=date;
      delay(5);
      lcden=1;
      delay(5);
      lcden=0;
}
/*************读AC 地址*****************/
void read_date(void)
{
      lcdrs=0;
      lcdrw=1;
	  delay(5);
	  P0=0xff;
      lcden=1;
      delay(5);
      state=P0;
      delay(5);
      lcden=0;delay(5);
	  state=state&127;
}
/*************写字符串******************/
void write_str(uchar *str)
{
      uchar idata i;
      for(i=0;str[i]!='\0';i++)
         write_date(str[i]);
}
/***********液晶初始化***************/
void init(void )
{
      write_com(0x38);       //模式：8位数据，两行，5*7字体
      write_com(0x0c);       //开显示，无光标
      write_com(0x06);       //向左增量移动
      write_com(0x01);       //清屏
}

/**********复用提示开关***************/
void shift(void)
{
   if(sc==1)
   {
   	  read_date();			//读状态
      write_com(0xc0);	    //显示复用提示，左下角's'
	  write_date('s');
	  write_com(state+0x80);//光标还回原来位置
   }
  else
  {
      read_date();
      write_com(0xc0);	   //关闭复用提示
	  write_date(' ');
	  write_com(state+0x80);
  }
  return;
}

/**************键盘扫描*************/
uchar keyscan(void)          //按行扫描，有键按下则返回键符号，否则返回null
{     uchar idata key0,key1,keyment=0;
      P3=0XFe;					   //行1
      key0=P3;
      key0=key0&0xf0;
      key1=key0;
      if(key0!=0xf0)
      {
           delay(5);               //键抖动处理
           P3=0XFe;
           key0=P3;
           key0=key0&0xf0;
           if(key0==key1)
           {
                switch(key0)
                {
                  case 0xe0:keyment='7';break;
                  case 0xd0:keyment='8';break;
                  case 0xb0:keyment='9';break;
                  case 0x70:keyment=0xfd;break;  //0xfd除号在1602液晶中的代码
                }
                while(key0!=0xf0)  //键释放处理
                {
                  key0=P3;
                  key0=key0&0xf0;
                }
                press=1;		   //键按标志置1
                return(keyment);
           }
      }
      P3=0XFd;					  //行2
      key0=P3;
      key0=key0&0xf0;
      key1=key0;
      if(key0!=0xf0)
      {
           delay(5);
           P3=0XFd;
           key0=P3;
           key0=key0&0xf0;
           if(key0==key1)
           {
               switch(key0)
               {
                  case 0xe0:keyment='4';break;
                  case 0xd0:keyment='5';break;
                  case 0xb0:keyment='6';break;
                  case 0x70:keyment='*';break;
               }
               while(key0!=0xf0)
               {
                  key0=P3;
                  key0=key0&0xf0;
               }
               press=1;
               return(keyment);
           }
      }
      P3=0XFb;						 //行3
      key0=P3;
      key0=key0&0xf0;
      key1=key0;
      if(key0!=0xf0)
      {
           delay(5);
           P3=0XFb;
           key0=P3;
           key0=key0&0xf0;
           if(key0==key1)
           {
               switch(key0)
               {
                   case 0xe0:keyment='1';break;
                   case 0xd0:keyment='2';break;
                   case 0xb0:keyment='3';break;
                   case 0x70:keyment='-';break;
               }
               while(key0!=0xf0)
               {
                   key0=P3;
                   key0=key0&0xf0;
               }
               press=1;
               return(keyment) ;
           }
      }
      P3=0XF7;					  //行4
      key0=P3;
      key0=key0&0xf0;
      key1=key0;
      if(key0!=0xf0)
      {
           delay(5);
           P3=0XF7;
           key0=P3;
           key0=key0&0xf0;
           if(key0==key1)
           {
               switch(key0)
               {
                 case 0xe0:keyment='.';break;
                 case 0xd0:keyment='0';break;
                 case 0xb0:keyment='=';break;
                 case 0x70:keyment='+';break;
               }
               while(key0!=0xf0)
               {
                 key0=P3;
                 key0=key0&0xf0;
               }
               press=1;
               return(keyment);
           }
     }
     if(reset==0)          //复位键
     {
           keyment=' ';
           press=1;
           while(reset==0);//键释放
           return(keyment);
	 }
     else if(back==0)	       //撤销前次输入
     {
		   keyment='c';
           press=1;
           while(back==0);
           shift();
           return(keyment);
	 }
     else  if(save==0)		   //储存数据键
     {	delay(5);
	    if(save==0)
	    { if(sc==0)	   //输出前次计算结果键
	       {
		      keyment='m';
              press=1;
              while(save==0);
              return(keyment);
		   }
		   else			   //更新存储的数据或输出存储的数据
		   {
              keyment='n';
              press=1;
			  sc=0;
			  shift();
              while(save==0);
              return(keyment);
		   }
		}
     }
	 else if(sfyong==0)		   //计算器时为复用功能键
     {	 delay(5);
	     if(sfyong==0)
	 	 {  keyment='h';
            press=1;
            while(sfyong==0);
            return(keyment);
		 }return(0);
     }else return(0);	   //无键按下返回null
}

/**************计算器复位****************/
void fuwei()
{   uchar idata j,i;
    write_com(0x0f);	    //lcd设置
    write_com(0x01);
    write_com(0x80);
    resultdate=0;
    for(i=0;i<2;i++)
	{ date[i]=0;bfun[i]=0;pai[0][i]=0;pai[1][i]=0;fsym[i]=0;futojiao[i]=0; //各标志置0
	  for(j=0;j<14;j++)		//字符数组初始化
      {
        bdate[i][j]='\0';
      }
	}
    count=0;				//各标志置0
    count_num=0;
	proc=0;
    ferror=0;
    eqsign=0;
	sc=0;
	savesign=0;
	ywei=0;
	press=0;
	on_symbol=0;
}
/**************error处理******************/
void callerror(void)
{   uchar idata i;
    write_com(0x01);
    write_com(0x80);
    write_str("     error!     ");
    for(i=0;i<200;i++)delay(25); //持续显示 1s
}
/*************函数组前符号处理************/
uchar funqian(void)	   //输入函数前检查是否错误
{  if((bdate[ferror][0]=='+'||bdate[ferror][0]=='-')&&count==1)
   {
   		fsym[ferror]=bdate[ferror][0];
   		bdate[ferror][0]='\0';
		bdate[ferror][1]='\0';
		count=0;
   }
   if(bdate[ferror][0]!='\0'||bfun[ferror]!=0)//错误输入方式处理
   {    callerror();
        huifu();
		return(1);
   }
   return(0);
}
/****************撤销上次输入****************/
void huifu(void)
{
     uchar j,pro;	   //pro变量用于恢复proc
     if(proc!=0)proc--;
	 pro=proc;
	 fuwei();
	 proc=pro;
	 for(j=0;j<proc;j++)calculator(process[j]);	//依次恢复之前的操作
	 return;
}
/*****************运算符预处理*****************/
void  cullars(uchar csym)
{	read_date();		   //看第二组是否有输入
	ferror++;              //检错标志+1
	continu=csym;		   //存储运算符号
	if(ferror==2&&state<0x43){callerror();huifu();return;}// 第二组无输入且ferror=2，错误
	if(ferror==2&&state>0x42)	//第二组有输入且ferror=2，则进行连续计算
	{
	   press=1;					//模拟按键‘=’和‘M+’；将本次的计算结果作为下一次计算的第一组
	   process_date('=');
	   press=1;
	   process_date('m');
	   ferror=1;				//恢复现场，避免变化
	   if(continu=='^'||continu=='%'){process[proc]='h';
	        proc++;process[proc]=continu;proc++;}
	   else{process[proc]=continu;proc++;}	  //恢复连续计算过程
	}
	if(continu=='%'){write_str(" f");}else write_date(' ');
	write_date(continu);//将用与连续计算的运算符号取出
	ywei=0;
	savesign=0;
	count_num=1;     //组标志加1
	count=0;		 //输入计数清零
	on_symbol=continu;  //运算符号调用
	write_com(0xc2);
	return;
}
/*************输出存储数据预处理************/
char  memory(void)
{	if((bdate[count_num][0]=='+'||bdate[count_num][0]=='-')&&count==1)//符号处理
    {
	 	ssym[count_num]=bdate[count_num][0];
	 	bdate[count_num][0]='\0';
		bdate[count_num][1]='\0';
		count=0;
	}
    if(bdate[count_num][0]!='\0'){callerror();huifu();return(1);}	//错误输入方式处理
	return(0);
}
/**************按键功能主处理****************/
void process_date(uchar press_date)
{
     if(form==0){write_com(0x0f);form=1;}   //显示方式切换 光标闪烁
     if(eqsign==1&&press==1)fuwei();		//复位再次计算，记忆过程清零
	 press=0;
	 if(press_date=='c'){huifu();press_date=0;}
	 if(press_date==0)return;         	//无按键
	 process[proc]=press_date;
	 proc++;
	 calculator(press_date);
	 return;
}
/***************按键功能子处理***************/
void calculator(uchar press_date)
{
     uchar idata j=0;
	 /*************************************/
	 if(press_date==('h'))	   //复用键
	 {
	     sc=~sc;		   //按键功能切换，sc为1时调用函数
		 shift();		   //显示与关闭复用提示
	 }
     /*************************************/
     else if((press_date<='9'&&press_date>='0')||(press_date=='.')) //数字键处理
     {	savesign=0;			  //不可更新存储数据
        if(sc==0)			  //是否为复用sc为0，是数字
        {
           if(count<14-ywei)  //未调用函数时最多输入14位数，包括小数点
           {
			       write_date(press_date);
                   bdate[count_num][count]=press_date; //储存键值
                   count++;
           }
           else {callerror();huifu();}  //超出14位出错,恢复

        }
        else switch(press_date)	  //复用时
        {             //	    排错			置函数标志符号	复用清零	 移位，此时数据只能(14-ywei)位
           case '7': j=funqian();if(j==1)return;bfun[ferror]='s';sc=0;shift();ywei=4;write_str("sin ");break; //函数sin
           case '8': j=funqian();if(j==1)return;bfun[ferror]='c';sc=0;shift();ywei=4;write_str("cos ");break; //函数cos
           case '9': j=funqian();if(j==1)return;bfun[ferror]='t';sc=0;shift();ywei=4;write_str("tan ");break; //函数tan
		   case '4': j=funqian();if(j==1)return;bfun[ferror]='e';sc=0;shift();ywei=4;write_str("exp ");break; //函数exp
		   case '5': j=funqian();if(j==1)return;bfun[ferror]='l';sc=0;shift();ywei=3;write_str("ln ");break;  //函数ln
		   case '6': j=funqian();if(j==1)return;bfun[ferror]='g';sc=0;shift();ywei=3;write_str("Lg ");break;  //函数Lg
		   case '1': j=funqian();if(j==1)return;bfun[ferror]='q';sc=0;shift();ywei=5;write_str("sqrt ");break;//函数sqrt
		   case '0': j=funqian();if(j==1)return;bfun[ferror]='d';sc=0;shift();ywei=4;write_str("dao ");break; //函数dao(倒数)
		   case '.': if(count==0){ pai[count_num][0]=1;count=14-ywei; }		 //圆周率 pai处理	   π前面无加权
					 else{ pai[count_num][1]=1;count=14;}   //  π前面可以有加权，后面不能跟数据
					 write_date(247);sc=0;shift();break;
		  /************************2和3是双参数函数 当运算符使用*************************/
		   case '2': 	//x^y x的y次方
					 cullars('^');		 //调用运算符号处理程序
					 sc=0;shift();
					 break;
		   case '3':	//A/B的余数（双精度）
					 cullars('%');		 //调用运算符号处理程序
					 sc=0;shift();
					 break;
         }
      }
       /*************************************************/
      else if(press_date=='m')       //上次计算结果输出
      {
           if(memory()==1)return;						  //输出存储数据预处理
           for(j=0;result[j]!='\0';j++)write_date(result[j]);	        //输出存储值
           strcpy(bdate[count_num],result);
		   savesign=1;		  //此时可以更新saveresult中存储的数据
		   count=14;          //count=14是让本组数据已确定
	  }
      /*************************************************/
      else if(press_date=='n')       //存储的数据更新或输出
      {   if(savesign==1)
          {  strcpy(saveresult,result);
             savesign=0;						 //标志重新置零
             write_com(0x01);
             write_com(0x80);
             write_str("    success!    ");		 //存储成功提示
             for(j=0;j<200;j++)delay(25);
             fuwei();
          }
          else
          {
		     if(memory()==1)return;						 //输出存储数据预处理
             for(j=0;saveresult[j]!='\0';j++)write_date(saveresult[j]);	  //输出存储值
             strcpy(bdate[count_num],saveresult);
			 count=14;
          }
      }
      /****************'+','-'处理*****************/
       else if((press_date=='+')||(press_date=='-'))
       {
		      if(count==0)      // 表示输入数据的符号，此时默认数值为0
              {
                write_date(press_date);
                bdate[count_num][0]=press_date;
				bdate[count_num][1]='0';
				read_date();			//读状态
	            write_date('0');
	            write_com(state);//光标还回原来位置
                count=1;
              }
              else			   
              {
			    cullars(press_date);//为运算符号
              }
       }
       /****************'*','/'处理*****************/
       else if(press_date==0xfd) //0xfd除号在1602液晶中的代码
       {
	       if(sc==1)	   //幅度 to 角度
		   {  if(count!=0)
		      {futojiao[count_num]=1;sc=0;shift();write_date(0xDF);count=14;}
			  else {callerror();huifu();}
		   }
     	   else cullars(press_date);
       }
	   else if(press_date=='*') //0xfd除号在1602液晶中的代码
       {
     	    cullars(press_date);
       }
       /*******************'='号处理*****************/
       else if(press_date=='=')
       {
            write_com(0x01);           //清屏
            write_com(0x80);
            write_date(press_date);    //显示等号
		    for(j=0;j<2;j++)		   //两运算组的函数处理
			{
			   	date[j]=atof(bdate[j]);  //俩组数据，字符串->实数
			   	if(pai[j][0]==1)
				    {  date[j]=3.1415926535897932384626433832795;}
				if(pai[j][1]==1)
			      	{  date[j]=date[j]*3.1415926535897932384626433832795;}//π前缀加权计算
			   	if(ssym[j]=='-')
			   	  	{  date[j]=-date[j];   }	  //数据前符号
				if(futojiao[j]==1)
				    {   date[j]=date[j]*3.1415926535897932384626433832795/180;}	 //幅度是否转角度
            	if(bfun[j]!=0)					  //是否有调用函数，有则先函数运算
            	{ 	switch(bfun[j])
              		{
					   	case 's': date[j]=sin(date[j]);break;	   //函数sin
               			case 'c': date[j]=cos(date[j]);break;	   //函数cos
               			case 't': date[j]=tan(date[j]);break;	   //函数tan
			   			case 'e': date[j]=exp(date[j]);break;	   //函数exp
			   			case 'l': date[j]=log(date[j]);break;	   //函数ln
			   			case 'g': date[j]=log10(date[j]);break;	   //函数Lg
			   			case 'q': date[j]=sqrt(date[j]);break;	   //函数sqrt
			   			case 'd': date[j]=1/date[j];break;		   //函数dao(倒数)
             		}
            	}
				if(fsym[j]=='-')date[j]=-date[j];	 //函数前的符号处理
			}
			if(ferror==0)							 //仅仅函数计算
			   {  resultdate=date[0];}
            else									 //运算符号计算
			{
			   switch(on_symbol)
               {
                  case '+':resultdate=date[0]+date[1];break;
                  case '-':resultdate=date[0]-date[1];break;
                  case '*':resultdate=date[0]*date[1];break;
                  case 0xfd:resultdate=date[0]/date[1];break;       //0xfd除号在1602液晶中的代码
				  case '^':resultdate=pow(date[0],date[1]);break;	//pow()函数	x^y
				  case '%':resultdate=fmod(date[0],date[1]);break;  //fmod()函数 求余数（双精度）
               }
			}
            sprintf(jieguo,"%g",resultdate);//结果实数->字符串,采用格式字符g,系统选择%f或%e输出格式，六位有效数字
	        if((jieguo[0]>'9'||jieguo[0]<'0')&&(jieguo[1]>'9'||jieguo[1]<'0')&&jieguo[1]!='\0'&&jieguo[1]!='.')
			 {   callerror();fuwei();return;  }	 //错误处理，当组1或组2不合法时结果是几个英文字母提示
			else strcpy(result,jieguo);   //没错误，保存结果，结果立存 M+键
            for(j=0;jieguo[j]!='\0';j++)               //显示结果
                 write_date(jieguo[j]);
			for(;j<15;j++)write_date(' ');	 //让光标出屏幕
			eqsign=1;						 //按等号键标志置1
	   }
	   /*******************复位*****************/
       else if(press_date==' ')
       {
            fuwei();
       }
	   return;
}
/*********************主程序**********************/
void main()
{    
      fuwei();		//计算器初始化
      init();       //显示初始化
	  form=1;		//显示方式初始化
	  proc=0;		//计算工程存储初始化
      write_com(0x80+0x10);						
      write_str("Name:Maxi");			//开机画面第一行
      write_com(0x80+0x50);					//指针设置
      write_str("SN:201212268031");			//开机画面第二行
      write();
      while(1)      
      {
	  process_date(keyscan()); //计算器
      }
}
/*********************end**********************/

