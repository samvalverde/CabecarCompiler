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
    string db 20 dup('$')
    mensajeVacio1 db 'La entrada esta vacia.$'
    mensajeVacio2 db 'Por favor, ingrese un valor.$'
    acercaDe1 db 'Rutina de manejo para strings.$'
    acercaDe2 db 'Manejo de entrada y salida.$'
    blank db 13, 10, '$'
    mensajeEntrada db 'Ingrese un string: $'
    mensajeSalida db 'String recibido: $'
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

    assume cs:codigo, ds:datos, ss:pila ;se asignan los segmentos

; Procedimiento para imprimir el mensaje "Acerca de"
prntAcercaDe proc near
    Print acercaDe1
    Print acercaDe2
    ret
prntAcercaDe endP

; Procedimiento para manejar entradas vacías
entradaVacia proc near
    Print mensajeVacio1
    Print mensajeVacio2
    ret
entradaVacia endP

; Procedimiento para obtener la entrada del usuario
obtenerEntrada proc near
    ; Mostrar mensaje de solicitud de entrada
    Print mensajeEntrada

    ; Inicializar puntero al buffer y contador
    lea si, string
    mov cx, 0

    leerCaracter:
        ; Leer un carácter de la entrada estándar
        mov ah, 01h
        int 21h
        cmp al, 0Dh           ; Comprobar si es "Enter" (código ASCII 13)
        je finLectura         ; Si es "Enter", terminar la lectura

        ; Almacenar el carácter en el buffer
        mov [si], al
        inc si                ; Incrementar el puntero
        inc cx                ; Incrementar el contador de caracteres leídos
        cmp cx, 19            ; Verificar que no se exceda el buffer
        jae finLectura        ; Si se excede, terminar la lectura
        jmp leerCaracter      ; Repetir para el siguiente carácter

    empty:
        ; Mostrar mensaje de entrada vacía
        call entradaVacia
        ret
    
    finLectura:
        ; Añadir el carácter de fin de cadena '$'
        mov byte ptr [si], '$'
        ; Verificar si la entrada está vacía
        lea si, string
        mov al, [si]
        cmp al, '$'           ; Comprobar si el primer carácter es '$' (sin entrada)
        je empty       ; Si es '$', mostrar mensaje de entrada vacía
        ret
obtenerEntrada endP

; Procedimiento para imprimir el número romano almacenado
imprimirString proc near
    ; Mostrar mensaje de salida
    Print mensajeSalida

    ; Imprimir el string almacenado en el buffer
    lea dx, string
    mov ah, 09h
    int 21h
    ret
imprimirString endP

; Procedimiento para imprimir una línea en blanco
blankspace proc near
    Print blank
    ret
blankspace endP

; Procedimiento para finalizar el programa
finRutina proc near
    mov ax, 4C00h
    int 21h
    ret
finRutina endP

; Procedimiento principal del programa
main:
    ; Inicialización de segmentos
    mov ax, ds
    mov es, ax

    mov ax, pila
    mov ss, ax

    mov ax, datos
    mov ds, ax

    ; ------------------------------------------------------------------
    call prntAcercaDe
    ; ------------------------------------------------------------------
    call blankspace
    ; ------------------------------------------------------------------
    call obtenerEntrada
    ; ------------------------------------------------------------------
    call imprimirString
    ; ------------------------------------------------------------------
    call finRutina

codigo endS
end main

; ---------------------------------------------------------------