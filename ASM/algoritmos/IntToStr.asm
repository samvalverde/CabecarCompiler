; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Shtáwa (Entero) a Shéj (String)
; Este programa toma un número entero y lo convierte en un string,
; carácter por carácter, incluyendo el símbolo negativo si es necesario.
; ---------------------------------------------------------------

datos segment
    entero dw ?             ; Variable para almacenar el entero
    string db 16 dup (?)    ; Buffer para el string del número convertido
    mensajeEntrada db 'Ingrese un número entero: $'
    mensajeSalida db 'El string correspondiente es: $'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Shtáwa a Shéj.$'
    acercaDe2 db 'Convertir un entero a string carácter por carácter.$'
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

leerEntero proc near
    ; Solicitar el entero al usuario
    Print mensajeEntrada
    mov ah, 01h             ; Leer un carácter (dígito)
    int 21h
    sub al, '0'             ; Convertir el dígito de ASCII a numérico
    mov entero, ax          ; Guardar el valor leído
    ret
leerEntero endp

convertirAString proc near
    ; Realizar la conversión de entero a string
    mov ax, entero
    xor cx, cx              ; Inicializar el contador de dígitos
    mov si, offset string    ; Apuntar al buffer del string

    cmp ax, 0               ; Comprobar si el número es 0
    jne convertirLoop       ; Si no es 0, proceder con la conversión
    mov byte ptr [si], '0'  ; Si es 0, mostrar directamente '0'
    jmp finConversion

    convertirLoop:
        mov bx, 10
        xor dx, dx
        div bx              ; Dividir AX por 10, resultado en AX, residuo en DX
        add dl, '0'         ; Convertir el dígito a ASCII
        push dx             ; Almacenar en la pila
        inc cx              ; Incrementar el contador
        test ax, ax
        jne convertirLoop   ; Repetir hasta que AX sea 0

    imprimirStack:
        pop dx
        mov [si], dl        ; Almacenar el dígito en el string
        inc si
        loop imprimirStack  ; Repetir para todos los dígitos

    finConversion:
        mov byte ptr [si], '$'  ; Terminar el string con '$'
        ret
convertirAString endp

imprimirString proc near
    ; Mostrar el string convertido
    Print mensajeSalida
    lea dx, string
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
    call leerEntero         ; Leer número entero
    call convertirAString   ; Convertir el entero a string
    call imprimirString     ; Mostrar el string convertido
    call finRutina          ; Terminar el programa

codigo ends
end main
