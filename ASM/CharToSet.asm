; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Ékla (Carácter) a Járjá (Conjunto)
; Este programa toma un carácter y lo convierte en un conjunto
; que contiene solo ese carácter.
; ---------------------------------------------------------------

datos segment
    caracter db ?             ; Variable para almacenar el carácter
    conjunto db 12 dup ('$')  ; Arreglo para almacenar el conjunto
    mensajeEntrada db 'Ingrese un carácter: $'
    mensajeSalida db 'El conjunto resultante es: {: ', 0
    finConjunto db ' :}$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Ékla (Carácter) a Járjá (Conjunto).$'
    acercaDe2 db 'El carácter se convierte en un conjunto.$'
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

convertirAConjunto proc near
    ; Convertir el carácter en un conjunto de un solo elemento
    lea si, conjunto
    mov al, caracter        ; Colocar el carácter en el conjunto
    mov [si], al
    inc si
    mov byte ptr [si], '$'  ; Terminar el string del conjunto
    ret
convertirAConjunto endp

imprimirConjunto proc near
    ; Imprimir el conjunto en formato {: 'X' :}
    Print mensajeSalida
    lea dx, conjunto
    mov ah, 09h
    int 21h
    Print finConjunto
    ret
imprimirConjunto endp

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
    call leerCaracter        ; Leer un carácter
    call convertirAConjunto  ; Convertir el carácter a conjunto
    call imprimirConjunto    ; Mostrar el conjunto resultante
    call finRutina           ; Terminar el programa

codigo ends
end main
