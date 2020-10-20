#include<reg51.h>
#include"gui.h"

void main(void)
{
	TFT_Init();
	TFT_ClearScreen(0x0000);
	while(1)
	{		
		GUI_ShowPicture(0, 0, 176, 220);
		
		while(1);
	}
}
