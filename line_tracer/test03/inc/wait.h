#ifndef __WAIT_H
#define __WAIT_H


//--------------------------------------------------------------------
// 定数・グローバル変数定義
//--------------------------------------------------------------------
#define	WAIT_COUNT	5350


//--------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------
extern inline void wait_1ms(void);
extern inline void wait_ms(unsigned short ms);


//--------------------------------------------------------------------
// inline関数の実体
// -> inline関数はcファイルではなくhファイルにプロトタイプ宣言と一緒に記述する
//--------------------------------------------------------------------
inline void wait_1ms(void){
    volatile unsigned int wait = 0;
	for(wait = 0; wait < WAIT_COUNT; wait++);	// 待ち
}

inline void wait_ms(unsigned short ms){
	unsigned short i;
	for(i = 0; i < ms; i++){
		wait_1ms();
	}
}

#endif
