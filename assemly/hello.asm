global _start

section .data
        ; Align to the nearest 2 byte boundary, must be a power of two
        align 2
        ; String, which is just a collection of bytes, 0xA is newline
        str:     db 'Hello, world!',0xA
        strLen:  equ $-str

section .bss

; compares two long numbers
;       rsi -- address of the first long num
;       rdx -- length of the long num in qwords
;       rdi -- address of the second long num
;       rcx -- length of the second long num in qwords
;
; Result's stored in flags.
cmp_long:
                push rdi
                push rcx
                push rsi
                push rdx

                cmp rdx, rcx
                jne .exit

                std
                add rdi, rcx
                sub rdi, 1
                add rsi, rdx
                sub rsi, 1
                repe cmpsq

section .text
    _start:
        mov     edx, strLen
        mov     ecx, str
        mov     ebx, 1
        mov     eax, 4
        int     0x80

        mov     ebx, 0
        mov     eax, 1
        int     0x80