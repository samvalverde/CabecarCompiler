; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Shéj (String) a Shtáwa (Entero)
; Este programa toma un string que representa un número y lo convierte en un valor entero.
; ---------------------------------------------------------------

datos segment
    stringVar db '1234$', 0          ; String que representa un número
    entero dw ?                      ; Variable para almacenar el valor entero
    mensajeEntrada db 'El string es: $'
    mensajeSalida db 'El valor entero es: $'
    mensajeError db 'String no válido.$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Shéj (String) a Shtáwa (Entero).$'
    acercaDe2 db 'El string se convierte en un valor numérico.$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

leerString proc near
    ; Mostrar el string almacenado
    Print mensajeEntrada
    lea dx, stringVar
    mov ah, 09h
    int 21h
    ret
leerString endp

convertirAEntero proc near
    ; Convertir el string en un número entero
    lea si, stringVar
    xor ax, ax                  ; Inicializar el acumulador de valor numérico
    xor bx, bx                  ; Inicializar el valor total (en BX)
    mov cx, 0                   ; Contador para limitar el número de caracteres

convertirLoop:
    mov al, [si]
    cmp al, '$'                 ; Verificar si se alcanzó el final del string
    je finConversion
    sub al, '0'                 ; Convertir el carácter a valor numérico
    cmp al, 9
    jae error                   ; Si el valor no es un dígito válido
    imul bx, 10                 ; Multiplicar el acumulador por 10
    add bx, ax                  ; Sumar el dígito al acumulador
    inc si                      ; Avanzar al siguiente carácter en el string
    inc cx                      ; Incrementar el contador de caracteres
    cmp cx, 5                   ; Limitar el tamaño del número a 5 dígitos
    jne convertirLoop

finConversion:
    mov entero, bx              ; Almacenar el valor numérico en la variable "entero"
    ret

error:
    Print mensajeError
    ret
convertirAEntero endp

imprimirEntero proc near
    ; Imprimir el valor entero convertido desde el string
    Print mensajeSalida
    mov ax, entero
    call imprimirNumero         ; Llamar a la rutina para imprimir números
    ret
imprimirEntero endp

imprimirNumero proc near
    ; Imprimir el valor entero en formato de dígitos
    xor cx, cx                  ; Inicializar el contador
    cmp ax, 0
    jne imprimirLoop
    mov dl, '0'                 ; Si el número es 0, mostrarlo directamente
    mov ah, 02h
    int 21h
    jmp finImpresion

    imprimirLoop:
        mov bx, 10
        xor dx, dx
        div bx                  ; AX = AX / 10, residuo en DX
        push dx                 ; Guardar el residuo (dígito)
        inc cx                  ; Incrementar el contador de dígitos
        test ax, ax
        jne imprimirLoop        ; Repetir mientras AX no sea 0

    imprimirStack:
        pop dx
        add dl, '0'             ; Convertir el dígito en carácter
        mov ah, 02h
        int 21h
        loop imprimirStack      ; Repetir para todos los dígitos

    finImpresion:
        ret
imprimirNumero endp

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
    call leerString            ; Leer el string de caracteres
    call convertirAEntero      ; Convertir el string a entero
    call imprimirEntero        ; Mostrar el valor entero resultante
    call finRutina             ; Terminar el programa

codigo ends
end main
