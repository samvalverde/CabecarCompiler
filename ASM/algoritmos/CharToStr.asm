; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Éka (Carácter) a Shéj (String)
; Este programa toma un carácter y lo convierte en un string
; de un solo carácter.
; ---------------------------------------------------------------

datos segment
    caracter db ?             ; Variable para almacenar el carácter
    string db 2 dup ('$')     ; Buffer para almacenar el string del carácter
    mensajeEntrada db 'Ingrese un carácter: $'
    mensajeSalida db 'El string correspondiente es: $'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Éka (Carácter) a Shéj (String).$'
    acercaDe2 db 'El carácter se convierte en un string de un solo carácter.$'
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

convertirAString proc near
    ; Convertir el carácter en un string de un solo carácter
    mov al, caracter
    mov byte ptr [string], al  ; Almacenar el carácter en el string
    ret
convertirAString endp

imprimirString proc near
    ; Imprimir el string correspondiente
    Print mensajeSalida
    lea dx, string
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
    call leerCaracter       ; Leer un carácter
    call convertirAString   ; Convertir el carácter a string
    call imprimirString     ; Mostrar el string correspondiente
    call finRutina          ; Terminar el programa

codigo ends
end main
