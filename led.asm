section .data
    ; ESC [ 41m (Fundo Vermelho)
    on_seq    db 0x1B, '[41m', 0x20, 0x1B, '[D' ; Muda cor, escreve espaço, volta cursor
    on_len    equ $ - on_seq
    
    ; ESC [ 0m (Resetar tudo)
    off_seq   db 0x1B, '[0m', 0x20, 0x1B, '[D'
    off_len   equ $ - off_seq

section .text
    global asm_io_signal

asm_io_signal:
    push rbp
    mov rbp, rsp

    cmp rdi, 1          ; RDI é o argumento vindo do C
    je turn_on

turn_off:
    mov rax, 1          ; syscall: write
    mov rdi, 1          ; stdout
    mov rsi, off_seq
    mov rdx, off_len
    syscall
    jmp done

turn_on:
    mov rax, 1          ; syscall: write
    mov rdi, 1          ; stdout
    mov rsi, on_seq
    mov rdx, on_len
    syscall

done:
    pop rbp
    ret

section .note.GNU-stack noalloc noexec nowrite progbits