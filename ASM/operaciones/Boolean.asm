; Proyecto - Etapa 2: Operaciones por tipo de dato
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 09/10/2024

; ###### Manual de Usuario ######

; Este programa recibe un booleano por la pila
; e imprime en la pantalla las diferentes operaciones
; relacionadas con el tipo de dato en forma de bitácora o tour. 
; Se usan valores de ejemplo.

; ---------------------------------------------------------------
; Proyecto - Operaciones para Booleanos (YinaKuliwa)
; IC5701 - Compiladores e Intérpretes
; ---------------------------------------------------------------

datos segment
    mensajeResultado db 13, 10, 'Resultado: $'
    mensajeMayor db 13, 10, 'Verdadero: Mayor que.$'
    mensajeMenor db 13, 10, 'Verdadero: Menor que.$'
    mensajeIgual db 13, 10, 'Verdadero: Igual.$'
    mensajeMayorIgual db 13, 10, 'Verdadero: Mayor o igual.$'
    mensajeMenorIgual db 13, 10, 'Verdadero: Menor o igual.$'
    mensajeDistinto db 13, 10, 'Verdadero: Distinto.$'
    mensajeFalso db 13, 10, 'Falso.$'
    mensajeTamaño db 'Longitud de booleano: 1$', 13, 10, '$'
    blank db 13, 10, '$'
    acercaDe db 'Operaciones con Booleano (YínaKulíwa).$'
    
    bool1 db ?
    bool2 db ?
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

leerBoolean proc near
    ; Leer los booleanos como argumentos (0 o 1) y empujarlos en la pila
    mov si, 80h                ; Dirección del PSP para el argumento
    mov cl, byte ptr [si]       ; Leer la longitud del argumento
    inc si
    mov al, byte ptr [si]       ; Leer el primer booleano
    sub al, '0'                ; Convertir de carácter a número
    mov bool1, al              ; Guardar el booleano 1
    push ax                    ; Empujar el booleano en la pila

    inc si
    mov al, byte ptr [si]       ; Leer el segundo booleano
    sub al, '0'                ; Convertir de carácter a número
    mov bool2, al              ; Guardar el booleano 2
    push ax                    ; Empujar el booleano en la pila
    ret
leerBoolean endp

; ---------------------------------------------------------------
; Operaciones de comparación
; ---------------------------------------------------------------

compMayor proc near
    ; Comparar si bool1 > bool2
    pop ax                   ; Obtener bool1
    pop bx                   ; Obtener bool2
    cmp ax, bx
    jg esMayor               ; Si ax > bx, verdadero
    Print mensajeFalso       ; Si no, es falso
    jmp finMayor
esMayor:
    Print mensajeMayor
finMayor:
    push ax                  ; Volver a empujar los valores
    push bx
    ret
compMayor endp

compMenor proc near
    ; Comparar si bool1 < bool2
    pop ax                   ; Obtener bool1
    pop bx                   ; Obtener bool2
    cmp ax, bx
    jl esMenor               ; Si ax < bx, verdadero
    Print mensajeFalso       ; Si no, es falso
    jmp finMenor
esMenor:
    Print mensajeMenor
finMenor:
    push ax                  ; Volver a empujar los valores
    push bx
    ret
compMenor endp

compIgual proc near
    ; Comparar si bool1 = bool2
    pop ax                   ; Obtener bool1
    pop bx                   ; Obtener bool2
    cmp ax, bx
    je esIgual               ; Si ax = bx, verdadero
    Print mensajeFalso       ; Si no, es falso
    jmp finIgual
esIgual:
    Print mensajeIgual
finIgual:
    push ax                  ; Volver a empujar los valores
    push bx
    ret
compIgual endp

compMayorIgual proc near
    ; Comparar si bool1 >= bool2
    pop ax                   ; Obtener bool1
    pop bx                   ; Obtener bool2
    cmp ax, bx
    jge esMayorIgual         ; Si ax >= bx, verdadero
    Print mensajeFalso       ; Si no, es falso
    jmp finMayorIgual
esMayorIgual:
    Print mensajeMayorIgual
finMayorIgual:
    push ax                  ; Volver a empujar los valores
    push bx
    ret
compMayorIgual endp

compMenorIgual proc near
    ; Comparar si bool1 <= bool2
    pop ax                   ; Obtener bool1
    pop bx                   ; Obtener bool2
    cmp ax, bx
    jle esMenorIgual         ; Si ax <= bx, verdadero
    Print mensajeFalso       ; Si no, es falso
    jmp finMenorIgual
esMenorIgual:
    Print mensajeMenorIgual
finMenorIgual:
    push ax                  ; Volver a empujar los valores
    push bx
    ret
compMenorIgual endp

compDistinto proc near
    ; Comparar si bool1 >< bool2
    pop ax                   ; Obtener bool1
    pop bx                   ; Obtener bool2
    cmp ax, bx
    jne esDistinto           ; Si ax != bx, verdadero
    Print mensajeFalso       ; Si no, es falso
    jmp finDistinto
esDistinto:
    Print mensajeDistinto
finDistinto:
    push ax                  ; Volver a empujar los valores
    push bx
    ret
compDistinto endp

BikoBoolean proc near
    mov ax, 1               ; Longitud fija de un booleano
    Print mensajeTamaño
    ret
BikoBoolean endp

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

    ; Leer los booleanos como argumentos
    call leerBoolean       ; Empujar los booleanos en la pila

    ; Realizar las comparaciones
    call compMayor         ; Comparar >
    call compMenor         ; Comparar <
    call compIgual         ; Comparar =
    call compMayorIgual    ; Comparar >=
    call compMenorIgual    ; Comparar <=
    call compDistinto      ; Comparar ><

    call BikoBoolean       ; Tamaño de un booleano

    ; Terminar el programa
    mov ax, 4C00h
    int 21h
codigo ends
end main

; boolean 1 0