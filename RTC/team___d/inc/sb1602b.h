#ifndef __SB1602B_H
#define __SB1602B_H


void lcd_init(void);
void lcd_putchar(char c);
void lcd_puts(char *str);
void lcd_clear();
void lcd_locate(int x, int y);			// 左上は x=0, y=0

// LCD上部のアイコンを表示させる関数
//  9個のアイコンがあり，13bitで制御
//  左端のアンテナを付けたい場合は0x20. 右端の①を付けたい場合は0x01.
//  bit単位で紐づいている為，複数のアイコンを表示したい場合は，表示したいアイコンに対応するビットを複数立てる
void lcd_put_icon(unsigned short flg);


#endif
