; Proyecto - Rutinas de Números Romanos
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
    random db ?
    msgAcercaDe db 'Rutina de manejo para valores random (aleatorio).$'
    msgGenerado db 'Numero aleatorio generado: $'
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

generarRandom proc near
    ; Genera un número aleatorio y lo guarda en random
    mov ah, 2Ch             ; Llamada a la función para obtener el reloj del sistema
    int 21h                 ; Interrupt 21h, función 2Ch - hora del sistema
    mov al, dh              ; Usa el valor de los segundos para generar el aleatorio
    mov random, al          ; Guarda el número generado en random
    ret
generarRandom endP

imprimirRandom proc near
    ; Convierte y muestra el valor random generado
    mov al, random
    ; Convertir el valor a un carácter imprimible (0-9)
    and al, 0Fh             ; Limita el valor entre 0-15 (para evitar caracteres no imprimibles)
    add al, '0'             ; Convierte a carácter numérico
    mov ah, 02h
    int 21h
    ret
imprimirRandom endP

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
    Print msgAcercaDe
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
    call generarRandom
    ;   ------------------------------------------------------------------
    Print msgGenerado
    call imprimirRandom
    ;   ------------------------------------------------------------------
    call finRutina

codigo endS
end main
