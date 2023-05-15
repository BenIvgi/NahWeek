IDEAL
MODEL small
STACK 800h
DATASEG
str0 db 104, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100, 33, 0
eods dw $ + 2
CODESEG
proc strCmp
param1 equ [bp + 6]
param2 equ [bp + 4]
	push bp                 
    mov  bp, sp            


    mov  si, param1         
    mov  di, param2       
	
cmp_loop:
    mov  al, [si]        
    cmp  al, [di]        
    jne  cmp_done          
    inc  si
	inc di
    cmp  al, 0              
    jne  cmp_loop
    mov  ax, 1
    jmp  cmp_exit
	
cmp_done:
    mov  ax, 0

cmp_exit:
    pop  bp
    ret 4
endp strCmp
proc toNumber
param1 equ [bp + 4]
push bp
mov bp, sp
mov si, param1
xor di, di
cmp [byte si], '-'
jne positive
negative:
mov di, 1
inc si
positive:
xor ax, ax
xor bx, bx
mov cx, 10
intParse:
mul cx
mov bl, [byte si]
sub bx, '0'
add ax, bx
inc si
cmp [byte si], 0
jne intParse
cmp di, 1
jne endtoNumber
neg ax
endtoNumber:
pop bp
ret 2
endp toNumber
proc input
param1 equ [bp + 4]
push bp
mov bp, sp
mov si, param1
push si
call typeNah
mov di, [eods]
push di
mov ah, 01h
inputloop:
int 21h
mov [di], al
inc di
cmp al, 13
jne inputloop
dec di
mov [byte di], 0
inc di
mov [eods], di
pop ax
pop bp
ret 2
endp input
proc strAdd
param1 equ [bp + 6]
param2 equ [bp + 4]
push bp
mov bp, sp
mov di, [eods]
push di
mov si, param1
mov cx, 2
writestr:
mov dl, [si]
mov [di], dl
inc di
inc si
cmp dl, 0
jne writestr
mov si, param2
dec di
loop writestr
inc di
mov [eods], di
pop ax
pop bp
ret 4
endp strAdd
proc toString
param1 equ [bp + 4]
push bp
mov bp, sp
mov ax, param1
mov di, [eods]
push di
xor cx, cx
cmp ax, 0
jge pos
mov [byte ptr di], '-'
neg ax
inc di
add [eods], 1
pos:
mov bx, 10
digs:
xor dx, dx
div bx
inc cx
add dl, '0'
push dx
cmp ax, 0
jne digs
add [eods], cx
writenum:
pop dx
mov [di], dl
inc di
loop writenum
mov [byte di], 0
add [eods], 1
pop ax
pop bp
ret 2
endp toString
proc typeNah
param1 equ [bp + 4]
push bp
mov bp, sp
mov si, param1
mov ah, 02h
printloop1:
mov dl, [si]
int 21h
inc si
cmp dl, 0
jne printloop1
mov ax, 0
pop bp
ret 2
endp typeNah
proc typeNahl
param1 equ [bp + 4]
push bp
mov bp, sp
mov si, param1
mov ah, 02h
printloop2:
mov dl, [si]
int 21h
inc si
cmp dl, 0
jne printloop2
mov dl, 0dh
mov ah, 2h
int 21h
mov dl, 0ah
mov ah, 2h
int 21h
mov ax, 0
pop bp
ret 2
endp typeNahl
proc pow
param1 equ [bp + 6]
param2 equ [bp + 4]
push bp
mov bp, sp
mov cx, param2
cmp cx, 0
jne pownot0
mov ax, 1
pop bp
ret 4
pownot0:
dec cx
cmp cx, 0
jne pownot1
mov ax, param1
pop bp
ret 4
pownot1:
mov ax, param1
mov bx, ax
multi:
mul bx
loop multi
pop bp
ret 4
endp pow
proc toPos
param1 equ [bp + 4]
push bp
mov bp, sp
mov ax, param1
cmp ax, 0
jg exitToPos
neg ax
exitToPos:
pop bp
ret 2
endp toPos
proc boolToStr
param1 equ [bp + 4]
push bp
mov bp, sp
mov ax, param1
mov di, [eods]
push di
cmp ax, 0
je False
True:
mov [byte di], 'T'
mov [byte di + 1], 'r'
mov [byte di + 2], 'u'
mov [byte di + 3], 'e'
mov [byte di + 4], 0
add di, 5
mov [eods], di
jmp endBoolToStr
False:
mov [byte di], 'F'
mov [byte di + 1], 'a'
mov [byte di + 2], 'l'
mov [byte di + 3], 's'
mov [byte di + 4], 'e'
mov [byte di + 5], 0
add di, 6
mov [eods], di
endBoolToStr:
pop ax
pop bp
ret 2
endp boolToStr
start:
mov ax, @data
mov ds, ax
call main
mov ah, 1
int 21h
exit :
mov ax, 04C00h
int 21h
proc main
push bp
sub sp, 2
msg equ [bp - 2]
mov ax, offset str0
mov msg, ax
mov ax, msg
push ax
call typeNah
mov ax, 0
pop bp
add sp, 2
ret 
endp main
end start
