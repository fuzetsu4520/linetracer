#include "uart.h"
#include "uart_io.h"

void init_syscall(void){
	init_uart_io(9600);
}

int __sys_write(int iFileHandle, char *pcBuffer, int iLength)
{
    int n;
    for (n = 0; n < iLength; n++) {
        if (pcBuffer[n] == '\n'){
        	uart_putchar('\r');
        }
        uart_putchar(pcBuffer[n]);
    }
    return iLength;
}

int __sys_readc(){
	return uart_getchar();
}
