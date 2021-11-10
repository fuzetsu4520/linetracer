/*****************************************************************************
 *   poly_kuma.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2017, Polytech Center Kumamoto
 *   All rights reserved.
 *
 *   History
 *   2017.07.19  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __POLY_KUMA_H
#define __POLY_KUMA_H

#include "uart.h"

/*****************************************************************************
 * Groval Veriable
*****************************************************************************/


/*****************************************************************************
 * Functions
*****************************************************************************/
#define init_uart_io(rate)		init_uart(rate)
extern int uart_putchar(char c);
extern int uart_puts(char *s);
extern int uart_printf(const char *format, ...);

extern int uart_getchar(void);
extern int uart_gets(char *s, int len);
//extern int uart_scanf(const char *format, ...);

#endif /* end __POLY_KUMA_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
