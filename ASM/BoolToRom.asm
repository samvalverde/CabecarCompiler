; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: YínaKulíwa (Booleano) a Wömële (Número Romano)
; Este programa toma un valor booleano y lo convierte en un número romano.
; Falso (kóyuwé) se convierte en 0 (sin representación) y Verdadero (síwa) en "I".
; ---------------------------------------------------------------

datos segment
    booleano db ?               ; Variable para almacenar el booleano
    numeroRomano db 3 dup ('$') ; Arreglo para almacenar el número romano
    mensajeEntrada db 'Ingrese un valor booleano (0 = kóyuwé, 1 = síwa): $'
    mensajeSalida db 'El número romano resultante es: $'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de YínaKulíwa (Booleano) a Wömële (Número Romano).$'
    acercaDe2 db 'El booleano se convierte en un número romano.$'
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
    sub al, '0'                 ; Convertir el carácter a numérico
    mov booleano, al
    ret
leerBooleano endp

convertirARomano proc near
    ; Convertir el booleano en un número romano ("0" o "I")
    cmp booleano, 1
    je esVerdadero

    ; Si es falso, el número romano será "0" (sin representación)
    lea si, numeroRomano
    mov byte ptr [si], '0'
    mov byte ptr [si+1], '$'
    ret

esVerdadero:
    ; Si es verdadero, el número romano será "I"
    lea si, numeroRomano
    mov byte ptr [si], 'I'
    mov byte ptr [si+1], '$'
    ret
convertirARomano endp

imprimirRomano proc near
    ; Imprimir el número romano resultante ("I" o "0")
    Print mensajeSalida
    lea dx, numeroRomano
    mov ah, 09h
    int 21h
    ret
imprimirRomano endp

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
    call leerBooleano           ; Leer el valor booleano
    call convertirARomano       ; Convertir el booleano a número romano
    call imprimirRomano         ; Mostrar el número romano resultante
    call finRutina              ; Terminar el programa

codigo ends
end main
