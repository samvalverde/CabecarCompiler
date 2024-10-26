datos segment
    entero dw ?                     ; Variable para almacenar el entero procesado
    numeroPredefinido dw 10         ; Número con el que se comparará el argumento
    mensajeResultado db 13, 10, 'Resultado: $'
    mensajeTamaño db 'Longitud de entero: $', 13, 10, '$'
    mensajeSuma db 13, 10, 'Resultado de la suma: $'
    mensajeResta db 13, 10, 'Resultado de la resta: $'
    mensajeMultiplicacion db 13, 10, 'Resultado de la multiplicación: $'
    mensajeDivision db 13, 10, 'Resultado de la división: $'
    mensajeResiduo db 13, 10, 'Resultado del residuo: $'
    mensajeMayor db 'Resultado de > : $', 13, 10, '$'
    mensajeMenor db 'Resultado de < : $', 13, 10, '$'
    mensajeIgual db 'Resultado de = : $', 13, 10, '$'
    mensajeMayorIgual db 'Resultado de >= : $', 13, 10, '$'
    mensajeMenorIgual db 'Resultado de <= : $', 13, 10, '$'
    mensajeDiferente db 'Resultado de >< : $', 13, 10, '$'
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
    mov si, 80h            ; Dirección del PSP para el argumento
    mov cl, byte ptr [si]  ; Leer la longitud del argumento
    inc si
    xor cx, cx             ; Reiniciar el valor en CX

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
        mov entero, cx        ; Almacenar en `entero` para las operaciones
        push cx               ; Empujar el valor en la pila
        ret
leerArgumento endp

imprimirEntero proc near
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
; Operaciones Aritméticas y Comparación
; ---------------------------------------------------------------

operacionSuma proc near
    pop ax
    mov bx, numeroPredefinido
    add ax, bx
    push ax
    Print mensajeSuma
    ret
operacionSuma endp

operacionResta proc near
    pop ax
    mov bx, numeroPredefinido
    sub ax, bx
    push ax
    Print mensajeResta
    ret
operacionResta endp

operacionMultiplicacion proc near
    pop ax
    mov bx, numeroPredefinido
    mul bx
    push ax
    Print mensajeMultiplicacion
    ret
operacionMultiplicacion endp

operacionDivision proc near
    pop ax
    mov bx, numeroPredefinido
    xor dx, dx
    div bx
    push ax
    Print mensajeDivision
    ret
operacionDivision endp

operacionResiduo proc near
    pop ax
    mov bx, numeroPredefinido
    xor dx, dx
    div bx
    push dx
    Print mensajeResiduo
    ret
operacionResiduo endp

mayor proc near
    pop ax
    cmp ax, numeroPredefinido
    jg esMayor
    Print mensajeMenor
    ret

esMayor:
    Print mensajeMayor
    ret
mayor endp

menor proc near
    pop ax
    cmp ax, numeroPredefinido
    jl esMenor
    Print mensajeMayorIgual
    ret

esMenor:
    Print mensajeMenor
    ret
menor endp

igual proc near
    pop ax
    cmp ax, numeroPredefinido
    je esIgual
    Print mensajeDiferente
    ret

esIgual:
    Print mensajeIgual
    ret
igual endp

mayorIgual proc near
    pop ax
    cmp ax, numeroPredefinido
    jge esMayorIgual
    Print mensajeMenor
    ret

esMayorIgual:
    Print mensajeMayorIgual
    ret
mayorIgual endp

menorIgual proc near
    pop ax
    cmp ax, numeroPredefinido
    jle esMenorIgual
    Print mensajeMayor
    ret

esMenorIgual:
    Print mensajeMenorIgual
    ret
menorIgual endp

diferente proc near
    pop ax
    cmp ax, numeroPredefinido
    jne esDiferente
    Print mensajeIgual
    ret

esDiferente:
    Print mensajeDiferente
    ret
diferente endp

BikoInteger proc near
    mov ax, entero           ; Cargar el entero almacenado
    xor cx, cx               ; Inicializar el contador de dígitos

contarDigitos:
    cmp ax, 0
    je finBikoInteger
    mov dx, 0
    div word ptr 10          ; Dividir AX por 10
    inc cx                   ; Incrementar el contador de dígitos
    jmp contarDigitos

finBikoInteger:
    mov ax, cx               ; Guardar el número de dígitos en AX
    Print mensajeTamaño
    ret
BikoInteger endp

; ---------------------------------------------------------------
; Programa principal
; ---------------------------------------------------------------

main:
    mov ax, datos
    mov ds, ax
    mov es, ax

    mov ax, pila
    mov ss, ax

    ; Leer el argumento pasado en la línea de comandos (integer 24)
    call leerArgumento       ; Empujar el valor del argumento en la pila

    Print acercaDe
    Print blank

    ; Realizar y mostrar resultados de las operaciones
    call operacionSuma
    call imprimirEntero
    call operacionResta
    call imprimirEntero
    call operacionMultiplicacion
    call imprimirEntero
    call operacionDivision
    call imprimirEntero
    call operacionResiduo
    call imprimirEntero

    ; Comparaciones
    call mayor
    call menor
    call igual
    call mayorIgual
    call menorIgual
    call diferente

    call BikoInteger

    ; Terminar el programa
    mov ax, 4C00h
    int 21h
end main
