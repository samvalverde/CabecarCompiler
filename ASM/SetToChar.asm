; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Járjá (Conjunto) a Ékla (Carácter)
; Este programa toma el primer carácter de un conjunto y lo devuelve como el resultado.
; ---------------------------------------------------------------

datos segment
    conjunto db 'H', 'o', 'l', 'a', '$', 0  ; Conjunto de caracteres
    caracter db ?                            ; Variable para almacenar el carácter
    mensajeEntrada db 'El conjunto es: $'
    mensajeSalida db 'El carácter resultante es: $'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Járjá (Conjunto) a Ékla (Carácter).$'
    acercaDe2 db 'El primer carácter del conjunto es el resultado.$'
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

convertirACaracter proc near
    ; Convertir el primer carácter del conjunto en un carácter resultante
    lea si, conjunto
    mov al, [si]               ; Leer el primer carácter del conjunto
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
    call leerConjunto         ; Leer el conjunto de caracteres
    call convertirACaracter   ; Convertir el primer carácter del conjunto
    call imprimirCaracter     ; Mostrar el carácter resultante
    call finRutina            ; Terminar el programa

codigo ends
end main
