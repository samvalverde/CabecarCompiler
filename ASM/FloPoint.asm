; Proyecto - Etapa 0: Rutinas de Puntos Flotante
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
    buffer db 20 dup (?)
    entero dw 0
    fraccion db 10 dup (?)
    msgAcercaDe db 'Rutina de manejo para punto flotante.$'
    msgEntrada db 'Ingrese un numero de punto flotante: $'
    msgResultado db 13, 10, 'El numero es: $'
    msgFin db 13, 10, '$'
    msgError db 'Error en la entrada.$'
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
    ; Solicita al usuario ingresar un número de punto flotante
    mov ah, 09h
    lea dx, msgEntrada
    int 21h

    ; Lee la entrada del usuario
    mov ah, 0Ah
    lea dx, buffer
    int 21h
    ret
obtenerEntrada endP

convertirAFloat proc near
    ; Convierte la cadena en buffer a un número de punto flotante
    finit                          ; Inicializa la FPU
    
    ; Convertir la parte entera
    lea si, buffer + 2             ; Saltar el byte de longitud
    xor cx, cx                     ; Inicializar contador de dígitos
    xor ax, ax                     ; Inicializar acumulador

    convertirEntera:
        lodsb                      ; Cargar el siguiente carácter
        cmp al, '.'                ; Ver si es el punto decimal
        je convertirFraccionaria   ; Saltar a la parte fraccionaria si se encuentra el punto
        sub al, '0'                ; Convertir el carácter a dígito
        mul cx                     ; Multiplicar el resultado anterior por 10
        add ax, cx                 ; Añadir el nuevo dígito
        jmp convertirEntera

    convertirFraccionaria:
        fstp st(0)                 ; Almacenar la parte entera en la FPU
        xor cx, cx                 ; Reiniciar el contador de dígitos
        xor bx, bx                 ; Inicializar acumulador para la parte fraccionaria

    fraccion_loop:
        lodsb                      ; Cargar el siguiente carácter
        cmp al, 0Dh                ; Comprobar si es el final de la cadena (Enter)
        je finalizar_conversion    ; Salir si es el final
        sub al, '0'                ; Convertir el carácter a dígito
        push ax                    ; Guardar el dígito en la pila
        mov ax, 10                 ; Preparar multiplicador
        mul cx                     ; Multiplicar por 10^cx (desplazar a la derecha)
        add bx, ax                 ; Añadir al acumulador fraccionario
        inc cx                     ; Incrementar el contador de posiciones decimales
        jmp fraccion_loop          ; Continuar con el siguiente dígito

    finalizar_conversion:
        ; Dividir la parte fraccionaria por 10^cx
        mov ax, 10
        xor dx, dx
        div cx
        ; Añadir la parte fraccionaria al número en la FPU
        fld bx
        fdiv cx
        fadd st(0), st(1)          ; Añadir la parte fraccionaria a la parte entera
        ret
convertirAFloat endP

imprimirPuntoFlotante proc near
    ; Muestra el número de punto flotante en pantalla

    ; Convertir el valor de la FPU a una cadena
    sub sp, 20                     ; Reservar espacio en la pila
    fld st(0)                      ; Cargar el número flotante desde la FPU
    fistp dword ptr [sp]           ; Convertir el número a entero y almacenarlo en la pila
    lea dx, [sp]                   ; Apuntar a la cadena resultante
    call print_number              ; Llamar a la rutina de impresión
    add sp, 20                     ; Restaurar la pila

    ret
imprimirPuntoFlotante endP

entradaVacia proc near
    Print msgError
    ret
entradaVacia endP

finRutina proc near
    mov ax, 4C00h
    int 21h
    ret
finRutina endP

blankspace proc near
    Print msgFin
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
    call obtenerEntrada
    ;   ------------------------------------------------------------------
    call convertirAFloat
    ;   ------------------------------------------------------------------
    call imprimirPuntoFlotante
    ;   ------------------------------------------------------------------
    call finRutina

codigo endS
end main
