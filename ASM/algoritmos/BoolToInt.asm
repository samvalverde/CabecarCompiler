; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: YínaKulíwa (Booleano) a Shtáwa (Entero)
; Este programa toma un valor booleano y lo convierte en un número
; entero. Falso (kóyuwé) se convierte en 0 y Verdadero (síwa) en 1.
; ---------------------------------------------------------------

datos segment
    booleano db ?              ; Variable para almacenar el booleano
    entero dw ?                ; Variable para almacenar el entero
    mensajeEntrada db 'Ingrese un valor booleano (0 = kóyuwé, 1 = síwa): $'
    mensajeSalida db 'El valor entero es: $'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de YínaKulíwa (Booleano) a Shtáwa (Entero).$'
    acercaDe2 db 'El booleano se convierte en un entero.$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

leerBooleano proc near
    ; Solicitar el valor booleano al usuario
    Print mensajeEntrada
    mov ah, 01h
    int 21h
    sub al, '0'               ; Convertir el carácter a numérico
    mov booleano, al
    ret
leerBooleano endp

convertirAEntero proc near
    ; Convertir el booleano en un número entero (0 o 1)
    cmp booleano, 1
    je esVerdadero

    ; Si es falso, el entero será 0
    mov entero, 0
    ret

esVerdadero:
    ; Si es verdadero, el entero será 1
    mov entero, 1
    ret
convertirAEntero endp

imprimirEntero proc near
    ; Imprimir el valor entero (0 o 1)
    Print mensajeSalida
    mov ax, entero
    call imprimirNumero       ; Llamar a la rutina para imprimir números
    ret
imprimirEntero endp

imprimirNumero proc near
    ; Imprimir el valor entero en formato de dígitos
    xor cx, cx                ; Inicializar el contador
    cmp ax, 0
    jne imprimirLoop
    mov dl, '0'               ; Si el número es 0, mostrarlo directamente
    mov ah, 02h
    int 21h
    jmp finImpresion

    imprimirLoop:
        mov bx, 10
        xor dx, dx
        div bx                ; AX = AX / 10, residuo en DX
        push dx               ; Guardar el residuo (dígito)
        inc cx                ; Incrementar el contador de dígitos
        test ax, ax
        jne imprimirLoop      ; Repetir mientras AX no sea 0

    imprimirStack:
        pop dx
        add dl, '0'           ; Convertir el dígito en carácter
        mov ah, 02h
        int 21h
        loop imprimirStack    ; Repetir para todos los dígitos

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
    call leerBooleano          ; Leer el valor booleano
    call convertirAEntero      ; Convertir el booleano a entero
    call imprimirEntero        ; Mostrar el valor entero
    call finRutina             ; Terminar el programa

codigo ends
end main
