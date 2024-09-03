; Proyecto - Etapa 0: Rutinas de Strings
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
    stringVar db 128 dup(?)
    mensajeVacio1 db 'La entrada esta vacia.$'
    mensajeVacio2 db 'Por favor, ingrese un valor.$'
    acercaDe1 db 'Rutina de manejo para strings.$'
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

; ---------------------------------------------------------------
; Rutina para Tipo de Dato String
; ---------------------------------------------------------------
obtenerEntradaString proc near
    ; Mensaje para solicitar la entrada
    Print mensajeEntradaString

    ; Inicializar puntero y contador
    lea si, stringVar
    mov cx, 0

    leerCaracter:
        ; Leer un carácter de la entrada estándar
        mov ah, 01h
        int 21h
        mov al, ah            ; Guardar carácter en AL
        cmp al, 0Dh           ; Comparar si es "Enter"
        je finLecturaString    ; Si es Enter, termina

        ; Almacenar el carácter en el buffer de string
        mov [si], al
        inc si                ; Incrementar el puntero
        inc cx                ; Incrementar el contador de caracteres leídos
        jmp leerCaracter      ; Repetir para el siguiente carácter

    finLecturaString:
        ; Añadir el carácter nulo al final del string
        mov byte ptr [si], '$'
        ret
obtenerEntradaString endp


imprimirString proc near
    ; Imprimir el string de vuelta a la salida estándar
    lea dx, stringVar
    mov ah, 09h
    int 21h
imprimirString endP

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
    call imprimirString
    ;   ------------------------------------------------------------------
    call finRutina

codigo endS
end main
