/*****************************************************************************
 *   serial.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2017, Polytech Center Kumamoto
 *   All rights reserved.
 *
 *   History
 *   2017.07.17  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __SERIAL_H
#define __SERIAL_H

//#include "type.h"
#include "stdint.h"

#define RS485_ENABLED   0
#define TX_INTERRUPT    0		/* 0 if TX uses polling, 1 interrupt driven. */
#define MODEM_TEST      0

#define IER_RBR         (0x01<<0)
#define IER_THRE        (0x01<<1)
#define IER_RLS         (0x01<<2)

#define IIR_PEND        0x01
#define IIR_RLS         0x03
#define IIR_RDA         0x02
#define IIR_CTI         0x06
#define IIR_THRE        0x01

#define LSR_RDR         (0x01<<0)
#define LSR_OE          (0x01<<1)
#define LSR_PE          (0x01<<2)
#define LSR_FE          (0x01<<3)
#define LSR_BI          (0x01<<4)
#define LSR_THRE        (0x01<<5)
#define LSR_TEMT        (0x01<<6)
#define LSR_RXFE        (0x01<<7)

#define BUFSIZE         0x40


/*****************************************************************************
 * Groval Veriable
*****************************************************************************/
extern volatile uint32_t UARTStatus;
extern volatile uint8_t  UARTTxEmpty;
extern volatile uint8_t  UARTBuffer[BUFSIZE];
extern volatile uint32_t UARTCount;


/*****************************************************************************
 * Functions
*****************************************************************************/
void init_uart(uint32_t Baudrate);
void UART_IRQHandler(void);
void send_uart(uint8_t *BufferPtr, uint32_t Length);


#endif /* end __SERIAL_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
