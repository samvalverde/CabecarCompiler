; Proyecto - Etapa 0: Rutinas de Enteros
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 11/09/2024

; ###### Manual de Usuario ######

; Este programa es sumamente básico ya que recibe una entrada por terminal  
; y la imprime en pantalla. Esto funciona para los 6 tipos de datos que se
; manejarán en el proyecto de Compiladores e Intérpretes.

; ---------------------------------------------------------------
datos segment
    entero db ?
    mensajeVacio1 db 'La entrada esta vacia.$'
    mensajeVacio2 db 'Por favor, ingrese un valor.$'
    acercaDe1 db 'Rutina de manejo para enteros.$'
    acercaDe2 db 'Manejo de entrada y salida.$'
    blank db 13, 10, '$'
datos endS

; ---------------------------------------------------------------
pila segment stack 'stack'
    dw 256 dup (?)
pila endS

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

; ---------------------------------------------------------------
codigo segment

    assume cs:codigo, ds:datos, ss:pila ;se asignan los segmentos

obtenerEntrada proc near
    ; Procesamiento de la entrada de un entero
    mov ah, 01h
    int 21h
    sub al, '0'
    mov entero, al
    ret
obtenerEntrada endP

imprimirEntero proc near
    ; Convierte y muestra el valor entero almacenado
    mov al, entero
    add al, '0'
    mov ah, 02h
    int 21h
    ret
imprimirEntero endP

entradaVacia proc near
    Print mensajeVacio1
    Print mensajeVacio2
    ret
entradaVacia endP

finRutina proc near
    mov ax, 4C00h
    int 21h
    ret
finRutina endP

blankspace proc near
    Print blank
    ret
blankspace endP

prntAcercaDe proc near
    Print acercaDe1
    Print acercaDe2
    ret
prntAcercaDe endP

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
    call imprimirEntero
    ;   ------------------------------------------------------------------
    call finRutina

codigo endS
end main
