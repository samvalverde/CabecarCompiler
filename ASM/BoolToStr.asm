; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: YínaKulíwa (Booleano) a Shéj (String)
; Este programa toma un valor booleano y lo convierte en un string.
; Falso (kóyuwé) se convierte en "false" y Verdadero (síwa) en "true".
; ---------------------------------------------------------------

datos segment
    booleano db ?                ; Variable para almacenar el booleano
    stringVar db 6 dup ('$')     ; Arreglo para almacenar el string resultante
    mensajeEntrada db 'Ingrese un valor booleano (0 = kóyuwé, 1 = síwa): $'
    mensajeSalida db 'El string resultante es: $'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de YínaKulíwa (Booleano) a Shéj (String).$'
    acercaDe2 db 'El booleano se convierte en un string.$'
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

convertirAString proc near
    ; Convertir el booleano en un string ("false" o "true")
    cmp booleano, 1
    je esVerdadero

    ; Si es falso, el string será "false"
    lea si, stringVar
    mov byte ptr [si], 'f'
    mov byte ptr [si+1], 'a'
    mov byte ptr [si+2], 'l'
    mov byte ptr [si+3], 's'
    mov byte ptr [si+4], 'e'
    mov byte ptr [si+5], '$'
    ret

esVerdadero:
    ; Si es verdadero, el string será "true"
    lea si, stringVar
    mov byte ptr [si], 't'
    mov byte ptr [si+1], 'r'
    mov byte ptr [si+2], 'u'
    mov byte ptr [si+3], 'e'
    mov byte ptr [si+4], '$'
    ret
convertirAString endp

imprimirString proc near
    ; Imprimir el string resultante ("true" o "false")
    Print mensajeSalida
    lea dx, stringVar
    mov ah, 09h
    int 21h
    ret
imprimirString endp

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
    call convertirAString       ; Convertir el booleano a string
    call imprimirString         ; Mostrar el string resultante
    call finRutina              ; Terminar el programa

codigo ends
end main
