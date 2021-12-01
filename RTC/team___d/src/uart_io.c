#include <uart_io.h>
#include "lpc13xx.h"
#include "uart.h"

#include "stdio.h"
#include "string.h"
#include "stdarg.h"

// uart版のputchar関数
int uart_putchar(char c)
{
	send_uart((uint8_t *)&c, 1);
	return 0;
}

// uart版のputs関数
int uart_puts(char *s)
{
	send_uart((uint8_t *)s, strlen(s));
	uart_putchar('\n');
	return 0;
}

// uart版のprintf関数
int uart_printf(const char *format, ...)
{
	va_list p_arg;
	char buf[256];

	va_start(p_arg, format);
	vsprintf(buf, format, p_arg);
	send_uart((uint8_t *)buf, strlen(buf));
	va_end(p_arg);

	return 0;
}

// uart版getchar関数
int uart_getchar(void)
{
	while ( UARTCount == 0 );
	UARTCount = 0;
	return UARTBuffer[0];
}

// uart版のgets関数
// 改行コードまでの読み出し
// lenはあくまでも文字配列の大きさの上限
int uart_gets(char *s, int len)
{
	do
	{
		*s = uart_getchar();
	}while (*s++ != '\n');
	*s = '\0';

	return 0;
}

// uart版のscanf関数
/*
int uart_scanf(const char *format, ...)
{
	va_list p_arg;
	char buf[256];

	uart_gets(buf, 256);

	va_start(p_arg, format);
	vsscanf(buf, format, p_arg);
	va_end(p_arg);

	return 0;
}
*/
