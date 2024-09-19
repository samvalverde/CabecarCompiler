; Proyecto - Etapa 0: Rutinas de Enteros
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

datos segment
    entero dw ?             ; Variable para almacenar el número de 16 bits
    msgEntrada db 'Por favor, ingrese un número entero:$'
    msgSalida db 'El número ingresado es: $'
    mensajeVacio db 'La entrada está vacía.$'
    blank db 13, 10, '$'
    acercaDe1 db 'Rutina de manejo para enteros.$'
    acercaDe2 db 'Manejo de entrada y salida de enteros de 16 bits.$'
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
    ; Solicitar la entrada de un número entero
    Print msgEntrada
    lea dx, entero         ; Dirección del buffer donde almacenar el número
    xor cx, cx             ; Inicializar CX para almacenar el número leído

    leerDigito:
        mov ah, 01h        ; Leer un carácter desde la entrada estándar
        int 21h
        cmp al, 0Dh        ; Verificar si se presionó "Enter"
        je finLectura
        sub al, '0'        ; Convertir el carácter leído a valor numérico
        mov bx, 10         ; BX = 10
        mul bx             ; Multiplicar el acumulador por 10
        add cx, ax         ; Sumar el valor leído a CX
        jmp leerDigito     ; Leer el siguiente dígito

    finLectura:
        mov entero, cx     ; Almacenar el número leído en la variable "entero"
        ret
leerEntero endp

imprimirEntero proc near
    ; Mostrar el valor entero almacenado
    mov ax, entero          ; Cargar el valor del entero en AX
    xor cx, cx              ; Inicializar CX como contador de dígitos

    cmp ax, 0               ; Verificar si el número es 0
    jne imprimirLoop        ; Si no es 0, continuar
    mov dl, '0'             ; Si es 0, mostrar directamente
    mov ah, 02h
    int 21h
    jmp finImpresion

    imprimirLoop:
        mov bx, 10
        xor dx, dx          ; DX:AX = AX / 10
        div bx              ; Dividir AX por 10
        add dl, '0'         ; Convertir el residuo en carácter
        push dx             ; Guardar el dígito en la pila
        inc cx              ; Incrementar el contador de dígitos
        test ax, ax         ; Comprobar si AX es 0
        jne imprimirLoop    ; Si no es 0, seguir dividiendo

    imprimirStack:
        pop dx              ; Recuperar el dígito de la pila
        mov ah, 02h         ; Imprimir el dígito
        int 21h
        loop imprimirStack  ; Imprimir todos los dígitos

    finImpresion:
        ret
imprimirEntero endp

finRutina proc near
    mov ax, 4C00h           ; Terminar el programa
    int 21h
    ret
finRutina endp

main:
    mov ax, ds
    mov es, ax

    mov ax, pila
    mov ss, ax

    mov ax, datos
    mov ds, ax

    ; Llamar a las rutinas en orden
    call leerEntero         ; Leer número entero
    call imprimirEntero     ; Mostrar el número ingresado
    call finRutina          ; Terminar el programa

codigo ends
end main
