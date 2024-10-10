; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Shéj (String) a Járjá (Conjunto)
; Este programa toma un string y lo convierte en un conjunto de
; caracteres.
; ---------------------------------------------------------------

datos segment
    stringVar db 'Hola Mundo$', 0      ; String de entrada
    conjunto db 12 dup ('$')           ; Arreglo para almacenar el conjunto
    mensajeSalida db 'El conjunto resultante es: {: ', 0
    finConjunto db ' :}$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Shéj (String) a Járjá (Conjunto).$'
    acercaDe2 db 'El string se convierte en un conjunto.$'
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

convertirAConjunto proc near
    ; Convertir el string en un conjunto de caracteres
    lea si, stringVar       ; Puntero al string de entrada
    lea di, conjunto        ; Puntero al conjunto
    xor cx, cx              ; Inicializar contador de caracteres

convertirLoop:
    mov al, [si]            ; Leer un carácter del string
    cmp al, '$'             ; Verificar si es el final del string
    je finConversion
    mov [di], al            ; Almacenar el carácter en el conjunto
    inc si
    inc di
    inc cx                  ; Incrementar el contador
    cmp cx, 12              ; Limitar el conjunto a 12 caracteres
    je finConversion
    jmp convertirLoop

finConversion:
    mov byte ptr [di], '$'   ; Terminar el string del conjunto
    ret
convertirAConjunto endp

imprimirConjunto proc near
    ; Imprimir el conjunto en formato {: 'H', 'o', 'l', 'a' :}
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
    call convertirAConjunto  ; Convertir el string a conjunto
    call imprimirConjunto    ; Mostrar el conjunto resultante
    call finRutina           ; Terminar el programa

codigo ends
end main
