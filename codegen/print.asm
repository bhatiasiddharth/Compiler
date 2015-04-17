.model tiny
.486
.data
x_0 dd 5234567
printnum macro number
local println, loopr

    push eax
    push edx
    push ebx
    push si
    mov cx,0
    mov eax,number
    mov ebx, 10
 loopr:
    xor edx, edx
    div ebx         ; eax <- eax/10, edx <- eax % 10
    add dl, '0'     ; edx to ASCII
    push dx
    inc cx
    cmp eax, 0
    jnz loopr

    println:
    pop dx
    mov ah, 2h
    int 21h
    dec cx
    jnz println

    mov dl, 13d     ;Carriage Return
    mov ah, 2h
    int 21h
    mov dl, 10d     ;Line Feed
    mov ah, 2h
    int 21h

    pop si
    pop edx
    pop eax
   endm printnum

.code
.startup
printnum x_0
.exit
end
