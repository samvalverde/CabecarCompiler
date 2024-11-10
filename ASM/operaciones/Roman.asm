; Proyecto - Etapa 2: Operaciones por tipo de dato
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 09/10/2024

; ###### Manual de Usuario ######

; Este programa recibe un número romano por la pila
; e imprime en la pantalla las diferentes operaciones
; relacionadas con el tipo de dato en forma de bitácora o tour. 
; Se usan valores de ejemplo.

; ---------------------------------------------------------------
; Proyecto - Operaciones para Números Romanos (wömële)
; IC5701 - Compiladores e Intérpretes
; ---------------------------------------------------------------

datos segment
    mensajeResultado db 13, 10, 'Resultado: $'
    mensajeSuma db 13, 10, 'Resultado de la suma: $'
    mensajeResta db 13, 10, 'Resultado de la resta: $'
    mensajeProducto db 13, 10, 'Resultado de la multiplicación: $'
    mensajeDivision db 13, 10, 'Resultado de la división: $'
    mensajeResiduo db 13, 10, 'Resultado del residuo: $'
    mensajePotencia db 13, 10, 'Resultado de la potencia: $'
    mensajeMCD db 13, 10, 'Resultado del MCD: $'
    mensajeTamaño db 'Longitud de número romano: $', 13, 10, '$'
    blank db 13, 10, '$'
    acercaDe db 'Operaciones con Números Romanos (wömële): Suma, Resta, Multiplicación, División, Residuo, Potencia, MCD, Bidireccional, Switch random.$'
    
    numeroRomano db 'XIV$', '$'         ; Primer número romano de ejemplo (convertido por argumento)
    segundoRomano db 'VIII$', '$'       ; Segundo número romano predefinido
    entero db ?                         ; Almacenar el número entero convertido del primer número romano
    segundoEntero db ?                  ; Almacenar el número entero convertido del segundo número romano
    potencia dw 3                       ; Definir el valor de la potencia
    valorMCD dw 6                       ; Valor predefinido para calcular el MCD
    resultadoRomano db 20 dup('$')      ; Almacenar el número romano resultante
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
; Conversión de Romano a Entero
; ---------------------------------------------------------------
romanoAEntero proc near
    ; Conversión de un número romano a entero (modificable para cualquier número romano)
    lea si, numeroRomano
    xor ax, ax               ; AX almacena el número entero

    siguienteSimbolo:
        mov al, [si]
        cmp al, '$'
        je finConversion

        ; Convertir el símbolo actual a su valor entero
        cmp al, 'I'
        je esI
        cmp al, 'V'
        je esV
        cmp al, 'X'
        je esX
        cmp al, 'L'
        je esL
        cmp al, 'C'
        je esC
        cmp al, 'D'
        je esD
        cmp al, 'M'
        je esM
        jmp siguiente

        esI: add ax, 1
        jmp siguiente
        esV: add ax, 5
        jmp siguiente
        esX: add ax, 10
        jmp siguiente
        esL: add ax, 50
        jmp siguiente
        esC: add ax, 100
        jmp siguiente
        esD: add ax, 500
        jmp siguiente
        esM: add ax, 1000
        jmp siguiente

    siguiente:
        inc si
        jmp siguienteSimbolo

    finConversion:
        mov entero, ax        ; Guardar el valor entero en 'entero'
        ret
romanoAEntero endp

