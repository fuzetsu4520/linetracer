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
// #include "ex_dock.h"

//--------------------------------------------------------------------
// 定数・グローバル変数定義
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// メイン関数
//--------------------------------------------------------------------
int main(void) {
	// init_syscall();
	init_led();
	init_button();
	init_motor();
	init_adc();

	set_led_orange(LED_ON);
	while(get_button_black());
	while(!get_button_black());
	set_led_orange(LED_OFF);
	set_led_green(LED_ON);

    while(1)
    {
    	for(int i=0;i<(1000/20);i++){
		_forward_20ms();
    	}
    	for(int i=0;i<(1000/20);i++){
    	_back_20ms();
    	}
    }

    return 0 ;
}

//--------------------------------------------------------------------
// 関数本体
//--------------------------------------------------------------------
