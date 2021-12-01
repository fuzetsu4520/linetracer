#ifdef __USE_CMSIS
#include "LPC13xx.h"
#endif

#include "pk_ltc.h"

//--------------------------------------------------------------------
// LED制御関数群
//--------------------------------------------------------------------
void init_led(void){
    LPC_GPIO0->DIR |= 0x88;
    LPC_GPIO2->DIR |= 0x800;
}

void set_led_orange(unsigned char value){
	if(value == LED_ON){
		LPC_GPIO0->DATA &= ~0x08;
	} else {
		LPC_GPIO0->DATA |= 0x08;
	}
}

void set_led_green(unsigned char value){
	if(value == LED_ON){
		LPC_GPIO0->DATA &= ~0x80;
	} else {
		LPC_GPIO0->DATA |= 0x80;
	}
}

//--------------------------------------------------------------------
// スイッチ制御関数群
//--------------------------------------------------------------------
void init_button(void){
    LPC_GPIO0->DIR &= ~0x0002;
}

unsigned char get_button_black(void){
	return (LPC_GPIO0->DATA & 0x0002) >> 1;
}


//--------------------------------------------------------------------
// センサ制御関数群
//--------------------------------------------------------------------
#define ADC_DONE		0x80000000
#define ADC_OVERRUN		0x40000000

void init_adc(void){
	// IO ControlでピンをADに設定
	LPC_IOCON->JTAG_TDI_PIO0_11	&= 0x67; // Clear bit7, change to analog mode. PullUp disable
	LPC_IOCON->JTAG_TDI_PIO0_11	|= 0x01; // ADC IN0
	LPC_IOCON->JTAG_TMS_PIO1_0	&= 0x67; // Clear bit7, change to analog mode. PullUp disable
	LPC_IOCON->JTAG_TMS_PIO1_0	|= 0x01; // ADC IN1
	LPC_IOCON->JTAG_TDO_PIO1_1	&= 0x67; // Clear bit7, change to analog mode. PullUp disable
	LPC_IOCON->JTAG_TDO_PIO1_1	|= 0x01; // ADC IN2
	LPC_IOCON->JTAG_nTRST_PIO1_2	&= 0x67; // Clear bit7, change to analog mode. PullUp disable
	LPC_IOCON->JTAG_nTRST_PIO1_2	|= 0x01; // ADC IN3
	LPC_IOCON->PIO1_4    &= 0x67; // Clear bit7, change to analog mode. PullUp disable
	LPC_IOCON->PIO1_4    |= 0x01; // ADC IN5
	LPC_IOCON->PIO1_10   &= 0x67; // Clear bit7, change to analog mode. PullUp disable
	LPC_IOCON->PIO1_10   |= 0x01; // ADC IN6

	/* Disable Power down bit to the ADC block. */
	// 3.5.47 Power-down configuration register(P41)
	// Power down configuration registerの略．0でPowerON, 1でPowerOff
	// 4bit目がADの電源管理ビット．最近のマイコンは低消費電力を意識している為，使わない機能に対しては電源を供給しない事で消費電力を落とす．
	LPC_SYSCON->PDRUNCFG &= ~(0x1<<4);

	/* Enable AHB clock to the ADC. */
	// 3.5.18 System AHB clock control register(P26)
	// ここのシステム及び周辺ブロックへのクロックのenable/disable制御
	// AHBからAPBブリッジ，AHBマトリックス, Coretex-M3, SysconブロックおよびPMUのクロックを提供可
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13);

	// ADCのコントロールレジスタを初期化
	LPC_ADC->CR =
		( 0x01 << 0 ) |					/* SEL=1,select channel 0~7 on ADC0 */			// どのピンでAD変換を開始するか．AD変換する対象のピンのビットを1立てる．とりあえずCH0を設定
		((48000000/1000000 - 1) << 8) |	/* CLKDIV = Fpclk / 1000000 - 1 */				// AD系のペリフェラルにクロック供給する
		( 0 << 16 ) | 					/* BURST = 0, no BURST, software controlled */	// バーストモード使うか否か．今回は使わない
		( 0 << 17 ) |  					/* CLKS = 0, 11 clocks/10 bits */				// バーストモード用の設定．使わないので初期値．
		( 1 << 21 ) |  					/* PDN = 1, normal operation */					// 予約
		( 0 << 22 ) |  					/* TEST1:0 = 00 */								// 予約
		( 0 << 24 ) |  					/* START = 0 A/D conversion stops */			// ココに1を立てるとAD変換開始．0ならAD変換停止
		( 0 << 27 );					/* EDGE = 0 (CAP/MAT singal falling,trigger A/D conversion) */	// CAPもしくはMATの立下り/立ち上がりでAD変換スタートできる､それの設定

	return;
}

