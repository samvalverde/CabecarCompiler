; Proyecto - Etapa 0: Rutinas de Booleanos
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 11/09/2024

; ###### Manual de Usuario ######

; Este programa es sumamente básico ya que recibe una entrada por terminal  
; y la imprime en pantalla. Este es uno de los seis programas para los 6 tipos de datos que se
; manejarán en el proyecto de Compiladores e Intérpretes.

; ---------------------------------------------------------------

datos segment
    booleano db ?             ; Variable para almacenar el booleano (0 o 1)
    mensajeEntrada db 'Por favor, ingrese un valor booleano (0 o 1):$'
    mensajeSalida db 'El valor booleano ingresado es: $'
    mensajeVacio db 'La entrada está vacía o es inválida.$'
    acercaDe1 db 'Rutina de manejo para booleanos.$'
    acercaDe2 db 'Manejo de entrada y salida.$'
    blank db 13, 10, '$'
datos ends

; ---------------------------------------------------------------
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

obtenerEntrada proc near
    ; Solicitar entrada de un booleano
    Print mensajeEntrada
    mov ah, 01h
    int 21h
    cmp al, '0'
    je setFalse
    cmp al, '1'
    je setTrue
    call entradaVacia
    jmp finLectura

setFalse:
    mov booleano, 0
    jmp finLectura

setTrue:
    mov booleano, 1
    jmp finLectura

finLectura:
    ret
obtenerEntrada endp

imprimirBooleano proc near
    ; Imprimir el valor booleano almacenado
    Print mensajeSalida
    mov al, booleano
    add al, '0'          ; Convertir el valor a carácter ('0' o '1')
    mov dl, al
    mov ah, 02h
    int 21h
    ret
imprimirBooleano endp

entradaVacia proc near
    Print mensajeVacio
    ret
entradaVacia endp

finRutina proc near
    mov ax, 4C00h
    int 21h
    ret
finRutina endp

blankspace proc near
    Print blank
    ret
blankspace endp

prntAcercaDe proc near
    Print acercaDe1
    Print acercaDe2
    ret
prntAcercaDe endp

main:
    mov ax, ds
    mov es, ax

    mov ax, pila
    mov ss, ax

    mov ax, datos
    mov ds, ax

    ;   ------------------------------------------------------------------
    call prntAcercaDe
    ;   ------------------------------------------------------------------
    call blankspace
    ;   ------------------------------------------------------------------
    call obtenerEntrada
    ;   ------------------------------------------------------------------
    call imprimirBooleano
    ;   ------------------------------------------------------------------
    call finRutina


codigo endS
end main
