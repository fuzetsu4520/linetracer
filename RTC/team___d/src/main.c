#ifdef __USE_CMSIS
#include "LPC13xx.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cr_section_macros.h>

#include "syscall.h"
#include "wait.h"
#include "pk_ltc.h"
#include "buzzer.h"

#define BLACK 600
#define WHITE 250
#define l_WHITE 300
#define c_BLACK 500
#define c_WHITE 400
#define ATTACK 5

//--------------------------------------------------------------------
// 定数・グローバル変数定義
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------
void init_timer16(void);
void init_motor2();
void drive_motor(short m1, short m2);

void forward(short speed);
void light_forward(short speed);
void back(short speed);
void turn_left(short speed);
void turn_right(short speed);
void back_left(short speed);
void back_right(short speed);
void stop();

void forward_t(short speed, unsigned int msec);
void back_t(short speed, unsigned int msec);
void turn_left_t(short speed, unsigned int msec);
void turn_right_t(short speed, unsigned int msec);
void light_forward_t(short speed, unsigned int msec);
void set_led_red(unsigned char value);

int get_graycount(int ls_c,int ls_l,int ls_r);

//--------------------------------------------------------------------
// メイン関数
//--------------------------------------------------------------------
int main(void) {
	int speed = 10000;
	int change_speed = 13000;
	int msec = 1000;
	int dist, ls_l, ls_c, ls_r, ls_h;
	int mode = 0;
	double duty;

	init_syscall();
	init_led();
	init_button();
	init_motor();
	init_adc();
	init_motor2();
	init_TIMER32_1_IRQHandler();		//タイマ割込み初期化

	set_led_orange(LED_ON);
	while (get_button_black());
	while (!get_button_black());
	set_led_orange(LED_OFF);
	set_led_green(LED_ON);

	while (1) {
		ls_l = get_left_line_sensor();
		ls_c = get_center_line_sensor();
		ls_r = get_right_line_sensor();
		ls_h = get_light_sensor();
		dist = get_distance();

		//printf("vol:%4d, l:%4d, c:%4d, r:%4d,", vol, ls_l, ls_c, ls_r);
		//printf("temp:%4.2lf, dist:%4.2lf\n", temp, dist);

		switch (mode) {

		case 0:
			if (dist < ATTACK) {
				stop();
			}
			else if (dist > ATTACK) {
				if (ls_h < 650) {
					set_led_red(LED_ON);
				}
				else {
					set_led_red(LED_OFF);
				}
				if (ls_r < WHITE) {
					turn_right(speed);
				}
				else if (ls_l < l_WHITE) {
					turn_left(speed);
				}
				else {
					forward(speed);
				}
			}
			if (ls_l < l_WHITE && ls_c < WHITE && ls_r < WHITE) {
				turn_right_t(speed, msec);
				mode = 1;
			}
			break;

		case 1:
			if (dist < ATTACK) {
				stop();
			}
			else if (dist > ATTACK) {
				if (ls_c > c_WHITE) {
					turn_right(change_speed);
				}
				else if (ls_c < c_WHITE) {
					turn_left(change_speed);
				}
			}
			if (ls_l < WHITE && ls_c < WHITE && ls_r < WHITE) {
				light_forward_t(speed, msec);
				mode = 0;
			}
			break;

//		case 2:
//			turn_left_t(speed, msec);
//			//turn_right_t(speed, msec*2);
//			//back_t(speed, msec);
//			mode = 3;
//			//mode = 4;
//			break;

//		case 3:
//			if (ls_r < WHITE) {
//				turn_right(speed);
//			}
//			else if (ls_l < l_WHITE) {
//				turn_left(speed);
//			}
//			else if (ls_l > BLACK&&ls_r > BLACK&&ls_c > BLACK) {
//				mode=5;
//			}
//			else {
//				forward(speed);
//			}
//			break;
//
//		/*case 4:
//			if (ls_r < WHITE) {
//				back_right(speed);
//			}
//			else if (ls_l < l_WHITE) {
//				back_left(speed);
//			}
//			else if (ls_l > BLACK && ls_r > BLACK && ls_c > BLACK) {
//				stop();
//			}
//			else {
//				back(speed);
//			}
//			break;*/
//
//		case 5:
//
//			stop();
//			set_led_orange(LED_ON);
//
//			break;

		}


	}

	return 0;
}


//--------------------------------------------------------------------
// 関数本体
//--------------------------------------------------------------------
void set_led_red(unsigned char value) {
	if (value == LED_ON) {
		LPC_GPIO2->DATA &= ~0x800;
	}
	else {
		LPC_GPIO2->DATA |= 0x800;
	}
}

