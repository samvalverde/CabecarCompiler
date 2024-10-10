; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Wömële (Número Romano) a Shtáwa (Entero)
; Este programa toma un número romano válido y lo convierte en su valor numérico.
; ---------------------------------------------------------------

datos segment
    numeroRomano db 'XLIX$', 0    ; Número romano de entrada
    valorNumerico dw ?            ; Variable para almacenar el valor numérico
    mensajeEntrada db 'Ingrese un número romano: $'
    mensajeSalida db 'El valor entero es: $'
    mensajeError db 'Número romano no válido.$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Wömële (Número Romano) a Shtáwa (Entero).$'
    acercaDe2 db 'El número romano se convierte en un valor numérico.$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

leerNumeroRomano proc near
    ; Solicitar el número romano al usuario
    Print mensajeEntrada
    lea si, numeroRomano         ; Leer directamente desde la variable (como ejemplo)
    ret
leerNumeroRomano endp

convertirANumero proc near
    ; Convertir el número romano en su valor numérico
    lea si, numeroRomano         ; Puntero al número romano
    xor ax, ax                   ; Inicializar el acumulador de valor numérico
    mov valorNumerico, 0

    nextSymbol:
        mov al, [si]             ; Leer un símbolo del número romano
        cmp al, '$'              ; Verificar si es el final del string
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
convertirANumero endp

imprimirEntero proc near
    ; Imprimir el valor entero convertido desde el número romano
    Print mensajeSalida
    mov ax, valorNumerico
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
    call leerNumeroRomano      ; Leer el número romano
    call convertirANumero       ; Convertir el número romano a entero
    call imprimirEntero         ; Mostrar el valor entero
    call finRutina              ; Terminar el programa

codigo ends
end main
