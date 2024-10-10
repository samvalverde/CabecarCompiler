; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Wömële (Número Romano) a Shéj (String)
; Este programa toma un número romano válido, lo convierte en un valor numérico,
; y luego convierte ese valor numérico en un string.
; ---------------------------------------------------------------

datos segment
    numeroRomano db 'XCIV$', 0    ; Número romano de entrada
    valorNumerico dw ?            ; Variable para almacenar el valor numérico
    stringVar db 6 dup ('$')      ; Arreglo para almacenar el string resultante
    mensajeEntrada db 'Ingrese un número romano: $'
    mensajeSalida db 'El valor string es: $'
    mensajeError db 'Número romano no válido.$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Wömële (Número Romano) a Shéj (String).$'
    acercaDe2 db 'El número romano se convierte en un string.$'
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
    lea si, numeroRomano
    ret
leerNumeroRomano endp

convertirANumero proc near
    ; Convertir el número romano en su valor numérico (misma lógica que antes)
    lea si, numeroRomano
    xor ax, ax
    mov valorNumerico, 0

    nextSymbol:
        mov al, [si]
        cmp al, '$'
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

        Print mensajeError
        jmp finConversion

    agregarI:
        cmp byte ptr [si+1], 'V'
        je agregarMenosI
        cmp byte ptr [si+1], 'X'
        je agregarMenosI
        add valorNumerico, 1
        jmp siguienteSimbolo

    agregarMenosI:
        add valorNumerico, 9
        jmp siguienteSimbolo2

    agregarV:
        add valorNumerico, 5
        jmp siguienteSimbolo

    agregarX:
        cmp byte ptr [si+1], 'L'
        je agregarMenosX
        cmp byte ptr [si+1], 'C'
        je agregarMenosX
        add valorNumerico, 10
        jmp siguienteSimbolo

    agregarMenosX:
        add valorNumerico, 40
        jmp siguienteSimbolo2

    agregarL:
        add valorNumerico, 50
        jmp siguienteSimbolo

    agregarC:
        cmp byte ptr [si+1], 'D'
        je agregarMenosC
        cmp byte ptr [si+1], 'M'
        je agregarMenosC
        add valorNumerico, 100
        jmp siguienteSimbolo

    agregarMenosC:
        add valorNumerico, 400
        jmp siguienteSimbolo2

    agregarD:
        add valorNumerico, 500
        jmp siguienteSimbolo

    agregarM:
        add valorNumerico, 1000
        jmp siguienteSimbolo

    siguienteSimbolo:
        inc si
        jmp nextSymbol

    siguienteSimbolo2:
        add si, 2
        jmp nextSymbol

finConversion:
    ret
convertirANumero endp

convertirAString proc near
    ; Convertir el valor numérico en un string de dígitos
    mov ax, valorNumerico
    lea di, stringVar
    xor cx, cx              ; Inicializar el contador de caracteres

    cmp ax, 0
    jne convertirLoop
    mov byte ptr [di], '0'  ; Si el valor es 0, representarlo como "0"
    inc di
    jmp finString

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
    mov [di], dl            ; Colocar los dígitos en el string
    inc di
    loop imprimirStack      ; Repetir hasta que se hayan impreso todos los dígitos

finString:
    mov byte ptr [di], '$'   ; Terminar el string
    ret
convertirAString endp

imprimirString proc near
    ; Imprimir el string resultante
    Print mensajeSalida
    lea dx, stringVar
    mov ah, 09h
    int 21h
    ret
imprimirString endp

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
    call convertirANumero       ; Convertir el número romano a valor numérico
    call convertirAString       ; Convertir el valor numérico a string
    call imprimirString         ; Mostrar el string resultante
    call finRutina              ; Terminar el programa

codigo ends
end main
