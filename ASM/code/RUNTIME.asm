; ----------------------------
; ★     RUNTIME Library     ★
; ----------------------------

; Proyecto - Etapa 4: Generación de Código
; Samuel Valverde A.
; Erick Kauffmann P.
; IC5701 - Compiladores e Intérpretes, GR 2
; Prof. Kirstein Gatjens S.
; II Semestre 2024
; Fecha de Entrega: 11/11/2024

; ###### Manual de Usuario ######
; 
; Este archivo contiene todas las rutinas organizadas por tipo de dato, incluyendo entrada/salida,
; algoritmos de conversión y operaciones específicas. El uso de cada rutina está documentado
; en las secciones respectivas.

; ---------------------------------------------------------------
; Segmento de Datos generales
; ---------------------------------------------------------------
datos segment
    msgAcercaDe db 'Operaciones con la Biblioteca RUNTIME.$', 13, 10, '$'
    blank db 13, 10, '$'
datos ends

pila segment stack 'stack'
    dw 256 dup (?)
pila ends

; ---------------------------------------------------------------
; Macros y Utilidades
; ---------------------------------------------------------------
Print Macro mensaje
    mov ah, 09h
    lea dx, mensaje
    int 21h
EndM

DATOS_INT segment
    int_valor dw ?                 ; Variable para almacenar el valor entero procesado
    int_temporal dw ?              ; Variable temporal si es necesaria
DATOS_INT ends

DATOS_CHAR segment
    char_valor db ?                 ; Variable para almacenar el carácter procesado
    char_temporal db ?              ; Variable temporal si es necesaria
DATOS_CHAR ends

DATOS_STR segment
    str_valor db 20 dup('$')       ; Buffer para almacenar el string
    str_temporal db ?              ; Variable temporal si es necesaria
DATOS_STR ends

DATOS_BOOL segment
    bool_valor db ?             ; Almacenar el valor booleano (0 o 1)
    bool_temporal db ?          ; Temporal para el proceso si es necesario
DATOS_BOOL ends

DATOS_SET segment
    set_conjunto db 12 dup ('$')  ; Arreglo para almacenar el conjunto de hasta 12 caracteres
    set_temporal db ?             ; Temporal de uso general en las operaciones
DATOS_SET ends

DATOS_ROM segment
    rom_conjunto db 20 dup ('$')   ; Buffer para almacenar el número romano
    rom_temporal db ?              ; Temporal de uso general en las operaciones
DATOS_ROM ends

; ---------------------------------------------------------------
codigo segment
    assume cs:codigo, ds:datos, ss:pila

; ---------------------------------------------------------------
; Rutinas Generales
; ---------------------------------------------------------------

; ---------------------------------------------------------------
; Sección 1: Rutinas para Enteros (Shtáwa)
; ---------------------------------------------------------------

; ---- Entrada/Salida ----

int_io_leer proc near
    ; Leer un valor entero desde la pila
    ; El valor ya está en la pila, solo se carga para procesamiento

    pop ax                 ; Obtener el valor desde la pila
    mov int_valor, ax      ; Guardar en la variable de entero para E/S
    ret
int_io_leer endp

int_io_imprimir proc near
    ; Almacenar el valor entero en AX para pasarlo al siguiente proceso
    ; o desplegarlo en el programa principal si fuera necesario

    mov ax, int_valor      ; Cargar el valor entero
    push ax                ; Guardarlo de nuevo en la pila
    ret
int_io_imprimir endp

; ---- Algoritmos de Conversión ----
; Incluir aquí los algoritmos de conversión para enteros

; ---- Operaciones ----
; Incluir aquí las operaciones aritméticas y de comparación para enteros

; ---------------------------------------------------------------
; Sección 2: Rutinas para Caracteres (ékla)
; ---------------------------------------------------------------

; ---- Entrada/Salida ----

char_io_leer proc near
    ; Leer un carácter desde la pila
    ; El carácter ya está en la pila, solo se carga para procesamiento

    pop ax                  ; Obtener el carácter desde la pila
    mov char_valor, al      ; Guardar en la variable de carácter para E/S
    ret
char_io_leer endp

char_io_imprimir proc near
    ; Almacenar el carácter en AL para pasarlo al siguiente proceso
    ; o desplegarlo en el programa principal si fuera necesario

    mov al, char_valor      ; Cargar el valor del carácter
    push ax                 ; Guardarlo de nuevo en la pila
    ret
char_io_imprimir endp

; ---- Algoritmos de Conversión ----
; Incluir aquí los algoritmos de conversión para caracteres

; ---- Operaciones ----
; Incluir aquí las operaciones de manipulación y comparación para caracteres

; ---------------------------------------------------------------
; Sección 3: Rutinas para Strings (Shéj)
; ---------------------------------------------------------------

; ---- Entrada/Salida ----
str_io_leer proc near
    ; Leer un string desde la pila y almacenarlo en el buffer str_valor
    ; El string completo se pasa por la pila, carácter por carácter, en formato ASCII

    lea si, str_valor              ; Apuntar al buffer
    xor cx, cx                     ; Inicializar contador de longitud del string

    leerCaracter:
        pop ax                     ; Obtener el carácter desde la pila
        cmp al, '$'                ; Comprobar si es fin de string
        je finLectura
        mov [si], al               ; Guardar el carácter en el buffer
        inc si                     ; Incrementar el puntero
        inc cx                     ; Incrementar el contador de caracteres
        cmp cx, 19                 ; Verificar límite del buffer
        jae finLectura
        jmp leerCaracter           ; Repetir para el siguiente carácter

    finLectura:
        mov byte ptr [si], '$'     ; Marcar fin de string en el buffer
        ret
