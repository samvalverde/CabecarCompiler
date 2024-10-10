; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Shéj (String) a Ékla (Carácter)
; Este programa toma el primer carácter de un string y lo devuelve como resultado.
; ---------------------------------------------------------------

datos segment
    stringVar db 'Hola$', 0           ; String de entrada
    caracter db ?                     ; Variable para almacenar el carácter
    mensajeEntrada db 'El string es: $'
    mensajeSalida db 'El carácter resultante es: $'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Shéj (String) a Ékla (Carácter).$'
    acercaDe2 db 'El primer carácter del string es el resultado.$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

leerString proc near
    ; Mostrar el string almacenado
    Print mensajeEntrada
    lea dx, stringVar
    mov ah, 09h
    int 21h
    ret
leerString endp

convertirACaracter proc near
    ; Convertir el primer carácter del string en un carácter resultante
    lea si, stringVar
    mov al, [si]               ; Leer el primer carácter del string
    mov caracter, al           ; Almacenar el carácter en la variable
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
    call leerString            ; Leer el string de entrada
    call convertirACaracter    ; Convertir el primer carácter del string
    call imprimirCaracter      ; Mostrar el carácter resultante
    call finRutina             ; Terminar el programa

codigo ends
end main
