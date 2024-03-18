ENTRY(vector_start) /*시작 위치의 심벌 지정*/
SECTIONS /*블록이 섹션 배치 정보를 가지고 있는 것이라고 알려줌*/
{
    . = 0x0; /* 첫 섹션이 메모리 주소 0x00000000에 위치함*/

    /* 섹션의 배치 순서를 지정 */
    .text :
    {
        *(vector_start)
        *(.text .rodata)
    }

    .data : 
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }
}