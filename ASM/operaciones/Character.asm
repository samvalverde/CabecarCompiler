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
; Proyecto - Operaciones para Enteros (shtawa)
; IC5701 - Compiladores e Intérpretes
; ---------------------------------------------------------------

datos segment
    mensajeResultado db 13, 10, 'Resultado: $'
    mensajeEsDigito db 13, 10, 'Es un dígito.$'
    mensajeNoEsDigito db 13, 10, 'No es un dígito.$'
    mensajeEsLetra db 13, 10, 'Es una letra.$'
    mensajeNoEsLetra db 13, 10, 'No es una letra.$'
    mensajeMayor db 13, 10, 'Resultado de > : $'
    mensajeMenor db 13, 10, 'Resultado de < : $'
    mensajeIgual db 13, 10, 'Resultado de = : $'
    mensajeMayorIgual db 13, 10, 'Resultado de >= : $'
    mensajeMenorIgual db 13, 10, 'Resultado de <= : $'
    mensajeDiferente db 13, 10, 'Resultado de >< : $'
    mensajeTamaño db 'Longitud de carácter: 1$', 13, 10, '$'
    blank db 13, 10, '$'
    acercaDe db 'Operaciones con Carácter (ékla).$'
    charPredefinido db 'M'        ; Carácter fijo para las comparaciones
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

codigo segment
    assume cs:codigo, ds:datos, ss:pila

leerCaracter proc near
    mov si, 80h             ; Dirección del PSP para el argumento
    mov cl, byte ptr [si]    ; Leer la longitud del argumento
    inc si
    mov al, byte ptr [si]    ; Leer el carácter del argumento
    push ax                 ; Empujar el carácter en la pila
    ret
leerCaracter endp

imprimirCaracter proc near
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
    pop ax              ; Obtener el carácter desde la pila
    cmp al, '0'         ; Comparar con '0'
    jl noEsDigito
    cmp al, '9'         ; Comparar con '9'
    jg noEsDigito
    Print mensajeEsDigito
    push ax             ; Volver a empujar el carácter a la pila
    ret
noEsDigito:
    Print mensajeNoEsDigito
    push ax             ; Volver a empujar el carácter a la pila
    ret
tölö endp

jélá proc near
    pop ax              ; Obtener el carácter desde la pila
    cmp al, 'A'
    jl noEsLetra
    cmp al, 'Z'
    jle esLetra
    cmp al, 'a'
    jl noEsLetra
    cmp al, 'z'
    jg noEsLetra
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
    pop ax
    cmp al, 'a'
    jl noConvertir
    cmp al, 'z'
    jg noConvertir
    sub al, 32          ; Convertir a mayúscula
noConvertir:
    push ax
    ret
júru endp

kári proc near
    pop ax
    cmp al, 'A'
    jl noConvertir
    cmp al, 'Z'
    jg noConvertir
    add al, 32          ; Convertir a minúscula
noConvertir:
    push ax
    ret
kári endp

BikoCharacter proc near
    mov ax, 1                ; Longitud fija de un carácter
    Print mensajeTamaño
    ret
BikoCharacter endp

; ---------------------------------------------------------------
; Operaciones de Comparación
; ---------------------------------------------------------------

mayor proc near
    pop ax
    cmp al, charPredefinido
    jg esMayor
    Print mensajeMenor
    ret

esMayor:
    Print mensajeMayor
    ret
mayor endp

menor proc near
    pop ax
    cmp al, charPredefinido
    jl esMenor
    Print mensajeMayorIgual
    ret

esMenor:
    Print mensajeMenor
    ret
menor endp

igual proc near
    pop ax
    cmp al, charPredefinido
    je esIgual
    Print mensajeDiferente
    ret

esIgual:
    Print mensajeIgual
    ret
igual endp

mayorIgual proc near
    pop ax
    cmp al, charPredefinido
    jge esMayorIgual
    Print mensajeMenor
    ret

esMayorIgual:
    Print mensajeMayorIgual
    ret
mayorIgual endp

menorIgual proc near
    pop ax
    cmp al, charPredefinido
    jle esMenorIgual
    Print mensajeMayor
    ret

esMenorIgual:
    Print mensajeMenorIgual
    ret
menorIgual endp

diferente proc near
    pop ax
    cmp al, charPredefinido
    jne esDiferente
    Print mensajeIgual
    ret

esDiferente:
    Print mensajeDiferente
    ret
diferente endp

; ---------------------------------------------------------------
; Programa principal
; ---------------------------------------------------------------

main:
    mov ax, datos
    mov ds, ax
    mov es, ax

    mov ax, pila
    mov ss, ax

    Print acercaDe
    Print blank

    call leerCaracter

    ; Operaciones sobre el carácter
    call tölö
    call imprimirCaracter

    call jélá
    call imprimirCaracter

    call júru
    call imprimirCaracter

    call kári
    call imprimirCaracter

    ; Comparaciones
    call mayor
    call menor
    call igual
    call mayorIgual
    call menorIgual
    call diferente

    call BikoCharacter

    mov ax, 4C00h
    int 21h
codigo ends
end main
