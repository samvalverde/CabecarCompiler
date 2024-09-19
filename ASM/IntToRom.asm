; Proyecto - Etapa 1: Algoritmos de Conversión
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 19/09/2024

; ###### Manual de Usuario ######

; ---------------------------------------------------------------
; Algoritmo de Conversión: Shtáwa (Entero) a Wömële (Número Romano)
; Este programa toma un número entero positivo y lo convierte en
; su representación en números romanos. Si el número es negativo,
; retorna un error.
; ---------------------------------------------------------------

datos segment
    entero dw ?              ; Variable para almacenar el entero
    roman db 20 dup (?)      ; Buffer para la representación romana
    mensajeEntrada db 'Ingrese un número entero positivo: $'
    mensajeSalida db 'El número romano es: $'
    mensajeError db 'Error: Número no válido.$'
    blank db 13, 10, '$'
    acercaDe1 db 'Conversión de Shtáwa a Wömële (Número Romano).$'
    acercaDe2 db 'Convertir un entero positivo a número romano.$'
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
    cmp ax, 0
    jl mostrarError         ; Si el número es negativo, mostrar error
    ret

mostrarError:
    Print mensajeError
    call finRutina
    ret
leerEntero endp

convertirARomano proc near
    ; Convertir el entero a número romano
    mov ax, entero
    mov si, offset roman    ; Apuntar al buffer de la cadena romana

    ; Convertir a números romanos según los valores
    cmp ax, 1000
    jl cientos
    sub ax, 1000
    mov byte ptr [si], 'M'  ; Agregar M (1000)
    inc si
    jmp convertirARomano

    cientos:
        cmp ax, 900
        jl quinientos
        sub ax, 900
        mov byte ptr [si], 'C'  ; Agregar CM (900)
        mov byte ptr [si+1], 'M'
        add si, 2
        jmp convertirARomano

    quinientos:
        cmp ax, 500
        jl cuatrocientos
        sub ax, 500
        mov byte ptr [si], 'D'  ; Agregar D (500)
        inc si
        jmp convertirARomano

    cuatrocientos:
        cmp ax, 400
        jl cientosMenores
        sub ax, 400
        mov byte ptr [si], 'C'  ; Agregar CD (400)
        mov byte ptr [si+1], 'D'
        add si, 2
        jmp convertirARomano

    cientosMenores:
        cmp ax, 100
        jl decenas
        sub ax, 100
        mov byte ptr [si], 'C'  ; Agregar C (100)
        inc si
        jmp convertirARomano

    decenas:
        cmp ax, 90
        jl cincuenta
        sub ax, 90
        mov byte ptr [si], 'X'  ; Agregar XC (90)
        mov byte ptr [si+1], 'C'
        add si, 2
        jmp convertirARomano

    cincuenta:
        cmp ax, 50
        jl cuarenta
        sub ax, 50
        mov byte ptr [si], 'L'  ; Agregar L (50)
        inc si
        jmp convertirARomano

    cuarenta:
        cmp ax, 40
        jl decenasMenores
        sub ax, 40
        mov byte ptr [si], 'X'  ; Agregar XL (40)
        mov byte ptr [si+1], 'L'
        add si, 2
        jmp convertirARomano

    decenasMenores:
        cmp ax, 10
        jl unidades
        sub ax, 10
        mov byte ptr [si], 'X'  ; Agregar X (10)
        inc si
        jmp convertirARomano

    unidades:
        cmp ax, 9
        jl cinco
        sub ax, 9
        mov byte ptr [si], 'I'  ; Agregar IX (9)
        mov byte ptr [si+1], 'X'
        add si, 2
        jmp convertirARomano

    cinco:
        cmp ax, 5
        jl cuatro
        sub ax, 5
        mov byte ptr [si], 'V'  ; Agregar V (5)
        inc si
        jmp convertirARomano

    cuatro:
        cmp ax, 4
        jl unidadesMenores
        sub ax, 4
        mov byte ptr [si], 'I'  ; Agregar IV (4)
        mov byte ptr [si+1], 'V'
        add si, 2
        jmp convertirARomano

    unidadesMenores:
        cmp ax, 1
        jl finConversion
        sub ax, 1
        mov byte ptr [si], 'I'  ; Agregar I (1)
        inc si
        jmp convertirARomano

    finConversion:
        mov byte ptr [si], '$'  ; Terminar el string con $
        ret
convertirARomano endp


imprimirRomano proc near
    ; Mostrar el número romano convertido
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
    call leerEntero         ; Leer número entero
    call convertirARomano   ; Convertir el entero a número romano
    call imprimirRomano     ; Mostrar el número romano
    call finRutina          ; Terminar el programa

codigo ends
end main
