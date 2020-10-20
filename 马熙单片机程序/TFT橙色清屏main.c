#include<reg51.h>
#include"gui.h"
#include"R61505V.h"


void delay(uchar z) // ÑÓ³Ùº¯Êý
{
uchar y;
for(z;z>0;z--)
   for(y=0;y<110;y++);
}

void main(void)
{
    TFT_Init();
   
   delay(20);
   
	TFT_ClearScreen(0x07ff);
		delay(20);
	  {
	  GUI_ShowPicture(0, 0, 176, 220);
  	  GUI_Write32CnChar(100, 100, "ÂíÎõ", 0x001f, 0x07ff);	 
	
	while(1);
		  }
   
}