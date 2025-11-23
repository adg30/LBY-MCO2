section .data
kmh_to_ms: dq 0.277778 ; conversion from kmh to ms accdg to google
    
section .text
global calculate_accel

; rcx rdx r8 gets put into this
calculate_accel:
    push rbx
    xor rbx, rbx

.loop:
    cmp rbx, rcx
    jge .done

    ; address offset = rbx * 24 since 3 doubles
    mov rax, rbx ; rax stores the current car index
    imul rax, 24
    
    add rax, rdx ; point to the current car's data in memory

    ; load from matrix
    movsd xmm0, [rax] ; vi
    movsd xmm1, [rax + 8] ; vf
    movsd xmm2, [rax + 16] ; t

    ; compute acceleration 
    subsd xmm1, xmm0 ; vf-vi
    movsd xmm3, [rel kmh_to_ms]
    mulsd xmm1, xmm3 ; x 0.277778 
    divsd xmm1, xmm2 ; / t

    ; convert to int - not explicitly said 
    ; but example had it rounded
    ; thus, we also round
    roundsd xmm1, xmm1, 0
    cvtsd2si eax, xmm1 
    mov [r8 + rbx*4], eax ; r8 is a pointer to the output array in c

    inc rbx
    jmp .loop
.done:
    pop rbx
    ret
