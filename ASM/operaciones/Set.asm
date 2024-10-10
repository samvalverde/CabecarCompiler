; Proyecto - Etapa 2: Operaciones por tipo de dato
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 09/10/2024

; ###### Manual de Usuario ######

; Este programa recibe un conjunto por la pila
; e imprime en la pantalla las diferentes operaciones
; relacionadas con el tipo de dato en forma de bitácora o tour. 
; Se usan valores de ejemplo.

; ---------------------------------------------------------------
datos segment
    mensajeResultado db 13, 10, 'Resultado: $'
    mensajeElementoAgregado db 13, 10, 'Elemento agregado al conjunto.$'
    mensajeElementoBorrado db 13, 10, 'Elemento borrado del conjunto.$'
    mensajeElementoNoEncontrado db 13, 10, 'Elemento no encontrado en el conjunto.$'
    mensajeConjunto db 13, 10, 'Conjunto actual: $'
    mensajeVacio db 13, 10, 'El conjunto está vacío.$'
    mensajeNoVacio db 13, 10, 'El conjunto no está vacío.$'
    mensajePertenece db 13, 10, 'El elemento pertenece al conjunto.$'
    mensajeNoPertenece db 13, 10, 'El elemento no pertenece al conjunto.$'
    blank db 13, 10, '$'
    acercaDe db 'Operaciones con Conjuntos (Járjá): Agregar, Borrar, Unión, Intersección, Pertenencia, Vacío.$'

    conjunto1 db 'A', 'E', 'I', 'O', 'U', '$'     ; Primer conjunto
    conjunto2 db 'B', 'C', 'D', 'E', 'U', '$'     ; Segundo conjunto
    conjuntoResultado db 12 dup('$')              ; Conjunto resultado
    elemento db ?                                 ; Elemento para agregar/borrar/buscar
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

leerElemento proc near
    ; Leer el elemento desde la línea de comandos y empujarlo en la pila
    mov si, 80h                ; Dirección del PSP para el argumento
    mov cl, byte ptr [si]       ; Leer la longitud del argumento
    inc si
    mov al, byte ptr [si]       ; Leer el carácter del argumento
    mov elemento, al            ; Guardar el elemento en la variable
    push ax                     ; Empujar el elemento en la pila
    ret
leerElemento endp

wobogo proc near
    ; Agregar el elemento al conjunto
    pop ax                      ; Obtener el elemento desde la pila
    lea di, conjunto1            ; Apuntar al conjunto

    ; Encontrar la posición vacía en el conjunto
    buscarPosicion:
        cmp byte ptr [di], '$'   ; Comprobar si es el final del conjunto
        je agregarElemento
        inc di
        jmp buscarPosicion

    agregarElemento:
        mov [di], al            ; Agregar el elemento al conjunto
        inc di
        mov byte ptr [di], '$'  ; Marcar el nuevo final del conjunto
        Print mensajeElementoAgregado
        ret
wobogo endp

shénawa proc near
    ; Borrar el elemento del conjunto
    pop ax                      ; Obtener el elemento desde la pila
    lea di, conjunto1            ; Apuntar al conjunto

    ; Buscar el elemento en el conjunto
    buscarElemento:
        cmp byte ptr [di], '$'   ; Comprobar si es el final del conjunto
        je noEncontrado
        cmp [di], al             ; Comparar el elemento actual con el que se busca
        je borrarElemento
        inc di
        jmp buscarElemento

    noEncontrado:
        Print mensajeElementoNoEncontrado
        ret

    borrarElemento:
        ; Desplazar los elementos restantes hacia la izquierda para borrar el elemento
        moverElementos:
            mov al, [di+1]
            mov [di], al
            cmp al, '$'
            je finBorrado
            inc di
            jmp moverElementos

    finBorrado:
        Print mensajeElementoBorrado
        ret
shénawa endp

