; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Shéj (String) a YínaKulíwa (Booleano)
; Este programa toma un string que representa "true" o "false" y lo convierte en un valor booleano.
; Si es "true", será síwa (verdadero). Si es "false", será kóyuwé (falso).
; ---------------------------------------------------------------

datos segment
    stringVar db 'true$', 0          ; String de entrada (puede ser "true" o "false")
    booleano db ?                    ; Variable para almacenar el valor booleano
    mensajeEntrada db 'El string es: $'
    mensajeSalida db 'El valor booleano es: $'
    mensajeSiwa db 'síwa (verdadero)$', 0
    mensajeKoyuwe db 'kóyuwé (falso)$', 0
    mensajeError db 'String no válido. Ingrese "true" o "false".$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Shéj (String) a YínaKulíwa (Booleano).$'
    acercaDe2 db 'El string se convierte en un valor booleano.$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

leerString proc near
    ; Mostrar el string almacenado
    Print mensajeEntrada
    lea dx, stringVar
    mov ah, 09h
    int 21h
    ret
leerString endp

convertirABooleano proc near
    ; Convertir el string a booleano (comprobar si es "true" o "false")
    lea si, stringVar
    mov cx, 0                     ; Inicializar el contador para el largo del string

    ; Verificar si el string es "true"
    cmp byte ptr [si], 't'
    jne verificarFalse
    cmp byte ptr [si+1], 'r'
    jne verificarFalse
    cmp byte ptr [si+2], 'u'
    jne verificarFalse
    cmp byte ptr [si+3], 'e'
    jne verificarFalse
    mov booleano, 1               ; Si es "true", asignar síwa (verdadero)
    ret

verificarFalse:
    ; Verificar si el string es "false"
    cmp byte ptr [si], 'f'
    jne error
    cmp byte ptr [si+1], 'a'
    jne error
    cmp byte ptr [si+2], 'l'
    jne error
    cmp byte ptr [si+3], 's'
    jne error
    cmp byte ptr [si+4], 'e'
    jne error
    mov booleano, 0               ; Si es "false", asignar kóyuwé (falso)
    ret

error:
    Print mensajeError
    ret
convertirABooleano endp

imprimirBooleano proc near
    ; Imprimir el valor booleano (síwa o kóyuwé)
    Print mensajeSalida
    cmp booleano, 1
    je imprimirSiwa

    ; Si es kóyuwé (falso)
    Print mensajeKoyuwe
    ret

imprimirSiwa:
    ; Si es síwa (verdadero)
    Print mensajeSiwa
    ret
imprimirBooleano endp

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
    call leerString            ; Leer el string de entrada
    call convertirABooleano    ; Convertir el string a booleano
    call imprimirBooleano      ; Mostrar el valor booleano resultante
    call finRutina             ; Terminar el programa

codigo ends
end main
