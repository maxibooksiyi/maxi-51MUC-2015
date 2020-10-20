#include<reg52.h>
#include<stdlib.h>
#include<math.h>
#define uchar unsigned char
#define uint unsigned int
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uchar code led[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar xx[8];
sbit dula=P2^6;
sbit wela=P2^7;
void displaypro(double h)
{
	uchar point=8,m;
	bit symbol;
	char i;
	double proh;
	symbol=0;
	if(h<0)
	{
		symbol=1;
		h=-h;
	}
	if(h>=0&&h<10){point=1;proh=h*10000000;}
	if(h>=10&&h<100){point=2;proh=h*1000000;}
	if(h>=100&&h<1000){point=3;proh=h*100000;}
	if(h>=1000&&h<10000){point=4;proh=h*10000;}
	if(h>=10000&&h<100000){point=5;proh=h*1000;}
	if(h>=100000&&h<1000000){point=6;proh=h*100;}
	if(h>=1000000&&h<10000000){point=7;proh=h*10;}
	if(h>=10000000&&h<100000000){point=8;proh=h;}
	if(h<100000000)
	{
		for(i=7;i>=0;i--)
		{
			m=proh/pow(10,i);
			xx[7-i]=table[m];
			proh=proh-(m*pow(10,i));
			if(proh<0)
			proh=0;
		}
		if(h>=1)//由于keil 中单精度和双精度是一样的，只能表示六位数，如不把后面两位清零，将会出现乱码。
		{
			xx[6]=0x3f;
			xx[7]=0x3f;
		}
		else 
			xx[7]=0x3f;
		xx[point-1]+=128;//显示小数点
	}
	else //超过八位数时，计算器报错显示E
	{
		for(i=6;i>=0;i--)
		xx[i]=0x00;
		xx[7]=0x79;
	}
	while(xx[7]==0x3f)//去除0.0000000 显示的问题，即把0.0000000 显示成0.
	{
		for(i=7;i>0;i--)
		xx[i]=xx[i-1];
		xx[0]=0x00;
	}
	if(symbol==1)//若为负数时，将数组中的数据后移
	{
		for(i=6;i>=0;i--)
		{
			if(xx[i]==0x00)
			{
				xx[i]=0x40;
				break;
			}
		}
	}
}
void delay(unsigned char x)
{
	unsigned char i,j;
	for(i=0;i<x;i++)
	for(j=0;j<x;j++);
}
unsigned char keyscan()//key 为按键返回值
{
	uchar temp,row=0,col=0,key;
	uint add;
	P2&=0xe8;
	P1|=0x1f;
	temp=P1;
	temp|=0xe0;
	if(temp!=0xff)
	{
		delay(15);
		temp=P1;
		temp|=0xe0;
		if(temp!=0xff)
		{
			row=P1;
			P2|=0x17;
			P1&=0xe0;
			col=P2;
			col|=0xe8;
			add=row*256+col;
			switch(add)
			{
				case 0xfefe: key=0;break;
				case 0xfdfe: key=1;break;
				case 0xfbfe: key=2;break;
				case 0xf7fe: key=3;break;
				case 0xeffe: key=10;break;
				case 0xfefd: key=4;break;
				case 0xfdfd: key=5;break;
				case 0xfbfd: key=6;break;
				case 0xf7fd: key=7;break;
				case 0xeffd: key=11;break;
				case 0xfefb: key=8;break;
				case 0xfdfb: key=9;break;
				case 0xfbfb: key=15;break;
				case 0xf7fb: key=14;break;
				case 0xeffb: key=12;break;
				case 0xfeef: key=16;break;
				case 0xfdef: key=17;break;
				case 0xfbef: key=18;break;
				case 0xf7ef: key=19;break;
				case 0xefef: key=13;break;
				default: key=20;break;
			}
			return key;
		}
		else return 20;
	}
	else return 20;
}
void main()
{
	uchar k,i,hand,h2;
	bit xsd=0;
	char data e;
	double data temp,h1=0,h3=0,h4=0;
	displaypro(0);
	while(1)
	{
		k=keyscan();
		if(k==20)
		hand=0;
		for(i=0;i<8;i++)
		{
			wela=1;
			P0=led[i];
			wela=0;
			P0=0xff;
			dula=1;
			P0=xx[i];
			dula=0;
			delay(20);
			P0=0xff;
		}
		switch(k)
		{
			case 20:break;
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9: if(hand==0)
			{
				h4=0;
				if(h2==0)
				{
					if(xsd==0)
					{
						if(h1>=0)h1=h1*10+k;
						else h1=h1*10-k;
					}
					if(xsd==1)
					{
						e--;
						temp=pow(10,e);
						if(h1>=0)h1=h1+k*temp;
						else h1=h1-k*temp;
					}
					displaypro(h1);
				}
				else
				{
					if(xsd==0)
					{
						if(h3>=0)h3=h3*10+k;
						else h3=h3*10-k;
					}
					if(xsd==1)
					{
						e--;
						temp=pow(10,e);
						if(h3>=0)h3=h3+k*temp;
						else h3=h3-k*temp;
					}
					displaypro(h3);
				}
			}
			hand++;
			break;
			case 10:if(hand==0)//加号
			{
				if(h4!=0)
				{
					h1=h4;
					h4=0;
				}
				switch(h2)
				{
					case 0: h2=1;break;
					case 1: h1=h1+h3;h2=1;h3=0;displaypro(h1);break;
					case 2: h1=h1-h3;h2=1;h3=0;displaypro(h1);break;
					case 3: h1=h1*h3;h2=1;h3=0;displaypro(h1);break;
					case 4: h1=h1/h3;h2=1;h3=0;displaypro(h1);break;
				}
				xsd=0;e=0;
			}
			hand++;
			break;
			case 11:if(hand==0)//减号
			{
				if(h4!=0)
				{
					h1=h4;
					h4=0;
				}
				switch(h2)
				{
					case 0: h2=2;break;
					case 1: h1=h1+h3;h2=2;h3=0;displaypro(h1);break;
					case 2: h1=h1-h3;h2=2;h3=0;displaypro(h1);break;
					case 3: h1=h1*h3;h2=2;h3=0;displaypro(h1);break;
					case 4: h1=h1/h3;h2=2;h3=0;displaypro(h1);break;
				}
				xsd=0;e=0;
			}
			hand++;
			break;
			case 12:if(hand==0)//乘号
			{
				if(h4!=0)
				{
					h1=h4;
					h4=0;
				}
				switch(h2)
				{
					case 0: h2=3;break;
					case 1: h1=h1+h3;h2=3;h3=0;displaypro(h1);break;
					case 2: h1=h1-h3;h2=3;h3=0;displaypro(h1);break;
					case 3: h1=h1*h3;h2=3;h3=0;displaypro(h1);break;
					case 4: h1=h1/h3;h2=3;h3=0;displaypro(h1);break;
				}
				xsd=0;e=0;
			}
			hand++;
			break;
			case 13:if(hand==0)//除号
			{
				if(h4!=0)
				{
					h1=h4;
					h4=0;
				}
				switch(h2)
				{
					case 0: h2=4;break;
					case 1: h1=h1+h3;h2=4;h3=0;displaypro(h1);break;
					case 2: h1=h1-h3;h2=4;h3=0;displaypro(h1);break;
					case 3: h1=h1*h3;h2=4;h3=0;displaypro(h1);break;
					case 4: h1=h1/h3;h2=4;h3=0;displaypro(h1);break;
				}
				xsd=0;e=0;
			}
			hand++;
			break;
			case 14:if(hand==0)//负号或正号
			{
				if(h2==0)
				{
					h1=-h1;
					displaypro(h1);
				}
				else
				{
					h3=-h3;
					displaypro(h3);
				}
			}
			hand++;
			break;
			case 15:if(hand==0)//开方
			{
				if(h4!=0)
				{
					h1=h4;
					h4=0;
				}
				if(h2==0)
				{
					h1=sqrt(h1);
					displaypro(h1);
				}
				else
				if(h3==0)
				{
					h1=sqrt(h1);
					displaypro(h1);
					h2=0;
				}
				else
				{
					h3=sqrt(h3);
					displaypro(h3);
				}
					xsd=0;e=0;
			}
			hand++;
			break;
			case 16:if(hand==0)//小数点
			{
				xsd=1;
			}
			hand++;
			break;
			case 17:if(hand==0)// 1/x
			{
				if(h4!=0)
				{
					h1=h4;
					h4=0;
				}
				if(h2==0)
				{
					h1=1/h1;
					displaypro(h1);
				}
				else
				if(h3==0)
				{
					h1=1/h1;
					displaypro(h1);
					h2=0;
				}
				else
				{
					h3=1/h3;
					displaypro(h3);
				}
				xsd=0;e=0;
			}
			hand++;
			break;
			case 18:if(hand==0)//全部清除
			{
				h1=0;
				h2=0;
				h3=0;
				xsd=0;
				e=0;
				displaypro(0);
			}
			hand++;
			break;
			case 19:if(hand==0)//等于号
			{
				switch(h2)
				{
					case 0: break;
					case 1: h4=h1+h3;h1=0;h2=0;h3=0;displaypro(h4);break;
					case 2: h4=h1-h3;h1=0;h2=0;h3=0;displaypro(h4);break;
					case 3: h4=h1*h3;h1=0;h2=0;h3=0;displaypro(h4);break;
					case 4: h4=h1/h3;h1=0;h2=0;h3=0;displaypro(h4);break;
				}
				xsd=0;e=0;
			}
		}
	}
}