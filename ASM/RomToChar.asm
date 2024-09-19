; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Wömële (Número Romano) a Ékla (Carácter)
; Este programa toma un número romano válido y lo convierte en un carácter.
; El carácter corresponde al primer dígito del valor numérico.
; ---------------------------------------------------------------

datos segment
    numeroRomano db 'XIV$', 0    ; Número romano de entrada
    valorNumerico dw ?           ; Variable para almacenar el valor numérico
    caracter db ?                ; Variable para almacenar el carácter resultante
    mensajeEntrada db 'Ingrese un número romano: $'
    mensajeSalida db 'El carácter resultante es: $'
    mensajeError db 'Número romano no válido.$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Wömële (Número Romano) a Ékla (Carácter).$'
    acercaDe2 db 'El número romano se convierte en un carácter.$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

leerNumeroRomano proc near
    ; Solicitar el número romano al usuario
    Print mensajeEntrada
    lea si, numeroRomano
    ret
leerNumeroRomano endp

convertirANumero proc near
    ; Convertir el número romano en su valor numérico (misma lógica que antes)
    lea si, numeroRomano
    xor ax, ax
    mov valorNumerico, 0

    nextSymbol:
        mov al, [si]
        cmp al, '$'
        je finConversion

        cmp al, 'I'
        je agregarI
        cmp al, 'V'
        je agregarV
        cmp al, 'X'
        je agregarX
        cmp al, 'L'
        je agregarL
        cmp al, 'C'
        je agregarC
        cmp al, 'D'
        je agregarD
        cmp al, 'M'
        je agregarM

        Print mensajeError
        jmp finConversion

    agregarI:
        cmp byte ptr [si+1], 'V'
        je agregarMenosI
        cmp byte ptr [si+1], 'X'
        je agregarMenosI
        add valorNumerico, 1
        jmp siguienteSimbolo

    agregarMenosI:
        add valorNumerico, 9
        jmp siguienteSimbolo2

    agregarV:
        add valorNumerico, 5
        jmp siguienteSimbolo

    agregarX:
        cmp byte ptr [si+1], 'L'
        je agregarMenosX
        cmp byte ptr [si+1], 'C'
        je agregarMenosX
        add valorNumerico, 10
        jmp siguienteSimbolo

    agregarMenosX:
        add valorNumerico, 40
        jmp siguienteSimbolo2

    agregarL:
        add valorNumerico, 50
        jmp siguienteSimbolo

    agregarC:
        cmp byte ptr [si+1], 'D'
        je agregarMenosC
        cmp byte ptr [si+1], 'M'
        je agregarMenosC
        add valorNumerico, 100
        jmp siguienteSimbolo

    agregarMenosC:
        add valorNumerico, 400
        jmp siguienteSimbolo2

    agregarD:
        add valorNumerico, 500
        jmp siguienteSimbolo

    agregarM:
        add valorNumerico, 1000
        jmp siguienteSimbolo

    siguienteSimbolo:
        inc si
        jmp nextSymbol

    siguienteSimbolo2:
        add si, 2
        jmp nextSymbol

finConversion:
    ret
convertirANumero endp

convertirACaracter proc near
    ; Convertir el valor numérico en su primer carácter (ASCII)
    mov ax, valorNumerico
    mov bx, 10
    xor dx, dx
    div bx                  ; Obtener el primer dígito
    add dl, '0'             ; Convertir el dígito en carácter
    mov caracter, dl
    ret
convertirACaracter endp

imprimirCaracter proc near
    ; Imprimir el carácter resultante
    Print mensajeSalida
    mov al, caracter
    mov ah, 02h
    int 21h
    ret
imprimirCaracter endp

finRutina proc near
    mov ax, 4C00h
    int 21h
    ret
finRutina endp

main:
    mov ax, datos
    mov ds, ax
    mov es, ax

    mov ax, pila
    mov ss, ax

    ; Llamar a las rutinas en orden
    call leerNumeroRomano      ; Leer el número romano
    call convertirANumero       ; Convertir el número romano a valor numérico
    call convertirACaracter     ; Convertir el valor numérico a carácter
    call imprimirCaracter       ; Mostrar el carácter resultante
    call finRutina              ; Terminar el programa

codigo ends
end main
