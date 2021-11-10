#ifndef __PK_LTC
#define __PK_LTC


//--------------------------------------------------------------------
// 定数・グローバル変数定義
//--------------------------------------------------------------------
// LED制御向けの定数
#define LED_OFF	0											// 消灯
#define LED_ON	1											// 点灯

// モータ制御向けの定数
#define L_CW		2
#define L_CCW		1
#define R_CW		1
#define R_CCW		2
#define BRAKE		3
#define FREE		0
#define MOTOR_LEFT	1
#define MOTOR_RIGHT	2


//--------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------
// LED制御関数群
void init_led(void);
void set_led_orange(unsigned char value);
void set_led_green(unsigned char value);


// スイッチ制御関数群
void init_button(void);
unsigned char get_button_black(void);


// センサ制御関数群
void init_adc(void);
unsigned short get_adc( unsigned char ch );

#define get_left_line_sensor(void)		get_adc(0)
#define get_center_line_sensor(void)	get_adc(2)
#define get_right_line_sensor(void)		get_adc(1)
double get_distance(void);


// モータ制御関数群
void init_motor(void);
void rotate_motor(unsigned char m, unsigned char c);

void _forward(void);
void _back(void);
void _turn_left(void);
void _turn_right(void);
void _stop(void);
void _free(void);

void _forward_20ms(void);
void _back_20ms(void);
void _turn_left_20ms(void);
void _turn_right_20ms(void);


#endif
