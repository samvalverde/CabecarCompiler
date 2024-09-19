; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Shtáwa (Entero) a Ékla (Carácter)
; Este programa toma un número entero y lo convierte en un carácter
; utilizando el código ASCII correspondiente (entero % 256)
; ---------------------------------------------------------------

datos segment
    entero dw ?             ; Variable para almacenar el entero
    caracter db ?           ; Variable para almacenar el carácter
    mensajeEntrada db 'Ingrese un numero entero: $'
    mensajeSalida db 'El caracter correspondiente es: $'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversion de Shtawa a Ekla.$'
    acercaDe2 db 'Modulo 256 para obtener el caracter ASCII.$'
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

convertirACaracter proc near
    ; Realizar la conversión de entero a carácter usando módulo 256
    mov ax, entero
    mov bx, 256
    div bx                  ; Realizar la operación ax % 256
    mov caracter, dl        ; Almacenar el residuo como carácter
    ret
convertirACaracter endp

imprimirCaracter proc near
    ; Mostrar el carácter convertido
    Print mensajeSalida
    mov dl, caracter
    mov ah, 02h
    int 21h
    ret
imprimirCaracter endp

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
    call convertirACaracter  ; Convertir el entero a carácter
    call imprimirCaracter   ; Mostrar el carácter convertido
    call finRutina          ; Terminar el programa

codigo ends
end main
