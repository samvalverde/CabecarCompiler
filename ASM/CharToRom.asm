; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Éka (Carácter) a Wömële (Número Romano)
; Este programa toma un carácter y lo convierte en su valor numérico
; si es un símbolo romano válido. Si no es válido, muestra un error.
; ---------------------------------------------------------------

datos segment
    caracter db ?             ; Variable para almacenar el carácter
    valorRomano dw ?          ; Variable para almacenar el valor del número romano
    mensajeEntrada db 'Ingrese un carácter romano (I, V, X, L, C, D, M): $'
    mensajeSalida db 'El valor del número romano es: $'
    mensajeError db 'Carácter no es un número romano válido.$'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Éka (Carácter) a Wömële (Número Romano).$'
    acercaDe2 db 'Convierte un carácter romano en su valor numérico.$'
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
    ; Solicitar el carácter romano al usuario
    Print mensajeEntrada
    mov ah, 01h             ; Leer un carácter
    int 21h
    mov caracter, al        ; Guardar el carácter leído
    ret
leerCaracter endp

convertirARomano proc near
    ; Convertir el carácter romano a su valor numérico
    cmp caracter, 'I'
    je esI
    cmp caracter, 'V'
    je esV
    cmp caracter, 'X'
    je esX
    cmp caracter, 'L'
    je esL
    cmp caracter, 'C'
    je esC
    cmp caracter, 'D'
    je esD
    cmp caracter, 'M'
    je esM

    ; Si no es un número romano válido, mostrar error
    Print mensajeError
    jmp finConversion

esI:
    mov ax, 1
    jmp guardarValor

esV:
    mov ax, 5
    jmp guardarValor

esX:
    mov ax, 10
    jmp guardarValor

esL:
    mov ax, 50
    jmp guardarValor

esC:
    mov ax, 100
    jmp guardarValor

esD:
    mov ax, 500
    jmp guardarValor

esM:
    mov ax, 1000
    jmp guardarValor

guardarValor:
    mov valorRomano, ax      ; Guardar el valor del número romano
    ret

finConversion:
    ret
convertirARomano endp

imprimirValorRomano proc near
    ; Imprimir el valor del número romano
    Print mensajeSalida
    mov ax, valorRomano
    call imprimirNumero     ; Llamar a la rutina para imprimir números
    ret
imprimirValorRomano endp

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
    call leerCaracter       ; Leer un carácter romano
    call convertirARomano   ; Convertir el carácter a número romano
    call imprimirValorRomano ; Mostrar el valor numérico correspondiente
    call finRutina          ; Terminar el programa

codigo ends
end main
