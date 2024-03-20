#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"

#define PRINTF_BUF_LEN  1024
static char printf_buf[PRINTF_BUF_LEN];

uint32_t putstr(const char* s)
{
    uint32_t c = 0;
    while (*s) // while *s != NULL(\0) 
    {
        Hal_uart_put_char(*s++); // s+=1 after print
        c++;
    }
    return c; // return count
}

uint32_t printf_self(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vsprintf(printf_buf, format, args);
    va_end(args);
    return putstr(printf_buf);
}

uint32_t vsprintf(char* buf, const char* format, va_list arg)
{
    uint32_t c = 0;
    char ch;
    char* str;
    uint32_t uint;
    uint32_t hex;
    
    for (uint32_t i=0; format[i]; i++)
    {
        if (format[i] == '%')
        {
            i++;
            switch(format[i])
            {
                case 'c':
                    ch = (char)va_arg(arg, int32_t);
                    buf[c++] = ch;
                    break;
                case 's':
                    str = (char*)va_arg(arg, char*);
                    if (str == NULL)
                        str = "(null)";
                    while(*str)
                        buf[c++] = (*str++);
                    break;
                case 'u':
                    uint = (uint32_t)va_arg(arg, uint32_t);
                    c += utoa(&buf[c], uint, utoa_dec);
                    break;
                case 'x':
                    hex = (uint32_t)va_arg(arg, uint32_t);
                    c += utoa(&buf[c], hex, utoa_hex);
                    break;
            }
        }
        else
            buf[c++] = format[i];
    }
    if (c>=PRINTF_BUF_LEN)
    {
        buf[0] = '\0';
        return 0;
    }

    buf[c] = '\0';
    return c;
}

uint32_t utoa(char* buf, uint32_t val, utoa_t base)
{
    uint32_t c = 0;
    int32_t idx = 0;
    char tmp[11];

    do {
        uint32_t t = val%(uint32_t)base;
        if (t >= 10)
            t+= 'A'-'0'-10;
        tmp[idx] = (t+'0');
        val /= base;
        idx++;
    } while(val);

    idx--;
    while (idx >= 0)
    {
        buf[c++] = tmp[idx];
        idx--;
    }
    return c;
}

char* itoa(int value, char* str, int base) {
    char* ptr = str, *ptr1 = str;
    char tmp_char;
    int tmp_value;

    // 체크: 인벨리드 베이스
    if (base < 2 || base > 36) { *str = '\0'; return str; }

    do {
        tmp_value = value; // 만약 value가 10 이면
        value /= base; // base 로 나눈 몫을 value로하고
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while ( value );

    // 음수일 경우 '-' 부호 추가
    if (tmp_value < 0) 
        *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return str;
}