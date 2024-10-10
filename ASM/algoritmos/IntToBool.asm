; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Shtáwa (Entero) a YínaKulíwa (Booleano)
; Este programa toma un número entero y lo convierte en un booleano:
; 0 se convierte en "kóyuwé" (falso), y cualquier otro valor se convierte en "síwa" (verdadero).
; ---------------------------------------------------------------

datos segment
    entero dw ?              ; Variable para almacenar el entero
    booleano db ?            ; Variable para almacenar el booleano (0 o 1)
    mensajeEntrada db 'Ingrese un número entero: $'
    mensajeSalida db 'El valor booleano es: $'
    boolFalso db 'kóyuwé$', 0
    boolVerdadero db 'síwa$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Shtáwa a YínaKulíwa.$'
    acercaDe2 db 'Entero convertido a booleano (falso o verdadero).$'
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

leerEntero proc near
    ; Solicitar el entero al usuario
    Print mensajeEntrada
    mov ah, 01h             ; Leer un carácter (dígito)
    int 21h
    sub al, '0'             ; Convertir el dígito de ASCII a numérico
    mov entero, ax          ; Guardar el valor leído
    ret
leerEntero endp

convertirABooleano proc near
    ; Convertir el entero en booleano
    mov ax, entero
    cmp ax, 0
    je esFalso              ; Si es 0, el booleano es kóyuwé (falso)
    mov booleano, 1         ; Si no es 0, el booleano es síwa (verdadero)
    jmp finConversion

esFalso:
    mov booleano, 0
finConversion:
    ret
convertirABooleano endp

imprimirBooleano proc near
    ; Mostrar el valor booleano
    cmp booleano, 0
    je imprimirFalso
    Print boolVerdadero
    jmp finImpresion

imprimirFalso:
    Print boolFalso

finImpresion:
    ret
imprimirBooleano endp

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
    call leerEntero         ; Leer número entero
    call convertirABooleano  ; Convertir el entero a booleano
    call imprimirBooleano   ; Mostrar el valor booleano
    call finRutina          ; Terminar el programa

codigo ends
end main
