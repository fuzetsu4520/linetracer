#ifndef __EX_DOCK
#define __EX_DOCK

#include "pk_ltc.h"

//--------------------------------------------------------------------
// 定数・グローバル変数定義
//--------------------------------------------------------------------
// ブレッドボード上のフルカラーLED制御向けの定数
#define LED_BLANK		0x00								// 消灯
#define LED_RED			0x01								// 赤色
#define LED_GREEN		0x02								// 緑色
#define LED_BLUE		0x04								// 青色
#define	LED_PURPLE		(LED_RED | LED_BLUE)				// 紫色
#define	LED_YELLOW		(LED_RED | LED_GREEN)				// 黄色
#define	LED_LIGHT_BLUE	(LED_BLUE | LED_GREEN)				// 水色
#define	LED_WHITE		(LED_RED | LED_BLUE | LED_GREEN)	// 白色

//--------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------
// LED制御関数群
void init_ex_led_full_color(void);
void set_ex_led_full_color(unsigned char value);

// スイッチ制御関数群
void init_ex_slide_switch(void);
unsigned char get_ex_slide_switch(void);
unsigned char is_ex_slide_switch1_on(void);
unsigned char is_ex_slide_switch2_on(void);
unsigned char is_ex_slide_switch3_on(void);
unsigned char is_ex_slide_switch4_on(void);

// センサ制御関数群
#define get_volume(void)				get_adc(5)
double get_temperature(void);

// モータ制御関数群

#endif
