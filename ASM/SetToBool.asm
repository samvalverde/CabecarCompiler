; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Járjá (Conjunto) a YínaKulíwa (Booleano)
; Este programa toma un conjunto de caracteres y lo convierte en un booleano.
; Si el conjunto no está vacío, se convierte en síwa (verdadero).
; Si está vacío, se convierte en kóyuwé (falso).
; ---------------------------------------------------------------

datos segment
    conjunto db 'H', 'e', 'l', 'l', 'o', '$', 0  ; Conjunto de caracteres
    booleano db ?                                ; Variable para almacenar el valor booleano
    mensajeEntrada db 'El conjunto es: $'
    mensajeSalida db 'El valor booleano es: $'
    mensajeSiwa db 'síwa (verdadero)$', 0
    mensajeKoyuwe db 'kóyuwé (falso)$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Járjá (Conjunto) a YínaKulíwa (Booleano).$'
    acercaDe2 db 'El conjunto se convierte en un valor booleano.$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

leerConjunto proc near
    ; Mostrar el conjunto almacenado
    Print mensajeEntrada
    lea dx, conjunto
    mov ah, 09h
    int 21h
    ret
leerConjunto endp

convertirABooleano proc near
    ; Convertir el conjunto de caracteres en un valor booleano
    lea si, conjunto
    mov al, [si]
    cmp al, '$'                ; Verificar si el conjunto está vacío
    je esFalso

    ; Si contiene caracteres, el booleano será síwa (verdadero)
    mov booleano, 1
    ret

esFalso:
    ; Si el conjunto está vacío, el booleano será kóyuwé (falso)
    mov booleano, 0
    ret
convertirABooleano endp

imprimirBooleano proc near
    ; Imprimir el valor booleano (síwa o kóyuwé)
    Print mensajeSalida
    cmp booleano, 1
    je imprimirSiwa

    ; Si es kóyuwé (falso)
    Print mensajeKoyuwe
    ret

imprimirSiwa:
    ; Si es síwa (verdadero)
    Print mensajeSiwa
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
    call leerConjunto          ; Leer el conjunto de caracteres
    call convertirABooleano    ; Convertir el conjunto a booleano
    call imprimirBooleano      ; Mostrar el valor booleano resultante
    call finRutina             ; Terminar el programa

codigo ends
end main
