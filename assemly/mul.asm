global _start

section .text

_start:
                push rbp
                mov rbp, rsp

                sub rsp, 7 * long_offset
                         ; 1|2|33|4.44

                lea r12, [rsp]
                lea r13, [rsp + 1 * long_offset]
                lea r14, [rsp + 2 * long_offset]
                lea r15, [rsp + 4 * long_offset]

                mov rdi, r12
                mov rcx, long_length
                call read_long

                lea rdi, [r13]
                call read_long

                mov rcx, long_length
                mov rdx, long_length
                call mul_long_long

                lea rdi, [r14]
                mov rcx, long_length * 2
                call write_long

                add rsp, 7 * long_offset

                leave
                jmp exit


; Multiplies 2 ints A and B and stores result in [r14]
; r12 - first long      long_length x1 size
; r13 - second long     long_length x1 size
; r14 - result long     long_length x2 size
; r15 - buffer          long_length x3 size
mul_long_long:
                push rcx
                push rdi
                push rsi
                push r13

                mov rcx, long_length ; constant

                ; setting buffer and result to zero
                mov r11, rcx
                lea rdi, [r14]
                mov rcx, long_length * 5
                call set_zero
                mov rcx, r11
                ; --------------------------------

.loop:
                cmp rcx, 0
                jz .exit

                ; Setting buffer to zero
                mov r10, rcx
                lea rdi, [r15]
                mov rcx, long_length * 3
                call set_zero
                mov rcx, r10
                ; ---------------------------

                ; loading A to buffer
                lea rsi, [r12]
                lea rdi, [r15 + long_offset]
                mov rcx, r11
                rep movsq

                mov rcx, r10
                ; ------------------------

                ; multiplying to a short
                mov rbx, [r13]
                lea rdi, [r15 + long_offset]
                mov rcx, long_length

                call mul_long_short

                mov rcx, r10
                ; ----------------------

                ; calculating shift
                lea r9, [r15 + long_offset]
                mov rax, r11
                sub rax, rcx
                shl rax, 3

                sub r9, rax
                ; --------------

                ; Debug
                lea r8, [r15 + long_offset]
                sub r8, r9
                ; -----

                ; adding current long to accumulator
                lea rdi, [r14]
                lea rsi, [r9]
                mov rcx, long_length * 2
                call add_long_long

                mov rcx, r10
                ; ----------------

                sub rcx, 1
                lea r13, [r13 + 8]
                jmp .loop

.exit:
                pop r13
                pop rsi
                pop rdi
                pop rcx
                ret

; adds two long number
;    rdi -- address of summand #1 (long number)
;    rsi -- address of summand #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    sum is written to rdi
add_long_long:
                push            rdi
                push            rsi
                push            rcx

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                adc             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret

; Substracts a long number from a long number (like a -= b)
;   rdi -- first long
;   rcx -- long length
;   rsi -- second long
;
; Result's address is stored in rdi
sub_long:
                push            rdi
                push            rsi
                push            rcx

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                sbb             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret

; adds 64-bit number to long number
;    rdi -- address of summand #1 (long number)
;    rax -- summand #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    sum is written to rdi
add_long_short:
                push            rdi
                push            rcx
                push            rdx

                xor             rdx,rdx
.loop:
                add             [rdi], rax
                adc             rdx, 0
                mov             rax, rdx
                xor             rdx, rdx
                add             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rdx
                pop             rcx
                pop             rdi
                ret

; multiplies long number by a short
;    rdi -- address of multiplier #1 (long number)
;    rbx -- multiplier #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    product is written to rdi
mul_long_short:
                push            rax
                push            rdi
                push            rcx

                xor             rsi, rsi
.loop:
                mov             rax, [rdi]
                mul             rbx
                add             rax, rsi
                adc             rdx, 0
                mov             [rdi], rax
                add             rdi, 8
                mov             rsi, rdx
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rdi
                pop             rax
                ret

; divides long number by a short
;    rdi -- address of dividend (long number)
;    rbx -- divisor (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    quotient is written to rdi
;    rdx -- remainder
div_long_short:
                push            rdi
                push            rax
                push            rcx

                lea             rdi, [rdi + 8 * rcx - 8]
                xor             rdx, rdx

.loop:
                mov             rax, [rdi]
                div             rbx
                mov             [rdi], rax
                sub             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rax
                pop             rdi
                ret


;    rdi -- location for output (long number)
;    rcx -- length of long number in qwords
read_long:
                push            rcx
                push            rdi
                call            set_zero
.loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              .done
                cmp             rax, '0'
                jb              .invalid_char
                cmp             rax, '9'
                ja              .invalid_char

                sub             rax, '0'
                mov             rbx, 10
                call            mul_long_short
                call            add_long_short
                jmp             .loop

.done:
                pop             rdi
                pop             rcx
                ret

.invalid_char:
                mov             rsi, invalid_char_msg
                mov             rdx, invalid_char_msg_size
                call            print_string
                call            write_char
                mov             al, 0x0a
                call            write_char

.skip_loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              exit
                jmp             .skip_loop

; write long number to stdout
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
write_long:
                push            rax
                push            rcx

                mov             rax, 20
                mul             rcx
                mov             rbp, rsp
                sub             rsp, rax

                mov             rsi, rbp

.loop:
                mov             rbx, 10
                call            div_long_short
                add             rdx, '0'
                dec             rsi
                mov             [rsi], dl
                call            is_zero
                jnz             .loop

                mov             rdx, rbp
                sub             rdx, rsi
                call            print_string

                mov             rsp, rbp
                pop             rcx
                pop             rax
                ret

; read one char from stdin
; result:
;    rax == -1 if error occurs
;    rax \in [0; 255] if OK
read_char:
                push            rcx
                push            rdi

                sub             rsp, 1
                xor             rax, rax
                xor             rdi, rdi
                mov             rsi, rsp
                mov             rdx, 1
                syscall

                cmp             rax, 1
                jne             .error
                xor             rax, rax
                mov             al, [rsp]
                add             rsp, 1

                pop             rdi
                pop             rcx
                ret
.error:
                mov             rax, -1
                add             rsp, 1
                pop             rdi
                pop             rcx
                ret

; write one char to stdout, errors are ignored
;    al -- char
write_char:
                sub             rsp, 1
                mov             [rsp], al

                mov             rax, 1
                mov             rdi, 1
                mov             rsi, rsp
                mov             rdx, 1
                syscall
                add             rsp, 1
                ret

; assigns a zero to long number
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
set_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep stosq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; checks if a long number is a zero
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
; result:
;    ZF=1 if zero
is_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep scasq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; just exit
exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall

; print string to stdout
;    rsi -- string
;    rdx -- size
print_string:
                push            rax

                mov             rax, 1
                mov             rdi, 1
                syscall

                pop             rax
                ret

section         .rodata
long_offset             equ     256 * 8
long_length             equ     long_offset / 8
invalid_char_msg:       db      "Invalid character: "
invalid_char_msg_size:  equ     $-invalid_char_msg
sec_gr_msg:             db      "Invalid long"
sec_gr_msg_size:        equ     $-sec_gr_msg
