; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Éka (Carácter) a YínaKulíwa (Booleano)
; Este programa toma un carácter y lo convierte en un booleano.
; Si el carácter es '0' o un espacio, se convierte en kóyuwé (falso).
; Cualquier otro carácter se convierte en síwa (verdadero).
; ---------------------------------------------------------------

datos segment
    caracter db ?            ; Variable para almacenar el carácter
    booleano db ?            ; Variable para almacenar el booleano
    mensajeEntrada db 'Ingrese un car',225,'cter: $'
    mensajeSalida db 'El valor booleano es: $'
    boolFalso db 'k',243,'yuw',233,'$', 0
    boolVerdadero db 's',237,'wa$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversi',243,'n de Éka (Carácter) a Y',237,'naKul',237,'wa (Booleano).$'
    acercaDe2 db 'El carácter se convierte en falso o verdadero.$'
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
    ; Solicitar el carácter al usuario
    Print mensajeEntrada
    mov ah, 01h             ; Leer un carácter
    int 21h
    mov caracter, al        ; Guardar el carácter leído
    ret
leerCaracter endp

convertirABooleano proc near
    ; Convertir el carácter a booleano
    cmp caracter, '0'
    je esFalso              ; Si es '0', es falso
    cmp caracter, ' '       ; Si es un espacio, también es falso
    je esFalso
    mov booleano, 1         ; Cualquier otro carácter es verdadero
    jmp finConversion

esFalso:
    mov booleano, 0         ; Falso para '0' o espacio
finConversion:
    ret
convertirABooleano endp

imprimirBooleano proc near
    ; Imprimir el valor booleano
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
    call leerCaracter       ; Leer un carácter
    call convertirABooleano ; Convertir el carácter a booleano
    call imprimirBooleano   ; Mostrar el valor booleano
    call finRutina          ; Terminar el programa

codigo ends
end main