unsigned short get_adc( unsigned char ch ){
	unsigned int reg_val, GDR;
	unsigned short ADC_Data;

	if ( ch > 7 ){
		ch = 0;
	}

	LPC_ADC->CR &= 0xFFFFFF00;				// SELビットを0クリア．ADは全ピンソフトウェア制御
	LPC_ADC->CR |= (1 << 24) | (1 << ch);	// お目当てのADチャンネルを変換開始

	while ( 1 )
	{
		switch(ch){
			case 0:
				reg_val = LPC_ADC->DR0;
				break;
			case 1:
				reg_val = LPC_ADC->DR1;
				break;
			case 2:
				reg_val = LPC_ADC->DR2;
				break;
			case 3:
				reg_val = LPC_ADC->DR3;
				break;
			case 4:
				reg_val = LPC_ADC->DR4;
				break;
			case 5:
				reg_val = LPC_ADC->DR5;
				break;
			case 6:
				reg_val = LPC_ADC->DR6;
				break;
			case 7:
				reg_val = LPC_ADC->DR7;
				break;
		}
		GDR = LPC_ADC->GDR;

		if ( GDR & ADC_DONE ){
			break;
		}
	}

	/* stop ADC now */
	LPC_ADC->CR &= 0xF8FFFFFF;

	if ( GDR & ADC_OVERRUN )	/* save data when it's not overrun, otherwise, return zero */
	{
		return ( 0 );
	}

	// AD変換されたデジタル値は6～15biの間に格納されている為，抽出する処理
	ADC_Data = (unsigned short)(( GDR >> 6 ) & 0x3FF);
	return ( ADC_Data );
}

double get_distance(void){
	unsigned short value;
	double voltage;
	double distance;

	value = get_adc(3);								// ADカウント値を取得
	voltage = 3.3 * value / 1024;					// 電圧計算
	distance = ((-200.0 * voltage) + 470.0) / 7.0;	// 距離計算
	return distance;
}


//--------------------------------------------------------------------
// モータ制御関数群
//--------------------------------------------------------------------
void init_motor(void){
	LPC_GPIO0->DIR |= 0x0100;	// 左モータ.速度調整
	LPC_GPIO1->DIR |= 0x0200;	// 右モータ.速度調整
	LPC_GPIO2->DIR |= 0x000F;	// 両モータ.方向

	LPC_GPIO0->DATA |= 0x0100;
	LPC_GPIO1->DATA |= 0x0200;
}

void rotate_motor(unsigned char m, unsigned char c){
	unsigned int reg_tmp;

	if(m == MOTOR_RIGHT){
		reg_tmp = LPC_GPIO2->DATA & ~0x0003;
		LPC_GPIO2->DATA = reg_tmp | c;
	}else{
		reg_tmp = LPC_GPIO2->DATA & ~0x000C;
		LPC_GPIO2->DATA = reg_tmp | (c << 2);
	}
}

void _forward(void){
	rotate_motor(MOTOR_LEFT, L_CCW);
	rotate_motor(MOTOR_RIGHT, R_CW);
}

void _back(void){
	rotate_motor(MOTOR_LEFT, L_CW);
	rotate_motor(MOTOR_RIGHT, R_CCW);
}

void _turn_left(void){
	rotate_motor(MOTOR_LEFT, L_CW);
	rotate_motor(MOTOR_RIGHT, R_CW);
}

void _turn_right(void){
	rotate_motor(MOTOR_LEFT, L_CCW);
	rotate_motor(MOTOR_RIGHT, R_CCW);
}

void _stop(void){
	rotate_motor(MOTOR_LEFT, BRAKE);
	rotate_motor(MOTOR_RIGHT, BRAKE);
}

void _free(void){
	rotate_motor(MOTOR_LEFT, FREE);
	rotate_motor(MOTOR_RIGHT, FREE);
}

void _forward_20ms(void){
	_forward();
	wait_ms(10);
	_free();
	wait_ms(10);
}

void _back_20ms(void){
	_back();
	wait_ms(10);
	_free();
	wait_ms(10);
}

void _turn_left_20ms(void){
	_turn_left();
	wait_ms(10);
	_free();
	wait_ms(10);
}
void _turn_right_20ms(void){
	_turn_right();
	wait_ms(10);
	_free();
	wait_ms(10);
}
