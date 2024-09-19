; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Shtáwa (Entero) a Járjá (Conjunto)
; Este programa toma un número entero y lo convierte en un conjunto
; de caracteres que representan los dígitos del número.
; ---------------------------------------------------------------

datos segment
    entero dw ?              ; Variable para almacenar el número entero
    conjunto db 12 dup ('$') ; Arreglo para almacenar el conjunto de dígitos
    mensajeEntrada db 'Ingrese un número entero: $'
    mensajeSalida db 'El conjunto de dígitos es: {: ', 0
    finConjunto db ' :}$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Shtáwa (Entero) a Járjá (Conjunto).$'
    acercaDe2 db 'El entero se convierte en un conjunto de dígitos.$'
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
    ; Solicitar el número entero al usuario
    Print mensajeEntrada
    mov ah, 01h             ; Leer un carácter (dígito)
    int 21h
    sub al, '0'             ; Convertir el dígito de ASCII a numérico
    mov entero, ax          ; Guardar el valor leído
    ret
leerEntero endp

convertirAConjunto proc near
    ; Convertir el número entero en un conjunto de dígitos
    mov ax, entero
    lea si, conjunto
    xor cx, cx              ; Inicializar contador de caracteres

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
    mov [si], dl            ; Colocar los dígitos en el conjunto
    inc si
    loop imprimirStack      ; Repetir hasta que se hayan impreso todos los dígitos

    mov byte ptr [si], '$'   ; Terminar el string del conjunto
    ret
convertirAConjunto endp

imprimirConjunto proc near
    ; Imprimir el conjunto en formato {: '1', '2', '3' :}
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
    call leerEntero          ; Leer un número entero
    call convertirAConjunto  ; Convertir el entero a conjunto
    call imprimirConjunto    ; Mostrar el conjunto resultante
    call finRutina           ; Terminar el programa

codigo ends
end main
