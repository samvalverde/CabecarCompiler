; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: YínaKulíwa (Booleano) a Ékla (Carácter)
; Este programa toma un valor booleano y lo convierte en un carácter.
; Falso (kóyuwé) se convierte en '0' y Verdadero (síwa) en '1'.
; ---------------------------------------------------------------

datos segment
    booleano db ?              ; Variable para almacenar el booleano
    caracter db ?              ; Variable para almacenar el carácter
    mensajeEntrada db 'Ingrese un valor booleano (0 = kóyuwé, 1 = síwa): $'
    mensajeSalida db 'El carácter resultante es: $'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de YínaKulíwa (Booleano) a Ékla (Carácter).$'
    acercaDe2 db 'El booleano se convierte en un carácter.$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

leerBooleano proc near
    ; Solicitar el valor booleano al usuario
    Print mensajeEntrada
    mov ah, 01h
    int 21h
    sub al, '0'               ; Convertir el carácter a numérico
    mov booleano, al
    ret
leerBooleano endp

convertirACaracter proc near
    ; Convertir el booleano en un carácter ('0' o '1')
    cmp booleano, 1
    je esVerdadero

    ; Si es falso, el carácter será '0'
    mov caracter, '0'
    ret

esVerdadero:
    ; Si es verdadero, el carácter será '1'
    mov caracter, '1'
    ret
convertirACaracter endp

imprimirCaracter proc near
    ; Imprimir el carácter resultante ('0' o '1')
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
    call leerBooleano          ; Leer el valor booleano
    call convertirACaracter    ; Convertir el booleano a carácter
    call imprimirCaracter      ; Mostrar el carácter resultante
    call finRutina             ; Terminar el programa

codigo ends
end main
