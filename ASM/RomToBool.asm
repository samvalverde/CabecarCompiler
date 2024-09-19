; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Wömële (Número Romano) a YínaKulíwa (Booleano)
; Este programa toma un número romano válido y siempre lo convierte en síwa (verdadero),
; ya que en los números romanos no existe el valor 0.
; ---------------------------------------------------------------

datos segment
    numeroRomano db 'IV$', 0      ; Número romano de entrada
    booleano db ?                 ; Variable para almacenar el valor booleano
    mensajeEntrada db 'Ingrese un número romano: $'
    mensajeSalida db 'El valor booleano es: síwa (verdadero)$', 0
    mensajeError db 'Número romano no válido.$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Wömële (Número Romano) a YínaKulíwa (Booleano).$'
    acercaDe2 db 'El número romano se convierte siempre en síwa (verdadero).$'
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

convertirABooleano proc near
    ; Cualquier número romano válido siempre se convierte en síwa (verdadero)
    mov booleano, 1              ; Asignar valor síwa (verdadero)
    ret
convertirABooleano endp

imprimirBooleano proc near
    ; Imprimir el valor booleano (siempre será síwa)
    Print mensajeSalida
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
    call leerNumeroRomano      ; Leer el número romano
    call convertirABooleano    ; Convertir el número romano a booleano (siempre síwa)
    call imprimirBooleano      ; Mostrar el valor booleano resultante (síwa)
    call finRutina             ; Terminar el programa

codigo ends
end main
