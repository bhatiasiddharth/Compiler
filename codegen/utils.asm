; constants
_true db 'true', '$'
_false db 'false', '$'
_printstr db 80 dup('$')
_scanmax db 80
_scancount db ?
_scanstr db 80 dup('$')
; utility macros and procedures
; print 32 bit number
printnum macro number
local println, loopr

    push eax
    push edx
    push ebx
    push si
    mov cx,0
    mov eax,number
    mov ebx, 10

    test eax, eax
    jns loopr
    mov dl, '-'     ;negative sign
    mov ah, 2h
    int 21h
    mov eax, number
    neg eax
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


printstr macro str
  lea dx, str
  mov ah, 09h
  int 21h

  mov dl, 13d     ;Carriage Return
  mov ah, 2h
  int 21h
  mov dl, 10d     ;Line Feed
  mov ah, 2h
  int 21h
endm printstr


printbool macro bool
  local _print_true, _print_end
  cmp bool, 1
  jz _print_true
  printstr _false
  jz _print_end
  _print_true:
  printstr _true
  _print_end:
endm printbool


scanstr macro strval
  local _x1
  lea dx, _scanmax
  mov ah, 0ah
  int 21h
  lea si, _scanstr
  lea di, strval
  mov cx, 80
  rep movsb
  lea si, _scanstr
  mov cx, 80
  mov al, '$'
  _x1:
  mov [si], al
  loop _x1

  mov dl, 13d     ;Carriage Return
  mov ah, 2h
  int 21h
  mov dl, 10d     ;Line Feed
  mov ah, 2h
  int 21h
endm scanstr
