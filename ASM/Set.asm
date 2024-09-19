; Proyecto - Etapa 0: Rutinas de Archivos de Texto
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 11/09/2024

; ###### Manual de Usuario ######

; Este programa es sumamente básico ya que recibe una entrada por terminal  
; y la imprime en pantalla. Este es uno de los seis programas para los 6 tipos de datos que se
; manejarán en el proyecto de Compiladores e Intérpretes.

; ---------------------------------------------------------------
; Rutina de Entrada y Salida para Járjá (Conjunto)
; Esta rutina permite ingresar un conjunto de caracteres y
; luego los imprime como un conjunto en el formato:
; {: 'A', 'B', 'C' :}
; ---------------------------------------------------------------

datos segment
    conjunto db 12 dup ('$')    ; Arreglo para almacenar el conjunto de hasta 12 caracteres
    mensajeEntrada db 'Ingrese un conjunto de hasta 10 caracteres: $'
    mensajeSalida db 'El conjunto ingresado es: {: ', 0
    finConjunto db ' :}$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Rutina de manejo para conjuntos (Járjá).$'
    acercaDe2 db 'Manejo de entrada y salida para conjuntos.$'
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

leerConjunto proc near
    ; Solicitar el conjunto de caracteres al usuario
    Print mensajeEntrada
    lea si, conjunto         ; Puntero al arreglo del conjunto
    mov cx, 10               ; Leer hasta 10 caracteres

leerCaracter:
    mov ah, 01h              ; Leer un carácter
    int 21h
    cmp al, 0Dh              ; Comparar si es "Enter"
    je finLectura            ; Si es Enter, terminar la lectura

    mov [si], al             ; Guardar el carácter en el conjunto
    inc si                   ; Avanzar al siguiente espacio en el arreglo
    loop leerCaracter        ; Repetir hasta que se lean 10 caracteres o Enter

finLectura:
    mov byte ptr [si], '$'   ; Terminar el string del conjunto
    ret
leerConjunto endp

imprimirConjunto proc near
    ; Imprimir el conjunto ingresado en el formato {: 'A', 'B', 'C' :}
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
    call leerConjunto        ; Leer el conjunto de caracteres
    call imprimirConjunto    ; Mostrar el conjunto ingresado
    call finRutina           ; Terminar el programa

codigo ends
end main
