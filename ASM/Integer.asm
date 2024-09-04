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
    entero dw ?             ; Se amplía a 16 bits para manejar números grandes
    mensajeVacio1 db 'La entrada esta vacia.$'
    mensajeVacio2 db 'Por favor, ingrese un valor.$'
    acercaDe1 db 'Rutina de manejo para enteros.$'
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

    assume cs:codigo, ds:datos, ss:pila ;se asignan los segmentos

obtenerEntrada proc near
    ; Inicializar el acumulador a 0
    mov cx, 0               ; CX almacenará el número
    leerDigito:
        mov ah, 01h         ; Leer un carácter
        int 21h
        cmp al, 0Dh         ; Comprobar si se presionó "Enter"
        je finLectura
        sub al, '0'         ; Convertir el carácter a valor numérico
        mov bx, 10          ; BX = 10
        mul bx              ; AX = AX * 10
        add cx, ax          ; Sumar el dígito al acumulador
        jmp leerDigito      ; Leer el siguiente dígito

    finLectura:
        mov entero, cx      ; Guardar el resultado en la variable "entero"
        ret
obtenerEntrada endp

imprimirEntero proc near
    ; Imprimir el valor entero almacenado
    mov ax, entero          ; Cargar el valor de "entero" en AX
    xor cx, cx              ; Inicializar CX a 0

    cmp ax, 0               ; Comprobar si el número es 0
    jne imprimirLoop        ; Si no es 0, continuar con la conversión
    mov dl, '0'             ; Si es 0, mostrarlo directamente
    mov ah, 02h
    int 21h
    jmp finImpresion

    imprimirLoop:
        mov bx, 10
        xor dx, dx          ; DX:AX = AX / 10
        div bx              ; Dividir AX por 10, resultado en AX, residuo en DX
        add dl, '0'         ; Convertir el residuo en carácter
        push dx             ; Guardar el dígito en la pila
        inc cx              ; Incrementar el contador de dígitos
        test ax, ax         ; Comprobar si AX es 0
        jne imprimirLoop    ; Si no es 0, repetir

    imprimirStack:
        pop dx              ; Recuperar el dígito desde la pila
        mov ah, 02h         ; Imprimir el dígito
        int 21h
        loop imprimirStack  ; Repetir para todos los dígitos

    finImpresion:
        ret
imprimirEntero endp

entradaVacia proc near
    Print mensajeVacio1
    Print mensajeVacio2
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
    call imprimirEntero
    ;   ------------------------------------------------------------------
    call finRutina

codigo ends
end main
