#include "sb1602b.h"
#include "i2c.h"
//#include "wait.h"

//--------------------------------------
static unsigned char icon_data[] = {
		0x00, 0x10,
		0x02, 0x10,
		0x04, 0x10,
		0x06, 0x10,

		0x07, 0x10,
		0x07, 0x08,
		0x09, 0x10,
		0x0B, 0x10,

		0x0D, 0x04,
		0x0D, 0x08,
		0x0D, 0x10,
		0x0D, 0x02,

		0x0F, 0x10,		// アンテナマーク
};


//--------------------------------------
#define	LCD_ADDRESS		0x7C		// 0x3Eを1つ左ビットシフト
#define CONTRAST		48	//63			// 0～63で設定．高めに設定する事


//--------------------------------------
void lcd_cmd(unsigned char x);
void lcd_data(unsigned char x);

void lcd_cmd(unsigned char x){
	unsigned char buf[2];

	buf[0] = 0x00;		// コマンドモード
	buf[1] = x;			// 送信するコマンド
	I2CWrite(LCD_ADDRESS, buf, 2);
	wait_ms(3);
}

void lcd_data(unsigned char x){
	unsigned char buf[2];

	buf[0] = 0x40;		// コマンドモード
	buf[1] = x;			// 送信するコマンド
	I2CWrite(LCD_ADDRESS, buf, 2);
	wait_ms(1);
}

void lcd_init(void){
	I2CInit(I2CMASTER, 0);

	wait_ms(100);
	lcd_cmd(0x38);		// function set
	lcd_cmd(0x39);		// function set
	lcd_cmd(0x14);		// interval osc
	lcd_cmd(0x70 | (CONTRAST & 15)); 		//？ contrast low
	wait_ms(2);
	lcd_cmd(0x5c | (CONTRAST >> 4 & 3));	//？ contrast high / icon / power
	wait_ms(2);
	lcd_cmd(0x6B); 		//？ follower control  3.3V:6c 5V:69 follower on
	wait_ms(300);

	lcd_cmd(0x38);		// function set
	lcd_cmd(0x0c);		// display on
	lcd_cmd(0x01);		// clear display
	wait_ms(2);
}

void lcd_putchar(char c){
	lcd_data(c);
}

void lcd_puts(char *str){
	while(*str != 0x00){
		lcd_data(*str++);
	}
}

void lcd_put_icon(unsigned short flg){
	static unsigned char icon_buff[16];	// アイコンの編集用
	unsigned char i;

	for(i=0;i<sizeof(icon_data)/2;i++){
		if(flg & (0x1000>>i)){	// 該当ビットが立っていたら
			icon_buff[icon_data[i*2]] |= icon_data[i*2+1];	// バッファを立てます。
		} else {
			icon_buff[icon_data[i*2]] &= ~icon_data[i*2+1];	// バッファをクリアします。
		}
	}
	// 一括でLCDに書き込みます。
	for(i=0;i<16;i++){
		lcd_cmd(0b00111001);	// コマンド
		lcd_cmd(0b01000000+i);	// アイコン領域のアドレスを設定
		lcd_data(icon_buff[i]);	// アイコンデータ
	}
}

void lcd_clear(){
	lcd_cmd(0x01);
	wait_ms(2);
}

void lcd_locate(int x, int y){
	char pos = 0x80;
	pos += 0x40 * y + x;	// yが1なら0x40を加算
	lcd_cmd(pos);
}
