; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Járjá (Conjunto) a Wömële (Número Romano)
; Este programa toma un conjunto de caracteres y los convierte en un string.
; ---------------------------------------------------------------

datos segment
    mensajeError db 'Conjunto inválido o vacío. Resultado: 0$', 13, 10, '$'
    mensajeResultado db 'Resultado en romano: $', 13, 10, '$'
    conjunto db 12 dup('$')     ; Almacenar el conjunto de caracteres romanos
    entero dw ?                 ; Almacenar el resultado final en formato entero
    resultadoRomano db 20 dup('$') ; Almacenar el número romano resultante
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

SetToRoman proc near
    ; Inicializar
    lea si, conjunto       ; Apuntar al conjunto
    mov ax, 0              ; Inicializar el acumulador de la suma
    mov cx, 0              ; Contador de elementos válidos

    siguienteSimbolo:
        mov al, [si]
        cmp al, '$'        ; Verificar si es el final del conjunto
        je verificarResultado

        ; Convertir el símbolo a su valor si es válido
        cmp al, 'I'
        je esI
        cmp al, 'V'
        je esV
        cmp al, 'X'
        je esX
        cmp al, 'L'
        je esL
        cmp al, 'C'
        je esC
        cmp al, 'D'
        je esD
        cmp al, 'M'
        je esM

        ; Si el carácter no es válido, mostrar error y terminar
        Print mensajeError
        xor ax, ax
        mov entero, ax
        ret

        ; Mapeo de caracteres romanos válidos
        esI: add ax, 1
             inc cx
             jmp siguiente
        esV: add ax, 5
             inc cx
             jmp siguiente
        esX: add ax, 10
             inc cx
             jmp siguiente
        esL: add ax, 50
             inc cx
             jmp siguiente
        esC: add ax, 100
             inc cx
             jmp siguiente
        esD: add ax, 500
             inc cx
             jmp siguiente
        esM: add ax, 1000
             inc cx
             jmp siguiente

    siguiente:
        inc si
        jmp siguienteSimbolo

    verificarResultado:
        ; Si cx es 0, el conjunto estaba vacío
        cmp cx, 0
        jne convertirEnteroARomano

        ; Conjunto vacío, retornar 0 como resultado
        Print mensajeError
        xor ax, ax
        mov entero, ax
        ret

convertirEnteroARomano:
    ; Guardar el valor de ax en 'entero' para la conversión
    mov entero, ax
    call enteroARomano     ; Llamada para convertir el entero a romano
    ret
SetToRoman endp

; ---------------------------------------------------------------
; Conversión de Entero a Romano (ya usado en otros programas)
; ---------------------------------------------------------------
enteroARomano proc near
    mov ax, entero
    lea di, resultadoRomano    ; Apuntar al buffer para el número romano

    ; Convertir de entero a romano (siguiendo la lógica de conversión)
    cmp ax, 1000
    jl cientos
    sub ax, 1000
    mov byte ptr [di], 'M'
    inc di
    jmp enteroARomano

cientos:
    cmp ax, 900
    jl quinientos
    sub ax, 900
    mov byte ptr [di], 'C'
    mov byte ptr [di+1], 'M'
    add di, 2
    jmp enteroARomano

quinientos:
    cmp ax, 500
    jl cuatrocientos
    sub ax, 500
    mov byte ptr [di], 'D'
    inc di
    jmp enteroARomano

cuatrocientos:
    cmp ax, 400
    jl cientosMenores
    sub ax, 400
    mov byte ptr [di], 'C'
    mov byte ptr [di+1], 'D'
    add di, 2
    jmp enteroARomano

cientosMenores:
    cmp ax, 100
    jl decenas
    sub ax, 100
    mov byte ptr [di], 'C'
    inc di
    jmp enteroARomano

decenas:
    ; Continuar para convertir 50, 40, 10, 9, 5, 4, 1
    ret
enteroARomano endp

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

    ; Llamar a la conversión de conjunto a romano
    call SetToRoman

    ; Imprimir el resultado en romano
    lea dx, resultadoRomano
    mov ah, 09h
    int 21h

    ; Terminar el programa
    mov ax, 4C00h
    int 21h
codigo ends
end main