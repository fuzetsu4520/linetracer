#ifdef __USE_CMSIS
#include "LPC13xx.h"
#endif

#include "ex_dock.h"

//--------------------------------------------------------------------
// LED制御関数群
//--------------------------------------------------------------------
void init_ex_led_full_color(void){
    LPC_GPIO3->DIR |= 0x07;
}

void set_ex_led_full_color(unsigned char value){
	LPC_GPIO3->DATA = value & 0x07;
}


//--------------------------------------------------------------------
// スイッチ制御関数群
//--------------------------------------------------------------------
void init_ex_slide_switch(void){
    LPC_GPIO2->DIR &= ~0x00F0;
}

unsigned char get_ex_slide_switch(void){
	return (LPC_GPIO2->DATA & 0x00F0) >> 4;
}

unsigned char is_ex_slide_switch1_on(void){
	return ((get_ex_slide_switch() & 0x01) == 0);
}

unsigned char is_ex_slide_switch2_on(void){
	return ((get_ex_slide_switch() & 0x02) == 0);
}

unsigned char is_ex_slide_switch3_on(void){
	return ((get_ex_slide_switch() & 0x04) == 0);
}

unsigned char is_ex_slide_switch4_on(void){
	return ((get_ex_slide_switch() & 0x08) == 0);
}

//--------------------------------------------------------------------
// センサ制御関数群
//--------------------------------------------------------------------
double get_temperature(void){
	unsigned short value;
	double voltage;
	double temprature;

	value = get_adc(6);						// ADカウント値を取得
	voltage = 3.3 * value / 1024;			// カウント値から電圧に変換(1カウント=約3.22mV)
	temprature = (voltage - 0.6) / 0.01;	// 電圧から温度に変換(1℃=10mV, +600mVのオフセット)
	return temprature;
}


//--------------------------------------------------------------------
// モータ制御関数群
//--------------------------------------------------------------------
