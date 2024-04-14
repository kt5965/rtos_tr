#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"
#include "HalInterrupt.h"
extern volatile PL011_t* Uart;
static void interrupt_handler(void);
void Hal_uart_init(void)
{
    Uart->uartcr.bits.UARTEN = 0; // UART disable 
    Uart->uartcr.bits.TXE = 1; // Transmit enable 1
    Uart->uartcr.bits.RXE = 1; // Reeive enable 1 
    Uart->uartcr.bits.UARTEN = 1; // UART enable
    // Enable input interrupt
    Uart->uartimsc.bits.RXIM = 1;
    // register UART interrupt handler
    Hal_interrupt_enable(UART_INTERRUPT0);
    Hal_interrupt_register_handler(interrupt_handler, UART_INTERRUPT0);
}



void Hal_uart_put_char(uint8_t ch)
{
    while(Uart->uartfr.bits.TXFF); // buf wait Transmit FIFO empty
    Uart->uartdr.all = (ch & 0xFF); // put ch buf limit 8 bit ch
}

uint8_t Hal_uart_get_char_OR(void)
{
    uint8_t data;
    while(Uart->uartfr.bits.RXFE);

    if (Uart->uartdr.bits.BE || Uart->uartdr.bits.FE || Uart->uartdr.bits.OE || Uart->uartdr.bits.PE)
    {
        Uart->uartrsr.bits.BE = 1;
        Uart->uartrsr.bits.FE = 1;
        Uart->uartrsr.bits.OE = 1;
        Uart->uartrsr.bits.PE = 1;
        return 0;
    }
    data = Uart->uartdr.bits.DATA;
    return data;
}

// optimize code using all
uint8_t Hal_uart_get_char(void)
{
    uint8_t data;
    while(Uart->uartfr.bits.RXFE);
    if (Uart->uartdr.all & 0xFFFFFF00)
    {
        Uart->uartrsr.all = 0xFF;
        return 0;
    }
    data = Uart->uartdr.bits.DATA;
    return data;
}


// optimize code reduce IO
uint8_t Hal_uart_get_char_once(void)
{
    uint8_t data;
    while(Uart->uartfr.bits.RXFE);
    data = Uart->uartdr.all;
    if (data & 0xFFFFFF00)
    {
        Uart->uartrsr.all = 0xFF;
        return 0;
    }
    return (uint8_t)(data & 0xFF); // return valid data
}

static void interrupt_handler(void)
{
    uint8_t ch = Hal_uart_get_char();
    Hal_uart_put_char(ch);
}