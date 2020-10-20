#ifndef __LCD1602_H
#define __LCD1602_H

void LCD_write_com(unsigned char com);
void LCD_write_Data(unsigned char Data);
void LCD_clear(void);
void LCD_init(void);
void LCD_write_char(unsigned char x,unsigned char y,unsigned char Data);
void LCD_write_str(unsigned char x,unsigned char y,unsigned char *s);
#endif 