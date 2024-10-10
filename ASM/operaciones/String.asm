; Proyecto - Etapa 2: Operaciones por tipo de dato
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 09/10/2024

; ###### Manual de Usuario ######

; Este programa recibe un string por la pila
; e imprime en la pantalla las diferentes operaciones
; relacionadas con el tipo de dato en forma de bitácora o tour. 
; Se usan valores de ejemplo.

; ---------------------------------------------------------------
datos segment
    mensajeResultado db 13, 10, 'Resultado: $'
    mensajeConcatenado db 13, 10, 'Concatenado: $'
    mensajeLargo db 13, 10, 'Largo: $'
    mensajeCortado db 13, 10, 'Cortado: $'
    mensajeRecortado db 13, 10, 'Recortado: $'
    mensajeEncontrado db 13, 10, 'Carácter encontrado en la posición: $'
    mensajeNoEncontrado db 13, 10, 'Carácter no encontrado.$'
    blank db 13, 10, '$'
    acercaDe db 'Operaciones con String (Shéj).$'

    string1 db 128 dup('$')
    string2 db 128 dup('$')
    resultado db 256 dup('$')
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

leerString proc near
    ; Leer un string de los argumentos de la línea de comandos y almacenarlo en string1
    mov si, 80h                ; Dirección del PSP para el argumento
    mov cl, byte ptr [si]       ; Leer la longitud del argumento
    inc si
    lea di, string1             ; Apuntar al buffer de string1

    leerCaracter:
        mov al, byte ptr [si]   ; Leer un carácter del argumento
        cmp al, 20h             ; Comprobar si es espacio (separador)
        je finLectura
        mov [di], al            ; Guardar el carácter en string1
        inc si
        inc di
        dec cl
        jmp leerCaracter

    finLectura:
        mov byte ptr [di], '$'  ; Terminar el string
        push offset string1     ; Empujar el puntero al string en la pila
        ret
leerString endp

leerSegundoString proc near
    ; Leer el segundo string desde el argumento de la línea de comandos
    lea di, string2             ; Apuntar al buffer de string2
    inc si                      ; Avanzar al siguiente argumento

    leerCaracter2:
        mov al, byte ptr [si]   ; Leer un carácter del argumento
        cmp al, 20h             ; Comprobar si es espacio (separador)
        je finLectura2
        mov [di], al            ; Guardar el carácter en string2
        inc si
        inc di
        dec cl
        jmp leerCaracter2

    finLectura2:
        mov byte ptr [di], '$'  ; Terminar el string
        push offset string2     ; Empujar el puntero al string en la pila
        ret
leerSegundoString endp

imprimirString proc near
    ; Imprimir el string desde la pila
    pop si                    ; Obtener el puntero al string desde la pila
    lea dx, [si]
    mov ah, 09h
    int 21h
    ret
imprimirString endp

; ---------------------------------------------------------------
; Operaciones sobre strings
; ---------------------------------------------------------------

bawa proc near
    ; Concatenar string1 y string2
    lea si, string1
    lea di, resultado

    ; Copiar string1 a resultado
    copiarString:
        mov al, [si]
        cmp al, '$'
        je copiarString2
        mov [di], al
        inc si
        inc di
        jmp copiarString

    copiarString2:
        lea si, string2

    ; Concatenar string2 a resultado
    copiarString2B:
        mov al, [si]
        cmp al, '$'
        je finConcatenar
        mov [di], al
        inc si
        inc di
        jmp copiarString2B

    finConcatenar:
        mov byte ptr [di], '$'  ; Terminar el string concatenado
        push offset resultado   ; Empujar el resultado en la pila
        ret
bawa endp

matsií proc near
    ; Calcular el largo de string1
    lea si, string1
    xor cx, cx                 ; Inicializar el contador de caracteres

    contarCaracteres:
        mov al, [si]
        cmp al, '$'
        je finContar
        inc cx                  ; Incrementar el contador
        inc si
        jmp contarCaracteres

    finContar:
        push cx                 ; Empujar el largo en la pila
        ret
matsií endp

tiä proc near
    ; Cortar string1 hasta una posición específica
    pop cx                      ; Obtener la posición a cortar
    lea si, string1
    lea di, resultado
    xor dx, dx                  ; Reiniciar el índice

    cortar:
        mov al, [si]
        cmp al, '$'
        je finCortar
        cmp dx, cx               ; Comparar con la posición
        je finCortar
        mov [di], al
        inc si
        inc di
        inc dx
        jmp cortar

    finCortar:
        mov byte ptr [di], '$'   ; Terminar el string cortado
        push offset resultado    ; Empujar el string cortado en la pila
        ret
tiä endp

kjätiä proc near
    ; Recortar string1 eliminando espacios al inicio y al final
    lea si, string1
    lea di, resultado

    ; Saltar los espacios iniciales
    recortarInicio:
        mov al, [si]
        cmp al, ' '
        jne copiarCaracter
        inc si
        jmp recortarInicio

    copiarCaracter:
        cmp al, '$'
        je finRecortar
        mov [di], al
        inc si
        inc di
        jmp recortarInicio

    finRecortar:
        ; Recortar los espacios finales
        dec di
        cmp byte ptr [di], ' '
        je finRecortar
        inc di
        mov byte ptr [di], '$'   ; Terminar el string recortado
        push offset resultado    ; Empujar el string recortado en la pila
        ret
kjätiä endp

kua proc near
    ; Encontrar un carácter en string1
    pop cx                      ; Obtener el carácter a buscar
    lea si, string1
    xor dx, dx                  ; Reiniciar el índice

    buscarCaracter:
        mov al, [si]
        cmp al, '$'
        je noEncontrado
        cmp al, cl
        je encontrado
        inc si
        inc dx
        jmp buscarCaracter

    noEncontrado:
        Print mensajeNoEncontrado
        ret

    encontrado:
        Print mensajeEncontrado
        push dx                 ; Empujar la posición en la pila
        ret
kua endp

; ---------------------------------------------------------------
; Programa principal
; ---------------------------------------------------------------

main:
    ; Inicializar segmentos
    mov ax, datos
    mov ds, ax
    mov es, ax

    mov ax, pila
    mov ss, ax

    ; Mostrar AcercaDe
    Print acercaDe
    Print blank

    ; Leer el string como argumento
    call leerString        ; Empujar el string1 en la pila
    call leerSegundoString ; Empujar el string2 en la pila

    ; Realizar operaciones sobre el string
    call bawa              ; Concatenar string1 con string2
    call imprimirString

    call matsií            ; Calcular el largo del string
    call imprimirString

    call tiä               ; Cortar el string
    call imprimirString

    call kjätiä            ; Recortar el string
    call imprimirString

    ; Terminar el programa
    mov ax, 4C00h
    int 21h
codigo ends
end main

; string "Hola Mundo" "!"  Concatenará "Hola Mundo" con "!"