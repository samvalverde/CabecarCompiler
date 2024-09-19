; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Wömële (Número Romano) a Járjá (Conjunto)
; Este programa toma un número romano válido y lo convierte en un conjunto
; de caracteres que representan los dígitos del valor numérico.
; ---------------------------------------------------------------

datos segment
    numeroRomano db 'XLIX$', 0  ; Número romano de entrada
    valorNumerico dw ?          ; Variable para almacenar el valor numérico
    conjunto db 12 dup ('$')    ; Arreglo para almacenar el conjunto de dígitos
    mensajeEntrada db 'Ingrese un número romano: $'
    mensajeSalida db 'El conjunto resultante es: {: ', 0
    finConjunto db ' :}$', 0
    mensajeError db 'Número romano no válido.$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Wömële (Número Romano) a Járjá (Conjunto).$'
    acercaDe2 db 'El número romano se convierte en un conjunto.$'
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

convertirARomano proc near
    ; Convertir el número romano en su valor numérico
    lea si, numeroRomano       ; Puntero al número romano
    xor ax, ax                 ; Inicializar el acumulador de valor numérico
    mov valorNumerico, 0

    nextSymbol:
        mov al, [si]           ; Leer un símbolo del número romano
        cmp al, '$'            ; Verificar si es el final del string
        je finConversion

        cmp al, 'I'
        je agregarI
        cmp al, 'V'
        je agregarV
        cmp al, 'X'
        je agregarX
        cmp al, 'L'
        je agregarL
        cmp al, 'C'
        je agregarC
        cmp al, 'D'
        je agregarD
        cmp al, 'M'
        je agregarM

        ; Si encuentra un símbolo no válido, mostrar error
        Print mensajeError
        jmp finConversion

    agregarI:
        cmp byte ptr [si+1], 'V'
        je agregarMenosI       ; Si "IV", agregar 4
        cmp byte ptr [si+1], 'X'
        je agregarMenosI       ; Si "IX", agregar 9
        add valorNumerico, 1    ; Si es "I", agregar 1
        jmp siguienteSimbolo

    agregarMenosI:
        add valorNumerico, 9    ; Para IX
        jmp siguienteSimbolo2

    agregarV:
        add valorNumerico, 5    ; Para V
        jmp siguienteSimbolo

    agregarX:
        cmp byte ptr [si+1], 'L'
        je agregarMenosX
        cmp byte ptr [si+1], 'C'
        je agregarMenosX
        add valorNumerico, 10   ; Para X
        jmp siguienteSimbolo

    agregarMenosX:
        add valorNumerico, 40   ; Para XL o XC
        jmp siguienteSimbolo2

    agregarL:
        add valorNumerico, 50   ; Para L
        jmp siguienteSimbolo

    agregarC:
        cmp byte ptr [si+1], 'D'
        je agregarMenosC
        cmp byte ptr [si+1], 'M'
        je agregarMenosC
        add valorNumerico, 100  ; Para C
        jmp siguienteSimbolo

    agregarMenosC:
        add valorNumerico, 400  ; Para CD o CM
        jmp siguienteSimbolo2

    agregarD:
        add valorNumerico, 500  ; Para D
        jmp siguienteSimbolo

    agregarM:
        add valorNumerico, 1000 ; Para M
        jmp siguienteSimbolo

    siguienteSimbolo:
        inc si                  ; Avanzar al siguiente símbolo
        jmp nextSymbol

    siguienteSimbolo2:
        add si, 2               ; Avanzar dos posiciones en caso de símbolos como IX o XL
        jmp nextSymbol

finConversion:
    ret
convertirARomano endp

convertirAConjunto proc near
    ; Convertir el valor numérico en un conjunto de dígitos
    mov ax, valorNumerico
    lea di, conjunto
    xor cx, cx              ; Inicializar el contador de caracteres

convertirLoop:
    mov bx, 10
    xor dx, dx
    div bx                  ; Dividir el número por 10
    add dl, '0'             ; Convertir el dígito en carácter ASCII
    push dx                 ; Guardar el dígito en la pila
    inc cx                  ; Incrementar el contador de dígitos
    test ax, ax
    jne convertirLoop       ; Repetir mientras el número no sea 0

imprimirStack:
    pop dx
    mov [di], dl            ; Colocar los dígitos en el conjunto
    inc di
    loop imprimirStack      ; Repetir hasta que se hayan impreso todos los dígitos

    mov byte ptr [di], '$'   ; Terminar el string del conjunto
    ret
convertirAConjunto endp

imprimirConjunto proc near
    ; Imprimir el conjunto en formato {: '1', '4' :} (por ejemplo)
    Print mensajeSalida
    lea dx, conjunto
    mov ah, 09h
    int 21h
    Print finConjunto
    ret
imprimirConjunto endp

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
    call convertirARomano     ; Convertir el número romano a valor numérico
    call convertirAConjunto   ; Convertir el valor numérico a conjunto
    call imprimirConjunto     ; Mostrar el conjunto resultante
    call finRutina            ; Terminar el programa

codigo ends
end main
