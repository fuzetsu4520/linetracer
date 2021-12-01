#ifdef __USE_CMSIS
#include "LPC13xx.h"
#endif

#include "buzzer.h"

//--------------------------------------------------------------------
// 定数・グローバル変数定義
//--------------------------------------------------------------------
#define  TIME_DIV_FREQ 44100
#define  PCLK_FREQ     72000000

BYTE     multi_tone_mode = 0;
BYTE     bBuzzerFlag;
uint32_t iBuzzerFreq [2] = {Mt, Mt};

//--------------------------------------------------------------------
// 関数本体
//--------------------------------------------------------------------
void init_buzzer()
{
	bBuzzerFlag = 0;

	LPC_GPIO1->DIR  |= 0x0100;
	LPC_GPIO1->DATA &= ~0x0100;

	Timer32Init();
}

// 単音／重音モード設定
// 単音モード時	割り込み周期 ＝ ブザー周期/2
// 重音モード時	割り込み周期 ＝ 約20μsec  → （注意）高負荷につき main 処理に影響あり
void set_multi_tone_mode (int st)
{
	if (st == 0){
		multi_tone_mode = 0;
	}
	else{
		multi_tone_mode = 1;

		LPC_TMR32B0->MR0 = PCLK_FREQ / TIME_DIV_FREQ - 1;
	}
}

// ブザー制御に使用している32bitタイマの初期化関数
// vs-wrc103.cより抜粋．一部改変．
void Timer32Init(void)
{
	// Enable AHB clock to the CT32B0
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);

	// Enable 32bit timer0 Interrupt
	NVIC_EnableIRQ(TIMER_32_0_IRQn);

	//maximum value for the Prescale Counter. Divide Mainclk
	LPC_TMR32B0->PR = 0;

	//interrupt enable. TC clear MR2 matches TC reset enable
    //LPC_TMR32B0->MCR = (1<<7 | 1<<6);

	//MR0 interrupt enable, reset enable
	LPC_TMR32B0->MCR = (1<<1 | 1<<0);
}

void BuzzerSet(uint32_t freq, BYTE vol){

	BuzzerFreq (0, freq);
}

void BuzzerFreq(BYTE ch, uint32_t freq){

	static uint32_t last_f = Mt;

	iBuzzerFreq[ch] = freq;

	if (multi_tone_mode == 0){	// 単音モード

		if (ch == 0        &&
			freq != last_f &&
			freq != Mt){

			last_f = freq;
			LPC_TMR32B0->MR0 = PCLK_FREQ /(freq*2) - 1;

			if (bBuzzerFlag == 1){
				LPC_TMR32B0->TCR = 2;	//Timer Reset
				LPC_TMR32B0->TCR = 1;	//Timer Start
			}
		}
	}
}

void BuzzerStart(){

	bBuzzerFlag = 1;

	LPC_TMR32B0->TCR = 2;	//Timer Reset
	LPC_TMR32B0->TCR = 1;	//Timer Start
}

void BuzzerStop(){

	bBuzzerFlag = 0;
    LPC_TMR32B0->TCR = 0;	//Timer Stop

	LPC_GPIO1->MASKED_ACCESS[(1<<8)] = 0; // clear buzzer level
}

BYTE isBuzzer(){
	return bBuzzerFlag;
}

//--------------------------------------------------------------------
// 割り込み関数本体
//--------------------------------------------------------------------
void TIMER32_0_IRQHandler(void){

	static int32_t  irqCnt = 0;
	static uint32_t out = 0;

	uint32_t tempIR = LPC_TMR32B0->IR;

	if (multi_tone_mode == 1){	// 重音モード

		irqCnt ++;

		uint32_t ch   = irqCnt % 2;
		uint32_t freq = iBuzzerFreq [ch];
		out = 0;

		if (freq != Mt) {
			uint32_t half_cnt = (TIME_DIV_FREQ + freq) / (freq * 2);

			out = (irqCnt / half_cnt) & 1;
		}
	}
	else{						// 単音モード
		if (iBuzzerFreq [0] != Mt) out ^= 1;
		else					   out = 0;
	}
	LPC_GPIO1->MASKED_ACCESS[(1<<8)] = (out<<8);
	LPC_TMR32B0->IR = tempIR;
}
