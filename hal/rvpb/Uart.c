#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"
#include "HalInterrupt.h"
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
// 000000d4 <Hal_uart_get_char_OR>:
//   d4:   e52db004        push    {fp} ; (str fp, [sp, #-4]!) // 현재 함수 프레임 포인터 스택에 넣음
//   d8:   e28db000        add     fp, sp, #0 // 스택 포인터(sp)의 현재 값을 프레임 포인터(fp)에 복사 새 함수 프레임의 시작점을 설정
//   dc:   e24dd014        sub     sp, sp, #20 // 스택 포인터 감소시켜 공간 할당
//   e0:   e1a03000        mov     r3, r0 // r0 값을 r3에 복사
//   e4:   e54b300d        strb    r3, [fp, #-13] // r3 값을 fp 가 가리키는 주소에서 13 바이트 뒤의 위치에 저장
//   e8:   e320f000        nop     {0}
//   ec:   e3003000        movw    r3, #0 // 16비트 값을 r3의 하위 워드에 씀 -> 반복 시작
//   f0:   e3403000        movt    r3, #0 // 16비트 값을 r3의 상위 워드에 씀
//   f4:   e5933000        ldr     r3, [r3] // r3가 가리키는 메모리주소에서 값을 읽어와 r3에 저장
//   f8:   e5933018        ldr     r3, [r3, #24]
//   fc:   e7e03253        ubfx    r3, r3, #4, #1 //r3에서 비트 필드를 추출하여 r3에 저장 4번째 비트부터 1비트
//  100:   e6ef3073        uxtb    r3, r3 // r3 값을 8비트로 제한하고 결과를 r3에 저장 하위 8비트만 남김
//  104:   e3530000        cmp     r3, #0  // immediate value 명령어에 직접 포함되어 있는 상수
//  108:   1afffff7        bne     ec <Hal_uart_get_char_OR+0x18> // not equal이면 지정된 주소로 분기
//  10c:   e3003000        movw    r3, #0
//  110:   e3403000        movt    r3, #0
//  114:   e5933000        ldr     r3, [r3]
//  118:   e5933000        ldr     r3, [r3]
//  11c:   e7e03553        ubfx    r3, r3, #10, #1
//  120:   e6ef3073        uxtb    r3, r3
//  124:   e3530000        cmp     r3, #0
//  128:   1a000017        bne     18c <Hal_uart_get_char_OR+0xb8>
//  12c:   e3003000        movw    r3, #0
//  130:   e3403000        movt    r3, #0
//  134:   e5933000        ldr     r3, [r3]
//  138:   e5933000        ldr     r3, [r3]
//  13c:   e7e03453        ubfx    r3, r3, #8, #1
//  140:   e6ef3073        uxtb    r3, r3
//  144:   e3530000        cmp     r3, #0
//  148:   1a00000f        bne     18c <Hal_uart_get_char_OR+0xb8>
//  14c:   e3003000        movw    r3, #0
//  150:   e3403000        movt    r3, #0
//  154:   e5933000        ldr     r3, [r3]
//  158:   e5933000        ldr     r3, [r3]
//  15c:   e7e035d3        ubfx    r3, r3, #11, #1
//  160:   e6ef3073        uxtb    r3, r3
//  164:   e3530000        cmp     r3, #0
//  168:   1a000007        bne     18c <Hal_uart_get_char_OR+0xb8>
//  16c:   e3003000        movw    r3, #0
//  170:   e3403000        movt    r3, #0
//  174:   e5933000        ldr     r3, [r3]
//  178:   e5933000        ldr     r3, [r3]
//  17c:   e7e034d3        ubfx    r3, r3, #9, #1
//  180:   e6ef3073        uxtb    r3, r3
//  184:   e3530000        cmp     r3, #0
//  188:   0a000018        beq     1f0 <Hal_uart_get_char_OR+0x11c>
//  18c:   e3003000        movw    r3, #0
//  190:   e3403000        movt    r3, #0
//  194:   e5932000        ldr     r2, [r3]
//  198:   e5923004        ldr     r3, [r2, #4]
//  19c:   e3833004        orr     r3, r3, #4 // r3값과 <value>를 OR 연산 결과 r3에 저장
//  1a0:   e5823004        str     r3, [r2, #4]  레지스터 r3의 값을 레지스터 r2가 가리키는 주소의 4 바이트 뒤에 저장
//  1a4:   e3003000        movw    r3, #0
//  1a8:   e3403000        movt    r3, #0
//  1ac:   e5932000        ldr     r2, [r3]
//  1b0:   e5923004        ldr     r3, [r2, #4]
//  1b4:   e3833001        orr     r3, r3, #1
//  1b8:   e5823004        str     r3, [r2, #4]
//  1bc:   e3003000        movw    r3, #0
//  1c0:   e3403000        movt    r3, #0
//  1c4:   e5932000        ldr     r2, [r3]
//  1c8:   e5923004        ldr     r3, [r2, #4]
//  1cc:   e3833008        orr     r3, r3, #8
//  1d0:   e5823004        str     r3, [r2, #4]
//  1d4:   e3003000        movw    r3, #0
//  1d8:   e3403000        movt    r3, #0
//  1dc:   e5932000        ldr     r2, [r3]
//  1e0:   e5923004        ldr     r3, [r2, #4]
//  1e4:   e3833002        orr     r3, r3, #2
//  1e8:   e5823004        str     r3, [r2, #4]
//  1ec:   ea000005        b       208 <Hal_uart_get_char_OR+0x134>
//  1f0:   e3003000        movw    r3, #0
//  1f4:   e3403000        movt    r3, #0
//  1f8:   e5933000        ldr     r3, [r3]
//  1fc:   e5933000        ldr     r3, [r3]
//  200:   e54b3005        strb    r3, [fp, #-5]
//  204:   e320f000        nop     {0}
//  208:   e28bd000        add     sp, fp, #0
//  20c:   e49db004        pop     {fp}            ; (ldr fp, [sp], #4)
//  210:   e12fff1e        bx      lr // 링크 레지스터에 저장된 주소로 실행을 분기 즉 종료 후 돌아가기




// optimize code using all
uint8_t Hal_uart_get_char_ALL(void)
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