; Proyecto - Etapa 2: Operaciones por tipo de dato
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 09/10/2024

; ###### Manual de Usuario ######

; Este programa recibe un caracter por la pila
; e imprime en la pantalla las diferentes operaciones
; relacionadas con el tipo de dato en forma de bitácora o tour. 
; Se usan valores de ejemplo.

; ---------------------------------------------------------------
datos segment
    mensajeResultado db 13, 10, 'Resultado: $'
    mensajeEsDigito db 13, 10, 'Es un dígito.$'
    mensajeNoEsDigito db 13, 10, 'No es un dígito.$'
    mensajeEsLetra db 13, 10, 'Es una letra.$'
    mensajeNoEsLetra db 13, 10, 'No es una letra.$'
    blank db 13, 10, '$'
    acercaDe db 'Operaciones con Carácter (ékla).$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

; ---------------------------------------------------------------
codigo segment
    assume cs:codigo, ds:datos, ss:pila

leerCaracter proc near
    ; Leer el argumento pasado desde la línea de comandos y empujarlo en la pila
    mov si, 80h             ; Dirección del PSP para el argumento
    mov cl, byte ptr [si]    ; Leer la longitud del argumento
    inc si
    mov al, byte ptr [si]    ; Leer el carácter del argumento
    push ax                 ; Empujar el carácter en la pila
    ret
leerCaracter endp

imprimirCaracter proc near
    ; Imprimir el carácter desde la pila
    pop ax              ; Obtener el carácter desde la pila
    mov dl, al          ; Colocar el carácter en DL
    mov ah, 02h
    int 21h             ; Imprimir el carácter
    ret
imprimirCaracter endp

; ---------------------------------------------------------------
; Operaciones sobre caracteres
; ---------------------------------------------------------------

tölö proc near
    ; Verificar si el carácter es un dígito (0-9)
    pop ax              ; Obtener el carácter desde la pila
    cmp al, '0'         ; Comparar con '0'
    jl noEsDigito
    cmp al, '9'         ; Comparar con '9'
    jg noEsDigito
    ; Es un dígito
    Print mensajeEsDigito
    push ax             ; Volver a empujar el carácter a la pila
    ret
noEsDigito:
    Print mensajeNoEsDigito
    push ax             ; Volver a empujar el carácter a la pila
    ret
tölö endp

jélá proc near
    ; Verificar si el carácter es una letra (A-Z, a-z)
    pop ax              ; Obtener el carácter desde la pila
    cmp al, 'A'
    jl noEsLetra
    cmp al, 'Z'
    jle esLetra         ; Si está entre A-Z, es letra
    cmp al, 'a'
    jl noEsLetra
    cmp al, 'z'
    jg noEsLetra        ; Si está entre a-z, es letra
esLetra:
    Print mensajeEsLetra
    push ax             ; Volver a empujar el carácter a la pila
    ret
noEsLetra:
    Print mensajeNoEsLetra
    push ax             ; Volver a empujar el carácter a la pila
    ret
jélá endp

júru proc near
    ; Convertir el carácter a mayúscula
    pop ax              ; Obtener el carácter desde la pila
    cmp al, 'a'
    jl noConvertir
    cmp al, 'z'
    jg noConvertir
    sub al, 32          ; Convertir a mayúscula
noConvertir:
    push ax             ; Empujar el carácter convertido
    ret
júru endp

kári proc near
    ; Convertir el carácter a minúscula
    pop ax              ; Obtener el carácter desde la pila
    cmp al, 'A'
    jl noConvertir
    cmp al, 'Z'
    jg noConvertir
    add al, 32          ; Convertir a minúscula
noConvertir:
    push ax             ; Empujar el carácter convertido
    ret
kári endp

; ---------------------------------------------------------------
; Programa principal
; ---------------------------------------------------------------

main:
    ; Inicializar segmentos
    mov ax, datos
    mov ds, ax
    mov es, ax

    mov ax, pila
    mov ss, ax

    ; Mostrar AcercaDe
    Print acercaDe
    Print blank

    ; Leer el argumento como carácter
    call leerCaracter      ; Empujar el carácter en la pila

    ; Operaciones sobre el carácter
    call tölö              ; Verificar si es dígito
    call imprimirCaracter

    call jélá              ; Verificar si es letra
    call imprimirCaracter

    call júru              ; Convertir a mayúscula
    call imprimirCaracter

    call kári              ; Convertir a minúscula
    call imprimirCaracter

    ; Terminar el programa
    mov ax, 4C00h
    int 21h
codigo ends
end main

; character A