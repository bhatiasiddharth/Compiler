.model tiny
.data
str1 db ‘HELLO $’ ; all strings must terminate with ‘$’ ASCII value (24h)
.code
.startup
lea dx, str1
mov ah, 09h
int 21h
.exit
end
