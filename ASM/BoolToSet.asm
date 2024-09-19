; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: YínaKulíwa (Booleano) a Járjá (Conjunto)
; Este programa toma un valor booleano y lo convierte en un conjunto.
; Si es verdadero, el conjunto contiene 'T'; si es falso, está vacío.
; ---------------------------------------------------------------

datos segment
    booleano db ?               ; Variable para almacenar el booleano
    conjunto db 12 dup ('$')    ; Arreglo para almacenar el conjunto
    mensajeEntrada db 'Ingrese un valor booleano (0 = kóyuwé, 1 = síwa): $'
    mensajeSalida db 'El conjunto resultante es: {: ', 0
    finConjunto db ' :}$', 0
    boolFalso db 'kóyuwé$', 0
    boolVerdadero db 'síwa$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de YínaKulíwa (Booleano) a Járjá (Conjunto).$'
    acercaDe2 db 'El booleano se convierte en un conjunto.$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

leerBooleano proc near
    ; Solicitar el valor booleano al usuario
    Print mensajeEntrada
    mov ah, 01h
    int 21h
    sub al, '0'
    mov booleano, al
    ret
leerBooleano endp

convertirAConjunto proc near
    ; Convertir el valor booleano en un conjunto
    cmp booleano, 1
    je esVerdadero

    ; Si es falso, el conjunto está vacío
    lea di, conjunto
    mov byte ptr [di], '$'
    ret

esVerdadero:
    ; Si es verdadero, el conjunto contiene 'T'
    lea di, conjunto
    mov byte ptr [di], 'T'
    inc di
    mov byte ptr [di], '$'   ; Terminar el string del conjunto
    ret
convertirAConjunto endp

imprimirConjunto proc near
    ; Imprimir el conjunto en formato {: 'T' :} o {: :}
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
    call leerBooleano         ; Leer el valor booleano
    call convertirAConjunto   ; Convertir el booleano a conjunto
    call imprimirConjunto     ; Mostrar el conjunto resultante
    call finRutina            ; Terminar el programa

codigo ends
end main