convertirSegundoRomanoAEntero proc near
    ; Conversión del segundo número romano a entero
    lea si, segundoRomano
    xor ax, ax               ; AX almacena el número entero

    siguienteSimbolo:
        mov al, [si]
        cmp al, '$'
        je finConversionSegundo

        ; Conversión similar a la del primer número
        cmp al, 'I'
        je esI2
        cmp al, 'V'
        je esV2
        cmp al, 'X'
        je esX2
        cmp al, 'L'
        je esL2
        cmp al, 'C'
        je esC2
        cmp al, 'D'
        je esD2
        cmp al, 'M'
        je esM2
        jmp siguiente2

        esI2: add ax, 1
        jmp siguiente2
        esV2: add ax, 5
        jmp siguiente2
        esX2: add ax, 10
        jmp siguiente2
        esL2: add ax, 50
        jmp siguiente2
        esC2: add ax, 100
        jmp siguiente2
        esD2: add ax, 500
        jmp siguiente2
        esM2: add ax, 1000
        jmp siguiente2

    siguiente2:
        inc si
        jmp siguienteSimbolo

    finConversionSegundo:
        mov segundoEntero, ax  ; Guardar el valor entero del segundo número en 'segundoEntero'
        ret
convertirSegundoRomanoAEntero endp

; ---------------------------------------------------------------
; Conversión de Entero a Romano
; ---------------------------------------------------------------
enteroARomano proc near
    ; Convertir el entero a número romano
    mov ax, entero
    lea di, resultadoRomano    ; Apuntar al buffer para el número romano

    ; Convertir de entero a romano (se mantiene la lógica de conversión)
    cmp ax, 1000
    jl cientos
    sub ax, 1000
    mov byte ptr [di], 'M'
    inc di
    jmp enteroARomano

cientos:
    cmp ax, 900
    jl quinientos
    sub ax, 900
    mov byte ptr [di], 'C'
    mov byte ptr [di+1], 'M'
    add di, 2
    jmp enteroARomano

quinientos:
    cmp ax, 500
    jl cuatrocientos
    sub ax, 500
    mov byte ptr [di], 'D'
    inc di
    jmp enteroARomano

cuatrocientos:
    cmp ax, 400
    jl cientosMenores
    sub ax, 400
    mov byte ptr [di], 'C'
    mov byte ptr [di+1], 'D'
    add di, 2
    jmp enteroARomano

cientosMenores:
    cmp ax, 100
    jl decenas
    sub ax, 100
    mov byte ptr [di], 'C'
    inc di
    jmp enteroARomano

decenas:
    ; Continuar para convertir 50, 40, 10, 9, 5, 4, 1
    ret
enteroARomano endp

; ---------------------------------------------------------------
; Operaciones Aritméticas
; ---------------------------------------------------------------
suma proc near
    ; Sumar los dos enteros (primer valor convertido del argumento y el segundo valor predefinido)
    mov ax, entero
    add ax, segundoEntero
    mov entero, ax
    Print mensajeSuma
    ret
suma endp

resta proc near
    ; Restar los dos enteros
    mov ax, entero
    sub ax, segundoEntero
    mov entero, ax
    Print mensajeResta
    ret
resta endp

producto proc near
    ; Multiplicar los dos enteros
    mov ax, entero
    mul segundoEntero
    mov entero, ax
    Print mensajeProducto
    ret
producto endp

division proc near
    ; Dividir los dos enteros
    mov ax, entero
    div segundoEntero
    mov entero, ax
    Print mensajeDivision
    ret
division endp

residuo proc near
    ; Obtener el residuo de dos enteros
    mov ax, entero
    div segundoEntero
    mov entero, dx
    Print mensajeResiduo
    ret
residuo endp

BikoRoman proc near
    lea si, numeroRomano     ; Apuntar al número romano
    xor cx, cx               ; Inicializar contador

contarSimbolos:
    mov al, [si]
    cmp al, '$'              ; Verificar el final
    je finBikoRoman
    inc cx                   ; Incrementar contador
    inc si                   ; Mover al siguiente símbolo
    jmp contarSimbolos

finBikoRoman:
    mov ax, cx               ; Guardar la longitud en AX
    Print mensajeTamaño
    ret
BikoRoman endp

kalwa proc near
    ; Calcular la potencia del número entero
    mov ax, entero         ; Cargar el número entero convertido del argumento
    mov cx, potencia       ; Cargar el valor de la potencia desde datos
    mov bx, ax             ; Guardar el número base en BX

    cmp cx, 1              ; Si la potencia es 1, no se necesita hacer nada
    je finPotencia

    calcularPotencia:
        mul bx             ; Multiplicar el número por sí mismo
        loop calcularPotencia

    finPotencia:
        mov entero, ax      ; Guardar el resultado en la variable 'entero'
        Print mensajePotencia
        ret
