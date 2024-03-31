#include "armcpu.h"

void enable_irq(void)
{
    __asm__ ("PUSH {r0, r1}");
    __asm__ ("MRS r0, cpsr");
    __asm__ ("BIC r1, r0, #0x80");
    __asm__ ("MSR cpsr, r1");
    __asm__ ("POP {r0, r1}");
}

void enable_fiq(void)
{
    __asm__ ("PUSH {r0, r1}");
    __asm__ ("MRS r0, cpsr"); // [CPSR - Current Program Status Register]를 r0에 복사 0x80은 IRQ의 마스크 비트
    __asm__ ("BIC r1, r0, #0x40"); // r1에 현재의 cpsr과 ~0x40의 and 연산 결과를 넣음 0x40은 FIQ의 마스크 비트 
    __asm__ ("MSR cpsr, r1"); // r1을 cpsr에 넣음
    __asm__ ("POP {r0, r1}");
}

void disable_irq(void)
{
    __asm__ ("PUSH {r0, r1}");
    __asm__ ("MRS r0, cpsr");
    __asm__ ("ORR r1, r0, #0x80");
    __asm__ ("MSR cpsr, r1");
    __asm__ ("POP {r0, r1}");
}

void disable_fiq(void)
{
    __asm__ ("PUSH {r0, r1}");
    __asm__ ("MRS r0, cpsr");
    __asm__ ("ORR r1, r0, #0x40");
    __asm__ ("MSR cpsr, r1");
    __asm__ ("POP {r0, r1}");
}