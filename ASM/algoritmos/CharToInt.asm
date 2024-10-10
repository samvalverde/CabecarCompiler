; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Éka (Carácter) a Shtáwa (Entero)
; Este programa toma un carácter y lo convierte en su valor
; numérico correspondiente al código ASCII.
; ---------------------------------------------------------------

datos segment
    caracter db ?             ; Variable para almacenar el carácter
    entero dw ?               ; Variable para almacenar el código ASCII
    mensajeEntrada db 'Ingrese un carácter: $'
    mensajeSalida db 'El valor entero (ASCII) es: $'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Éka (Carácter) a Shtáwa (Entero).$'
    acercaDe2 db 'El carácter se convierte en su valor ASCII.$'
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
codigo segment
    assume cs:codigo, ds:datos, ss:pila

leerCaracter proc near
    ; Solicitar el carácter al usuario
    Print mensajeEntrada
    mov ah, 01h             ; Leer un carácter
    int 21h
    mov caracter, al        ; Guardar el carácter leído
    ret
leerCaracter endp

convertirAEntero proc near
    ; Convertir el carácter a su valor ASCII
    mov al, caracter
    xor ah, ah              ; Asegurarse de que el registro AX sea 16 bits
    mov entero, ax          ; Guardar el código ASCII en "entero"
    ret
convertirAEntero endp

imprimirEntero proc near
    ; Imprimir el valor entero (ASCII)
    Print mensajeSalida
    mov ax, entero          ; Cargar el valor entero
    call imprimirNumero     ; Llamar a la rutina para imprimir números
    ret
imprimirEntero endp

imprimirNumero proc near
    ; Imprimir el valor entero en formato de dígitos
    xor cx, cx              ; Inicializar el contador
    cmp ax, 0
    jne imprimirLoop
    mov dl, '0'             ; Si el número es 0, mostrarlo directamente
    mov ah, 02h
    int 21h
    jmp finImpresion

    imprimirLoop:
        mov bx, 10
        xor dx, dx
        div bx              ; AX = AX / 10, residuo en DX
        push dx             ; Guardar el residuo (dígito)
        inc cx              ; Incrementar el contador de dígitos
        test ax, ax
        jne imprimirLoop    ; Repetir mientras AX no sea 0

    imprimirStack:
        pop dx
        add dl, '0'         ; Convertir el dígito en carácter
        mov ah, 02h
        int 21h
        loop imprimirStack  ; Repetir para todos los dígitos

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
    call leerCaracter       ; Leer un carácter
    call convertirAEntero   ; Convertir el carácter a entero (ASCII)
    call imprimirEntero     ; Mostrar el valor entero
    call finRutina          ; Terminar el programa

codigo ends
end main