void init_motor2() {
	// GPIO
	LPC_GPIO2->DIR |= 0x000F;
	// LPC_GPIO0->DIR |= 0x0100;
	// LPC_GPIO1->DIR |= 0x0200;

	//Timer16B0,B1
	//use MTR1
	LPC_IOCON->PIO0_8 &= 0xE7;	// PullUp disable,CT16B0_MAT0
	LPC_IOCON->PIO0_8 |= 0x02;	// Selects function

	//use MTR2
	LPC_IOCON->PIO1_9 &= 0xE7;	// PullUp disable,CT16B1_MAT0
	LPC_IOCON->PIO1_9 |= 0x01;	// Selects function

	init_timer16();
}

void drive_motor(short m1, short m2) {
	short mta1, mta2;
	unsigned int duty1, duty2;
	volatile unsigned int gpio2_tmp;

	gpio2_tmp = LPC_GPIO2->DATA;
	gpio2_tmp &= ~0x000F;

	mta1 = m1;
	mta2 = m2;

	// 反転制御(取付向き的にCWが反対になる)
	if (mta1 == 0x8000) {
		mta1 = 0;
	}
	if (mta2 == 0x8000) {
		mta2 = 0;
	}
	mta2 = -mta2;

	// 回転方向の制御・速度制御
	if (mta1 > 0) {
		duty1 = (unsigned int)(~(mta1 * 2));
		gpio2_tmp |= 1;
	}
	else if (mta1 < 0) {
		duty1 = (unsigned int)(~(-mta1 * 2));
		gpio2_tmp |= 2;
	}
	else {
		duty1 = 0;
	}

	if (mta2 > 0) {
		duty2 = (unsigned int)(~(mta2 * 2));
		gpio2_tmp |= 1 << 1 * 2;
	}
	else if (mta2 < 0) {
		duty2 = (unsigned int)(~(-mta2 * 2));
		gpio2_tmp |= 2 << 1 * 2;
	}
	else {
		duty2 = 0;
	}

	LPC_TMR16B0->MR0 = duty1 & 0x0000FFFF;
	LPC_TMR16B1->MR0 = duty2 & 0x0000FFFF;

	LPC_GPIO2->DATA = gpio2_tmp;
}

void init_timer16(void) {
	/* Enable AHB clock to the CT16B0,CT16B1. */
	LPC_SYSCON->SYSAHBCLKCTRL |= ((1 << 9) | (1 << 8) | (1 << 7));

	//Timer stop
	LPC_TMR16B0->TCR &= ~0x01;
	LPC_TMR16B1->TCR &= ~0x01;

	//set PWMmode
	LPC_TMR16B0->PWMC = (0 << 3 | 0 << 2 | 0 << 1 | 1 << 0);	//	MAT0 enable, MAT1,MAT2,MAT3 disable
	LPC_TMR16B1->PWMC = (0 << 3 | 0 << 2 | 0 << 1 | 1 << 0);	//	MAT0 enable, MAT1,MAT2,MAT3 disable

	//interrupt flag clear
	LPC_TMR16B0->IR = 0;
	LPC_TMR16B1->IR = 0;

	//interrupt
	LPC_TMR16B0->MCR = 0;
	LPC_TMR16B1->MCR = 0;

	//maximum value for the Prescale Counter. Divide Mainclk
	LPC_TMR16B0->PR = 0;
	LPC_TMR16B1->PR = 0;

	//Set Duty
	LPC_TMR16B0->MR0 = ~0x0000;	//MTR1
	LPC_TMR16B1->MR0 = ~0x0000;	//MTR2

	//Timer start
	LPC_TMR16B0->TCR |= 0x01;
	LPC_TMR16B1->TCR |= 0x01;

	return;
}

//----------------------------------------
void forward(short speed) {
	drive_motor(speed, -speed);
}

void light_forward(short speed) {
	drive_motor(speed, ((-speed) - 2000));
}

void back(short speed) {
	drive_motor(-speed, speed);
}

void turn_left(short speed) {
	drive_motor(speed, 0);
}

void turn_right(short speed) {
	drive_motor(0, -speed);
}

void back_left(short speed) {
	drive_motor(-speed, 0);
}

void back_right(short speed) {
	drive_motor(0, speed);
}

void stop() {
	drive_motor(0, 0);
}

void forward_t(short speed, unsigned int msec) {
	forward(speed);
	wait_ms(msec);
	stop();
}

void back_t(short speed, unsigned int msec) {
	back(speed);
	wait_ms(msec);
	stop();
}

