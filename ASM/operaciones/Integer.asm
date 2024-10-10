; Proyecto - Etapa 2: Operaciones por tipo de dato
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 09/10/2024

; ###### Manual de Usuario ######

; Este programa recibe un entero por la pila
; e imprime en la pantalla las diferentes operaciones
; relacionadas con el tipo de dato en forma de bitácora o tour. 
; Se usan valores de ejemplo.

; ---------------------------------------------------------------

datos segment
    mensajeResultado db 13, 10, 'Resultado: $'
    blank db 13, 10, '$'
    acercaDe db 'Operaciones con Enteros.$'
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
; Rutinas de E/S de enteros usando la pila
; ---------------------------------------------------------------

leerArgumento proc near
    ; Leer el argumento pasado desde la línea de comandos
    mov si, 80h            ; Dirección del PSP para el argumento
    mov cl, byte ptr [si]   ; Leer la longitud del argumento
    inc si
    xor cx, cx              ; Reiniciar el valor en CX

    leerDigito:
        mov al, byte ptr [si] ; Leer un carácter del argumento
        cmp al, 20h           ; Comprobar si es espacio (terminador)
        je finLectura
        sub al, '0'           ; Convertir a número
        imul cx, 10           ; Multiplicar por 10
        add cx, ax            ; Sumar el dígito
        inc si                ; Avanzar al siguiente carácter
        dec cl                ; Reducir la longitud
        jmp leerDigito

    finLectura:
        push cx               ; Empujar el valor leído en la pila
        ret
leerArgumento endp

imprimirEntero proc near
    ; Imprimir el entero desde la pila
    pop ax                  ; Obtener el valor desde la pila
    xor cx, cx              ; Inicializar el contador

    cmp ax, 0
    jne imprimirLoop
    mov dl, '0'             ; Si el número es 0, imprimir '0'
    mov ah, 02h
    int 21h
    jmp finImpresion

    imprimirLoop:
        mov bx, 10
        xor dx, dx
        div bx              ; AX = AX / 10, residuo en DX
        push dx             ; Guardar el residuo en la pila
        inc cx              ; Contar los dígitos
        test ax, ax
        jne imprimirLoop    ; Repetir hasta que AX sea 0

    imprimirStack:
        pop dx              ; Recuperar los dígitos desde la pila
        add dl, '0'         ; Convertir a carácter
        mov ah, 02h
        int 21h
        loop imprimirStack

    finImpresion:
        ret
imprimirEntero endp

; ---------------------------------------------------------------
; Operaciones aritméticas
; ---------------------------------------------------------------

operacionSuma proc near
    ; Suma dos enteros (uno desde la pila y otro alambrado)
    pop ax                  ; Obtener el valor de la pila
    mov bx, 10              ; Número alambrado para la prueba
    add ax, bx              ; Sumar los valores
    push ax                 ; Empujar el resultado en la pila
    ret
operacionSuma endp

operacionResta proc near
    ; Resta dos enteros (uno desde la pila y otro alambrado)
    pop ax                  ; Obtener el valor de la pila
    mov bx, 10              ; Número alambrado para la prueba
    sub ax, bx              ; Restar los valores
    push ax                 ; Empujar el resultado en la pila
    ret
operacionResta endp

operacionMultiplicacion proc near
    ; Multiplica dos enteros (uno desde la pila y otro alambrado)
    pop ax                  ; Obtener el valor de la pila
    mov bx, 10              ; Número alambrado para la prueba
    mul bx                  ; Multiplicar los valores
    push ax                 ; Empujar el resultado en la pila
    ret
operacionMultiplicacion endp

operacionDivision proc near
    ; Divide dos enteros (uno desde la pila y otro alambrado)
    pop ax                  ; Obtener el valor de la pila
    mov bx, 10              ; Número alambrado para la prueba
    xor dx, dx              ; Limpiar DX para la división
    div bx                  ; Dividir AX entre BX
    push ax                 ; Empujar el cociente en la pila
    ret
operacionDivision endp

operacionResiduo proc near
    ; Calcula el residuo de la división entre dos enteros
    pop ax                  ; Obtener el valor de la pila
    mov bx, 10              ; Número alambrado para la prueba
    xor dx, dx              ; Limpiar DX para la división
    div bx                  ; Dividir AX entre BX
    push dx                 ; Empujar el residuo en la pila
    ret
operacionResiduo endp

; ---------------------------------------------------------------
; Programa principal
; ---------------------------------------------------------------

main:
    ; Inicializar segmentos
    mov ax, datos
    mov ds, ax
    mov es, ax

    mov ax, pila
    mov ss, ax

    ; Leer el argumento pasado en la línea de comandos (integer 24)
    call leerArgumento       ; Empujar el valor del argumento en la pila

    ; Realizar las operaciones
    Print mensajeResultado

    call operacionSuma        ; Suma
    call imprimirEntero

    call operacionResta       ; Resta
    call imprimirEntero

    call operacionMultiplicacion ; Multiplicación
    call imprimirEntero

    call operacionDivision    ; División
    call imprimirEntero

    call operacionResiduo     ; Residuo
    call imprimirEntero

    ; Terminar el programa
    mov ax, 4C00h
    int 21h
end main

; integer 24