ñawötkö proc near
    ; Realizar la unión de conjunto1 y conjunto2
    lea si, conjunto1            ; Apuntar al primer conjunto
    lea di, conjuntoResultado    ; Apuntar al conjunto resultado

    ; Copiar el primer conjunto al conjunto resultado
    copiarConjunto1:
        mov al, [si]
        cmp al, '$'
        je copiarConjunto2       ; Si llega al final, copiar el segundo conjunto
        mov [di], al
        inc si
        inc di
        jmp copiarConjunto1

    copiarConjunto2:
        lea si, conjunto2        ; Apuntar al segundo conjunto

    ; Copiar elementos no repetidos del segundo conjunto
    copiarNoRepetidos:
        mov al, [si]
        cmp al, '$'
        je finUnion              ; Final de conjunto2, terminar la unión
        lea bx, conjuntoResultado
        buscarDuplicado:
            cmp [bx], '$'
            je agregarElemento    ; Si es final, agregar el elemento
            cmp [bx], al
            je noAgregar          ; Si ya está, no agregar
            inc bx
            jmp buscarDuplicado

        agregarElemento:
            mov [di], al
            inc di

        noAgregar:
            inc si
            jmp copiarNoRepetidos

    finUnion:
        mov byte ptr [di], '$'   ; Terminar el conjunto resultado
        Print mensajeResultado
        ret
ñawötkö endp

pakakirei proc near
    ; Realizar la intersección de conjunto1 y conjunto2
    lea si, conjunto1            ; Apuntar al primer conjunto
    lea di, conjuntoResultado    ; Apuntar al conjunto resultado

    ; Comparar elementos de conjunto1 con conjunto2
    buscarInterseccion:
        mov al, [si]
        cmp al, '$'
        je finInterseccion       ; Si se llega al final, terminar la intersección
        lea bx, conjunto2        ; Apuntar al segundo conjunto

    compararConjunto2:
        cmp [bx], '$'
        je siguienteElemento     ; Si se llega al final de conjunto2, avanzar al siguiente elemento de conjunto1
        cmp al, [bx]
        je agregarElemento       ; Si hay coincidencia, agregar el elemento a la intersección
        inc bx
        jmp compararConjunto2

    agregarElemento:
        mov [di], al
        inc di

    siguienteElemento:
        inc si
        jmp buscarInterseccion

    finInterseccion:
        mov byte ptr [di], '$'   ; Terminar el conjunto resultado
        Print mensajeResultado
        ret
pakakirei endp

dodwa proc near
    ; Verificar si el elemento pertenece al conjunto
    pop ax                      ; Obtener el elemento desde la pila
    lea si, conjunto1            ; Apuntar al conjunto

    buscarElemento:
        cmp byte ptr [si], '$'   ; Comprobar si es el final del conjunto
        je noPertenece
        cmp [si], al             ; Comparar el elemento actual con el que se busca
        je siPertenece
        inc si
        jmp buscarElemento

    noPertenece:
        Print mensajeNoPertenece
        ret

    siPertenece:
        Print mensajePertenece
        ret
dodwa endp

járebo proc near
    ; Verificar si el conjunto está vacío
    lea si, conjunto1            ; Apuntar al conjunto
    cmp byte ptr [si], '$'       ; Comprobar si el primer elemento es el final
    je estaVacio
    Print mensajeNoVacio
    ret

    estaVacio:
        Print mensajeVacio
        ret
járebo endp

imprimirConjunto proc near
    ; Imprimir el conjunto actual
    lea dx, conjunto1
    mov ah, 09h
    int 21h
    ret
imprimirConjunto endp

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

    ; Leer el elemento como argumento
    call leerElemento        ; Empujar el elemento en la pila

    ; Realizar las operaciones
    call wobogo              ; Agregar el elemento al conjunto
    call imprimirConjunto

    call shénawa             ; Borrar el elemento del conjunto
    call imprimirConjunto

    call ñawötkö             ; Unión de conjuntos
    call imprimirConjunto

    call pakakirei           ; Intersección de conjuntos
    call imprimirConjunto

    call dodwa               ; Verificar pertenencia
    call járebo              ; Verificar si está vacío

    ; Terminar el programa
    mov ax, 4C00h
    int 21h
codigo ends
end main

; set A