kalwa endp

saka proc near
    ; Calcular el MCD entre el número entero y el valor predefinido (valorMCD)
    mov ax, entero         ; Cargar el número entero convertido del argumento
    mov bx, valorMCD       ; Cargar el valor predefinido desde el segmento de datos

calcularMCD:
    cmp bx, 0              ; Si el valor predefinido es 0, terminar
    je finMCD
    xor dx, dx             ; Asegurarse de que DX esté limpio para la división
    div bx                 ; AX = AX / BX, DX = residuo

    mov ax, bx             ; AX toma el valor de BX
    mov bx, dx             ; BX toma el valor del residuo (DX)
    jmp calcularMCD        ; Repetir hasta que BX sea 0

finMCD:
    mov entero, ax         ; Guardar el MCD en la variable 'entero'
    Print mensajeMCD
    ret
saka endp

reversoRomano proc near
    ; Invertir el contenido de 'numeroRomano'
    lea si, numeroRomano       ; Apuntar al inicio de la cadena
    lea di, resultadoRomano    ; Apuntar al buffer donde se guardará el resultado
    mov cx, 0                  ; Inicializar el contador de longitud

    ; Calcular la longitud de numeroRomano
    contarLongitud:
        mov al, [si + cx]
        cmp al, '$'
        je invertir
        inc cx
        jmp contarLongitud

    invertir:
        dec cx                  ; Ajustar el índice a la última posición
        invertirLoop:
            mov al, [si + cx]   ; Obtener el último carácter
            mov [di], al        ; Guardarlo en el buffer de resultado
            dec cx              ; Retroceder en la cadena original
            inc di              ; Avanzar en el buffer de resultado
            cmp cx, -1          ; Revisar si llegamos al inicio
            jne invertirLoop

        mov byte ptr [di], '$'  ; Terminar la cadena con '$'
        Print mensajeResultado
        ret
reversoRomano endp

switchRandom proc near
    ; Cambiar el orden de los símbolos en 'numeroRomano' de forma pseudoaleatoria
    lea si, numeroRomano       ; Apuntar al inicio de la cadena
    lea di, resultadoRomano    ; Apuntar al buffer de resultado
    mov cx, 0                  ; Contador de longitud
    mov bx, 0                  ; Índice temporal

    ; Contar la longitud del número romano
    contarLongitudSwitch:
        mov al, [si + cx]
        cmp al, '$'
        je intercambiar
        inc cx
        jmp contarLongitudSwitch

    intercambiar:
        mov bx, cx              ; Guardar la longitud total en BX
        dec bx                  ; Ajustar índice para intercambios

        ; Intercambio de símbolos
        mov al, [si]            ; Primer símbolo
        mov ah, [si + bx]       ; Último símbolo
        mov [di], ah            ; Escribe el último al inicio
        mov [di + bx], al       ; Escribe el primero al final
        ; Copia el resto en orden
        mov cx, bx
        dec di

        mov byte ptr [di], '$'  ; Finalizar cadena
        Print mensajeResultado
        ret
switchRandom endp

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

    ; Convertir ambos números romanos a enteros
    call romanoAEntero
    call convertirSegundoRomanoAEntero

    ; Realizar las operaciones aritméticas
    call suma
    call resta
    call producto
    call division
    call residuo
    call biko
    ; operaciones extra
    call kalwa
    call saka
    call reversoRomano
    call switchRandom

    ; Convertir el resultado de nuevo a número romano
    call enteroARomano

    ; Imprimir el número romano resultante
    lea dx, resultadoRomano
    mov ah, 09h
    int 21h

    ; Terminar el programa
    mov ax, 4C00h
    int 21h
codigo ends
end main

; roman XIV