str_io_leer endp

str_io_imprimir proc near
    ; Cargar el string en el buffer para pasarlo al siguiente proceso
    lea si, str_valor              ; Apuntar al buffer con el string leído
    ; Opcional: Si es necesario, el programa principal puede imprimir desde `str_valor`
    ret
str_io_imprimir endp

; ---- Algoritmos de Conversión ----
; Incluir aquí los algoritmos de conversión para cadenas

; ---- Operaciones ----
; Incluir aquí las operaciones específicas para cadenas

; ---------------------------------------------------------------
; Sección 4: Rutinas para Booleanos (YínaKulíwa)
; ---------------------------------------------------------------

; ---- Entrada/Salida ----

bool_io_leer proc near
    ; Leer un valor booleano (0 o 1) desde la pila
    pop ax                      ; Obtener el valor desde la pila
    cmp al, '0'
    je setFalse
    cmp al, '1'
    je setTrue
    jmp entradaInvalida         ; Manejar entradas no válidas

setFalse:
    mov bool_valor, 0           ; Almacenar 0 si es falso
    ret

setTrue:
    mov bool_valor, 1           ; Almacenar 1 si es verdadero
    ret

entradaInvalida:
    ; Acción en caso de entrada inválida (0 o 1 esperados)
    mov bool_valor, 0           ; Opcionalmente, asignar un valor por defecto
    ret
bool_io_leer endp

bool_io_imprimir proc near
    ; Cargar el valor booleano en bool_valor para el proceso siguiente
    lea dx, bool_valor          ; Apuntar al valor booleano
    ; Opcional: Si es necesario, el programa principal puede imprimir desde `bool_valor`
    ret
bool_io_imprimir endp

; ---- Algoritmos de Conversión ----
; Incluir aquí los algoritmos de conversión para booleanos

; ---- Operaciones ----
; Incluir aquí las operaciones lógicas para booleanos

; ---------------------------------------------------------------
; Sección 5: Rutinas para Conjuntos (Járjá)
; ---------------------------------------------------------------

; ---- Entrada/Salida ----
set_io_leer proc near
    ; Leer un conjunto de caracteres desde la pila
    lea si, set_conjunto        ; Puntero al arreglo del conjunto
    mov cx, 10                  ; Leer hasta 10 caracteres

leerCaracter:
    pop ax                      ; Obtener cada carácter desde la pila
    cmp al, 0Dh                 ; Comparar si es "Enter" (0Dh)
    je finLectura               ; Terminar la lectura si se encuentra Enter

    mov [si], al                ; Guardar el carácter en el conjunto
    inc si                      ; Avanzar al siguiente espacio en el arreglo
    loop leerCaracter           ; Repetir hasta que se lean 10 caracteres o se alcance el final

finLectura:
    mov byte ptr [si], '$'      ; Terminar el string del conjunto
    ret
set_io_leer endp

set_io_imprimir proc near
    ; Cargar el conjunto en set_conjunto para que sea procesado en el programa principal
    lea dx, set_conjunto        ; Apuntar a set_conjunto
    ; Opcional: Se imprime desde `set_conjunto` en el programa principal si es necesario
    ret
set_io_imprimir endp

; ---- Algoritmos de Conversión ----
; Incluir aquí los algoritmos de conversión para conjuntos

; ---- Operaciones ----
; Incluir aquí las operaciones de conjunto, como agregar, borrar, y verificar pertenencia

; ---------------------------------------------------------------
; Sección 6: Rutinas para Números Romanos (Wömële)
; ---------------------------------------------------------------

; ---- Entrada/Salida ----

rom_io_leer proc near
    ; Leer un número romano desde la pila
    lea si, rom_conjunto        ; Puntero al arreglo de rom_conjunto
    mov cx, 19                  ; Leer hasta 19 caracteres

leerCaracter:
    pop ax                      ; Obtener cada carácter desde la pila
    cmp al, 0Dh                 ; Comparar si es "Enter" (0Dh)
    je finLectura               ; Terminar la lectura si se encuentra Enter

    mov [si], al                ; Guardar el carácter en rom_conjunto
    inc si                      ; Avanzar al siguiente espacio en el arreglo
    loop leerCaracter           ; Repetir hasta que se lean 19 caracteres o se alcance el final

finLectura:
    mov byte ptr [si], '$'      ; Terminar el string del conjunto
    ret
rom_io_leer endp

rom_io_imprimir proc near
    ; Cargar el conjunto en rom_conjunto para que sea procesado en el programa principal
    lea dx, rom_conjunto        ; Apuntar a rom_conjunto
    ; Opcional: Se imprime desde `rom_conjunto` en el programa principal si es necesario
    ret
rom_io_imprimir endp

; ---- Algoritmos de Conversión ----
; Incluir aquí los algoritmos de conversión para números romanos

; ---- Operaciones ----
; Incluir aquí las operaciones aritméticas, reverso, y cambio aleatorio para números romanos

; ---------------------------------------------------------------
; Rutina de Finalización
; ---------------------------------------------------------------
finRutina proc near
    mov ax, 4C00h           ; Terminar el programa
    int 21h
    ret
finRutina endp

main:
    ; Inicializar segmentos
    mov ax, ds
    mov es, ax
    mov ax, pila
    mov ss, ax
    mov ax, datos
    mov ds, ax

    ; Llamar a las rutinas según el flujo de cada tipo de dato
    ; Ejemplo: call rutinas de E/S, de conversión y de operaciones

    ; Finalizar programa
    Print msgAcercaDe
    call finRutina
codigo ends
end main
