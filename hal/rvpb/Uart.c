#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"

extern volatile PL011_t* Uart;

void Hal_uart_init(void)
{
    Uart->uartcr.bits.UARTEN = 0; // UART disable 
    Uart->uartcr.bits.TXE = 1; // Transmit enable 1
    Uart->uartcr.bits.RXE = 1; // Reeive enable 1 
    Uart->uartcr.bits.UARTEN = 1; // UART enable
}

void Hal_uart_put_char(uint8_t ch)
{
    while(Uart->uartfr.bits.TXFF); // buf wait Transmit FIFO empty
    Uart->uartdr.all = (ch & 0xFF); // put ch buf 
}