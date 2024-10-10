; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Járjá (Conjunto) a Shéj (String)
; Este programa toma un conjunto de caracteres y los convierte en un string.
; ---------------------------------------------------------------

datos segment
    conjunto db 'H', 'e', 'l', 'l', 'o', '$', 0  ; Conjunto de caracteres
    stringVar db 128 dup ('$')                   ; Variable para almacenar el string resultante
    mensajeEntrada db 'El conjunto es: $'
    mensajeSalida db 'El string resultante es: $'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Járjá (Conjunto) a Shéj (String).$'
    acercaDe2 db 'El conjunto se convierte en un string.$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

leerConjunto proc near
    ; Mostrar el conjunto almacenado
    Print mensajeEntrada
    lea dx, conjunto
    mov ah, 09h
    int 21h
    ret
leerConjunto endp

convertirAString proc near
    ; Convertir el conjunto de caracteres en un string
    lea si, conjunto
    lea di, stringVar
    xor cx, cx                 ; Contador para el tamaño del string

convertirLoop:
    mov al, [si]
    cmp al, '$'                ; Verificar si se alcanzó el final del conjunto
    je finConversion
    mov [di], al               ; Mover el carácter al string resultante
    inc si                     ; Avanzar al siguiente carácter en el conjunto
    inc di                     ; Avanzar al siguiente carácter en el string
    inc cx                     ; Incrementar el contador
    cmp cx, 127                ; Limitar el tamaño del string a 127 caracteres
    jne convertirLoop

finConversion:
    mov byte ptr [di], '$'      ; Terminar el string
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
    call leerConjunto          ; Leer el conjunto de caracteres
    call convertirAString      ; Convertir el conjunto a string
    call imprimirString        ; Mostrar el string resultante
    call finRutina             ; Terminar el programa

codigo ends
end main
