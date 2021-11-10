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
#include "ex_dock.h"

//--------------------------------------------------------------------
// 定数・グローバル変数定義
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------
void init_timer16(void);
void init_motor2();
void drive_motor(short m1,short m2);

void forward(short speed);
void back(short speed);
void turn_left(short speed);
void turn_right(short speed);
void stop();

void forward_t(short speed, unsigned int msec);
void back_t(short speed, unsigned int msec);
void turn_left_t(short speed, unsigned int msec);
void turn_right_t(short speed, unsigned int msec);
#define get_left_line_sensor(void)		get_adc(0)
#define get_center_line_sensor(void)	get_adc(2)
#define get_right_line_sensor(void)		get_adc(1)
#define get_advance_line_sensor(void)	get_adc(3)

//--------------------------------------------------------------------
// メイン関数
//--------------------------------------------------------------------
int main(void) {
	char cmd;
	int speed = 8000;
	int msec = 3000;
	int old_speed = 0;
	int old_msec = 0;

	init_syscall();
	init_adc();
	init_motor2();

	printf("-----\n");
	printf("*** Motor Control TEST2 ***\n");
	printf("-----\n");
    while(1) {
    	printf("input command:");
    	scanf("%c", &cmd);
    	printf("\n");

    	switch(cmd){
    	case 'a':
    		forward(speed);
        	printf("> Forward\n");
    		break;
    	case 'b':
    		back(speed);
        	printf("> Back\n");
    		break;
    	case 'c':
    		turn_left(speed);
        	printf("> Turn Left\n");
    		break;
    	case 'd':
    		turn_right(speed);
        	printf("> Turn Right\n");
    		break;
    	case 'e':
    		stop();
        	printf("> Stop\n");
    		break;

    	case 'A':
    		forward_t(speed, msec);
        	printf("> Forward\n");
    		break;
    	case 'B':
    		back_t(speed, msec);
        	printf("> Back\n");
    		break;
    	case 'C':
    		turn_left_t(speed, msec);
        	printf("> Turn Left\n");
    		break;
    	case 'D':
    		turn_right_t(speed, msec);
        	printf("> Turn Right\n");
    		break;
    	case 'E':
    		stop();
        	printf("> Stop\n");
    		break;

    	case 'f':
    		old_speed = speed;
    		scanf("%d", &speed);
    		printf("> Speed Changed : %d -> %d\n", old_speed, speed);
    		break;
    	case 'g':
    		old_msec = msec;
    		scanf("%d", &msec);
    		printf("> Time Changed : %d -> %d\n", old_msec, msec);
    		break;

    	/*課題11-1*/
    	case '1':
    		printf("> kadai 11-1\n");
    		forward_t(speed, 1000);
    		turn_right_t(speed, 2000);
    		back_t(speed, 1500);
    		break;
    	case '2':
    		printf("> kadai 11-2\n");
    		for(int i=0;i<4;i++){
    		forward_t(speed, 1000);
    		turn_right_t(speed, 1000);
    		}
    		break;
    	case '3':
    		printf("> kadai 11-3\n");
    		for(int i=0;i<2;i++){
    		forward_t(speed, 1500);
    		turn_right_t(speed, 2000);
    		}
    		break;
    	case '4':
    		while(1){
    			if(get_left_line_sensor() > 500){
    				turn_left(speed);
    			}
    			else if(get_right_line_sensor() > 500){
    				turn_right(speed);
    			}
    			else{
    				forward(speed);
    			}
    		}
    	}
    }


    return 0 ;
}


//--------------------------------------------------------------------
// 関数本体
//--------------------------------------------------------------------
void init_motor2(){
	// GPIO
	LPC_GPIO2->DIR |= 0x000F;
	// LPC_GPIO0->DIR |= 0x0100;
	// LPC_GPIO1->DIR |= 0x0200;

	//Timer16B0,B1
	//use MTR1
	LPC_IOCON->PIO0_8   &= 0xE7;	// PullUp disable,CT16B0_MAT0
	LPC_IOCON->PIO0_8   |= 0x02;	// Selects function

	//use MTR2
	LPC_IOCON->PIO1_9   &= 0xE7;	// PullUp disable,CT16B1_MAT0
	LPC_IOCON->PIO1_9   |= 0x01;	// Selects function

	init_timer16();
}

void drive_motor(short m1,short m2){
	short mta1, mta2;
	unsigned int duty1, duty2;
	volatile unsigned int gpio2_tmp;

	gpio2_tmp = LPC_GPIO2->DATA;
	gpio2_tmp &= ~0x000F;

	mta1 = m1;
	mta2 = m2;

	// 反転制御(取付向き的にCWが反対になる)
	if(mta1 == 0x8000){
		mta1 = 0;
	}
	if(mta2 == 0x8000){
		mta2 = 0;
	}
	mta2 = -mta2;

	// 回転方向の制御・速度制御
	if(mta1 > 0){
		duty1 = (unsigned int)(~(mta1*2));
		gpio2_tmp |= 1;
	}
	else if(mta1 < 0){
		duty1 = (unsigned int)(~(-mta1*2));
		gpio2_tmp |= 2;
	}
	else{
		duty1 = 0;
	}

	if(mta2 > 0){
		duty2 = (unsigned int)(~(mta2 * 2));
		gpio2_tmp |= 1 << 1 * 2;
	}
	else if(mta2 < 0){
		duty2 = (unsigned int)(~(-mta2 * 2));
		gpio2_tmp |= 2 << 1 * 2;
	}
	else{
		duty2 = 0;
	}

	LPC_TMR16B0->MR0 = duty1 & 0x0000FFFF;
	LPC_TMR16B1->MR0 = duty2 & 0x0000FFFF;

	LPC_GPIO2->DATA = gpio2_tmp;
}

void init_timer16(void){
	/* Enable AHB clock to the CT16B0,CT16B1. */
	LPC_SYSCON->SYSAHBCLKCTRL |= ((1<<9)|(1<<8)|(1<<7));

	//Timer stop
	LPC_TMR16B0->TCR &= ~0x01;
	LPC_TMR16B1->TCR &= ~0x01;

	//set PWMmode
	LPC_TMR16B0->PWMC = (0<<3|0<<2|0<<1|1<<0);	//	MAT0 enable, MAT1,MAT2,MAT3 disable
	LPC_TMR16B1->PWMC = (0<<3|0<<2|0<<1|1<<0);	//	MAT0 enable, MAT1,MAT2,MAT3 disable

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
void forward(short speed){
	drive_motor(speed,-speed);
}

void back(short speed){
	drive_motor(-speed,speed);
}

void turn_left(short speed){
	drive_motor(speed,0);
}

void turn_right(short speed){
	drive_motor(0,-speed);
}

void stop(){
	drive_motor(0,0);
}

void forward_t(short speed, unsigned int msec){
	drive_motor(speed,-speed);
	wait_ms(msec);
	stop();
}

void back_t(short speed, unsigned int msec){
	drive_motor(-speed,speed);
	wait_ms(msec);
	stop();
}

void turn_left_t(short speed, unsigned int msec){
	drive_motor(speed,0);
	wait_ms(msec);
	stop();
}

void turn_right_t(short speed, unsigned int msec){
	drive_motor(0,-speed);
	wait_ms(msec);
	stop();
}