void turn_left_t(short speed, unsigned int msec) {
	turn_left(speed);
	wait_ms(msec);
	stop();
}

void turn_right_t(short speed, unsigned int msec) {
	turn_right(speed);
	wait_ms(msec);
	stop();
}

void light_forward_t(short speed, unsigned int msec) {
	light_forward(speed);
	wait_ms(msec);
	stop();
}

//--------------------------------------------------------------------
// buzzer関数本体
//--------------------------------------------------------------------
// 500msec周期のタイマ割込みの初期化
void init_TIMER32_1_IRQHandler()
{
	NVIC_EnableIRQ(TIMER_32_1_IRQn);	//32bitタイマ割込みの有効化

	LPC_SYSCON->SYSAHBCLKCTRL |= 0x400; 	// タイマ動作クロック起動

	LPC_TMR32B1->PR = 7200 - 1; 			// 分周設定 ：10,000 Hz
	LPC_TMR32B1->MCR = 0x600; 			// MR3 を周期として割込み発生
	LPC_TMR32B1->MR3 = 5000 - 1;	 		// カウンタマッチ：10 Hz
	LPC_TMR32B1->TCR = 2; 				// カウンタリセット&停止
	LPC_TMR32B1->TCR = 1; 				// カウントスタート
}

//--------------------------------------------------------------------
// 割り込み関数本体
//--------------------------------------------------------------------

// タイマ割込み処理
void TIMER32_1_IRQHandler(void) {
	static unsigned char flg = 0;

	init_buzzer();

	switch(flg){

	case 0:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 1:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 2:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 3:
		set_buzzer(A1);
		start_buzzer();
		break;

	case 4:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 5:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 6:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 7:
		set_buzzer(E1);
		start_buzzer();
		break;

	case 8:
		set_buzzer(C1);
		start_buzzer();
		break;

	case 9:
		set_buzzer(C1);
		start_buzzer();
		break;

	case 10:
		set_buzzer(D1);
		start_buzzer();
		break;

	case 11:
		set_buzzer(E1);
		start_buzzer();
		break;

	case 12:
		set_buzzer(D1);
		start_buzzer();
		break;




	case 15:
		set_buzzer(E1);
		start_buzzer();
		break;

	case 17:
		set_buzzer(E1);
		start_buzzer();
		break;

	case 18:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 19:
		set_buzzer(A1);
		start_buzzer();
		break;

	case 20:
		set_buzzer(C2);
		start_buzzer();
		break;

	case 21:
		set_buzzer(C2);
		start_buzzer();
		break;

	case 22:
		set_buzzer(A1);
		start_buzzer();
		break;

	case 23:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 24:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 25:
		set_buzzer(A1);
		start_buzzer();
		break;

	case 26:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 27:
		set_buzzer(C2);
		start_buzzer();
		break;




	case 30:
		set_buzzer(C2);
		start_buzzer();
		break;

	case 32:
		set_buzzer(D2);
		start_buzzer();
		break;

	case 33:
		set_buzzer(C2);
		start_buzzer();
		break;

	case 34:
		set_buzzer(A1);
		start_buzzer();
		break;

	case 35:
		set_buzzer(C2);
		start_buzzer();
		break;

	case 36:
		set_buzzer(C2);
		start_buzzer();
		break;

	case 37:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 38:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 39:
		set_buzzer(A1);
		start_buzzer();
		break;

	case 40:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 41:
		set_buzzer(A1);
		start_buzzer();
		break;

	case 42:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 43:
		set_buzzer(E1);
		start_buzzer();
		break;




	case 46:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 47:
		set_buzzer(E1);
		start_buzzer();
		break;

	case 48:
		set_buzzer(D1);
		start_buzzer();
		break;

	case 49:
		set_buzzer(C1);
		start_buzzer();
		break;

	case 50:
		set_buzzer(D1);
		start_buzzer();
		break;

	case 51:
		set_buzzer(D1);
		start_buzzer();
		break;

	case 52:
		set_buzzer(C1);
		start_buzzer();
		break;

	case 53:
		set_buzzer(D1);
		start_buzzer();
		break;

	case 54:
		set_buzzer(E1);
		start_buzzer();
		break;

	case 55:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 56:
		set_buzzer(A1);
		start_buzzer();
		break;

	case 57:
		set_buzzer(G1);
		start_buzzer();
		break;

	case 58:
		set_buzzer(C2);
		start_buzzer();
		break;

	case 60:
		stop_buzzer();
		break;


	}

	flg++;

	if (flg == 61) {
		flg = 0;
	}

	LPC_TMR32B1->IR = 0x08; // タイマからの割込み要求の解除
}						  // IR : Interrupt Register

