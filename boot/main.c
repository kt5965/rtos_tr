#include "stdint.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "stdio.h"


static void Hw_init(void);
static void printf_test(void);

void main(void)
{
    Hw_init();
    uint32_t i = 100;
    while (i--)
    {
        Hal_uart_put_char('N');
    }

    Hal_uart_put_char('\n');
    putstr("Hello world\n");

    printf_test();
    uint32_t j = 100;
    while (j--)
    {
        uint8_t ch = Hal_uart_get_char_once();
        Hal_uart_put_char(ch);
    }
    while (1);
}

static void Hw_init(void)
{
    Hal_interrupt_init();
    Hal_uart_init();
}

static void printf_test(void)
{
    char* str = "printf test";
    char* nullptr = 0;
    uint32_t i = 3;

    printf_self("%s\n", "Hello");
    printf_self("output string pointer : %s\n", str);
    printf_self("%s is null pointer, %u number\n", nullptr, 10);
    printf_self("%u = 3\n", i);
    printf_self("dec = %u hex = %x\n", 0xff, 0xff);
}