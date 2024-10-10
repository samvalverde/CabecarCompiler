; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######


; ---------------------------------------------------------------
; Algoritmo de Conversión: Shéj (String) a Wömële (Número Romano)
; Este programa toma un string que representa un número entero y lo convierte en un número romano.
; ---------------------------------------------------------------

datos segment
    stringVar db '49$', 0              ; String que representa el número entero
    entero dw ?                        ; Variable para almacenar el valor numérico
    roman db 20 dup ('$')              ; Arreglo para almacenar el número romano
    mensajeEntrada db 'El string es: $'
    mensajeSalida db 'El número romano es: $'
    mensajeError db 'String no válido. Solo números enteros positivos.$', 0
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Shéj (String) a Wömële (Número Romano).$'
    acercaDe2 db 'El string se convierte en su equivalente en números romanos.$'
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

convertirAEntero proc near
    ; Convertir el string en un número entero
    lea si, stringVar
    xor ax, ax                  ; Inicializar el acumulador de valor numérico
    xor bx, bx                  ; Inicializar el valor total (en BX)
    mov cx, 0                   ; Contador para limitar el número de caracteres

convertirLoop:
    mov al, [si]
    cmp al, '$'                 ; Verificar si se alcanzó el final del string
    je finConversion
    sub al, '0'                 ; Convertir el carácter a valor numérico
    cmp al, 9
    ja error                    ; Si el valor no es un dígito válido
    imul bx, 10                 ; Multiplicar el acumulador por 10
    add bx, ax                  ; Sumar el dígito al acumulador
    inc si                      ; Avanzar al siguiente carácter en el string
    inc cx                      ; Incrementar el contador de caracteres
    cmp cx, 5                   ; Limitar el tamaño del número a 5 dígitos
    jne convertirLoop

finConversion:
    cmp bx, 0                   ; No se permiten valores negativos o 0
    jle error
    mov entero, bx              ; Almacenar el valor numérico en la variable "entero"
    ret

error:
    Print mensajeError
    ret
convertirAEntero endp

convertirARomano proc near
    ; Convertir el entero a número romano
    mov ax, entero
    mov si, offset roman        ; Apuntar al buffer de la cadena romana

    ; Convertir a números romanos según los valores
    cmp ax, 1000
    jl cientos
    sub ax, 1000
    mov byte ptr [si], 'M'      ; Agregar M (1000)
    inc si
    jmp convertirARomano

cientos:
    cmp ax, 900
    jl quinientos
    sub ax, 900
    mov byte ptr [si], 'C'      ; Agregar CM (900)
    mov byte ptr [si+1], 'M'
    add si, 2
    jmp convertirARomano

quinientos:
    cmp ax, 500
    jl cuatrocientos
    sub ax, 500
    mov byte ptr [si], 'D'      ; Agregar D (500)
    inc si
    jmp convertirARomano

cuatrocientos:
    cmp ax, 400
    jl cientosMenores
    sub ax, 400
    mov byte ptr [si], 'C'      ; Agregar CD (400)
    mov byte ptr [si+1], 'D'
    add si, 2
    jmp convertirARomano

cientosMenores:
    cmp ax, 100
    jl decenas
    sub ax, 100
    mov byte ptr [si], 'C'      ; Agregar C (100)
    inc si
    jmp convertirARomano

decenas:
    cmp ax, 90
    jl cincuenta
    sub ax, 90
    mov byte ptr [si], 'X'      ; Agregar XC (90)
    mov byte ptr [si+1], 'C'
    add si, 2
    jmp convertirARomano

cincuenta:
    cmp ax, 50
    jl cuarenta
    sub ax, 50
    mov byte ptr [si], 'L'      ; Agregar L (50)
    inc si
    jmp convertirARomano

cuarenta:
    cmp ax, 40
    jl decenasMenores
    sub ax, 40
    mov byte ptr [si], 'X'      ; Agregar XL (40)
    mov byte ptr [si+1], 'L'
    add si, 2
    jmp convertirARomano

decenasMenores:
    cmp ax, 10
    jl unidades
    sub ax, 10
    mov byte ptr [si], 'X'      ; Agregar X (10)
    inc si
    jmp convertirARomano

unidades:
    cmp ax, 9
    jl cinco
    sub ax, 9
    mov byte ptr [si], 'I'      ; Agregar IX (9)
    mov byte ptr [si+1], 'X'
    add si, 2
    jmp convertirARomano

cinco:
    cmp ax, 5
    jl cuatro
    sub ax, 5
    mov byte ptr [si], 'V'      ; Agregar V (5)
    inc si
    jmp convertirARomano

cuatro:
    cmp ax, 4
    jl unidadesMenores
    sub ax, 4
    mov byte ptr [si], 'I'      ; Agregar IV (4)
    mov byte ptr [si+1], 'V'
    add si, 2
    jmp convertirARomano

unidadesMenores:
    cmp ax, 1
    jl finConversion
    sub ax, 1
    mov byte ptr [si], 'I'      ; Agregar I (1)
    inc si
    jmp convertirARomano

finConversion:
    mov byte ptr [si], '$'      ; Terminar la cadena de números romanos
    ret
convertirARomano endp

imprimirRomano proc near
    ; Imprimir el número romano resultante
    Print mensajeSalida
    lea dx, roman
    mov ah, 09h
    int 21h
    ret
imprimirRomano endp

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
    call convertirAEntero      ; Convertir el string a entero
    call convertirARomano      ; Convertir el entero a número romano
    call imprimirRomano        ; Mostrar el número romano resultante
    call finRutina             ; Terminar el programa

codigo ends
end main