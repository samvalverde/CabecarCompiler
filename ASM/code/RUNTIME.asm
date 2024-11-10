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
    int_numeroPredefinido dw 10    ; Número fijo para pruebas y comparaciones
    int_resultado dw ?             ; Variable para almacenar resultados de operaciones aritméticas
    int_tamaño dw ?                ; Variable para almacenar el tamaño o número de dígitos
DATOS_INT ends

DATOS_CHAR segment
    char_valor db ?                 ; Variable para almacenar el carácter procesado
    char_temporal db ?              ; Variable temporal si es necesaria
    char_predefinido db 'M'         ; Carácter fijo para las comparaciones
DATOS_CHAR ends

DATOS_STR segment
    str_valor db 20 dup('$')          ; Buffer principal para almacenar el string
    str_temporal db ?                 ; Variable temporal si es necesaria
    str_segundo db 20 dup('$')        ; Buffer para un segundo string
    str_resultado db 40 dup('$')      ; Buffer para almacenar el resultado de operaciones (e.g., concatenación)
DATOS_STR ends

DATOS_BOOL segment
    bool_valor db ?             ; Almacenar el valor booleano (0 o 1)
    bool_temporal db ?          ; Temporal para el proceso si es necesario
    bool_valor1 db ?            ; Primer valor booleano para comparación
    bool_valor2 db ?            ; Segundo valor booleano para comparación
DATOS_BOOL ends

DATOS_SET segment
    set_conjunto db 12 dup ('$')            ; Arreglo para almacenar el conjunto de hasta 12 caracteres
    set_temporal db ?                       ; Variable temporal si es necesaria
    set_conjunto1 db 12 dup ('$')           ; Primer conjunto de hasta 12 caracteres
    set_conjunto2 db 12 dup ('$')           ; Segundo conjunto de hasta 12 caracteres
    set_resultado db 12 dup ('$')           ; Conjunto resultado
    set_elemento db ?                       ; Elemento para agregar/borrar/buscar
DATOS_SET ends

DATOS_ROM segment
    rom_conjunto db 20 dup ('$')   ; Buffer para almacenar el número romano
    rom_segundo db 20 dup ('$')    ; Segundo número romano para operaciones
    rom_resultado db 40 dup ('$')  ; Buffer para el resultado de operaciones y conversiones
    rom_entero dw ?                ; Entero convertido desde el primer número romano
    rom_segundoEntero dw ?         ; Entero convertido desde el segundo número romano
    rom_temporal dw ?              ; Temporal de uso general en las operaciones
    rom_potencia dw ?              ; Valor de potencia para cálculo con kalwa
    rom_valorMCD dw ?              ; Valor predefinido para el cálculo del MCD
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
int_op_suma proc near
    pop ax
    mov bx, int_op_numeroPredefinido
    add ax, bx
    push ax
    ret
int_op_suma endp

int_op_resta proc near
    pop ax
    mov bx, int_op_numeroPredefinido
    sub ax, bx
    push ax
    ret
int_op_resta endp

int_op_multiplicacion proc near
    pop ax
    mov bx, int_op_numeroPredefinido
    mul bx
    push ax
    ret
int_op_multiplicacion endp

int_op_division proc near
    pop ax
    mov bx, int_op_numeroPredefinido
    xor dx, dx
    div bx
    push ax
    ret
int_op_division endp

int_op_residuo proc near
    pop ax
    mov bx, int_op_numeroPredefinido
    xor dx, dx
    div bx
    push dx
    ret
int_op_residuo endp

; Operaciones de Comparación
int_op_mayor proc near
    pop ax
    cmp ax, int_op_numeroPredefinido
    setg al
    push ax
    ret
int_op_mayor endp

int_op_menor proc near
    pop ax
    cmp ax, int_op_numeroPredefinido
    setl al
    push ax
    ret
int_op_menor endp

int_op_igual proc near
    pop ax
    cmp ax, int_op_numeroPredefinido
    sete al
    push ax
    ret
int_op_igual endp

int_op_mayorIgual proc near
    pop ax
    cmp ax, int_op_numeroPredefinido
    setge al
    push ax
    ret
int_op_mayorIgual endp

int_op_menorIgual proc near
    pop ax
    cmp ax, int_op_numeroPredefinido
    setle al
    push ax
    ret
int_op_menorIgual endp

int_op_diferente proc near
    pop ax
    cmp ax, int_op_numeroPredefinido
    setne al
    push ax
    ret
int_op_diferente endp

; Operación de Tamaño (Biko)
int_op_biko proc near
    mov ax, int_op_entero   ; Cargar el entero
    xor cx, cx              ; Inicializar el contador de dígitos

contarDigitos:
    cmp ax, 0
    je finBikoInteger
    mov dx, 0
    div word ptr 10         ; Dividir AX por 10
    inc cx                  ; Incrementar el contador de dígitos
    jmp contarDigitos

finBikoInteger:
    mov ax, cx              ; Guardar el número de dígitos en AX
    push ax
    ret
int_op_biko endp

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

char_tölö proc near
    ; Verificar si el carácter es un dígito (0-9)
    pop ax
    cmp al, '0'
    jl char_noEsDigito
    cmp al, '9'
    jg char_noEsDigito
    push ax
    ret
char_noEsDigito:
    push ax
    ret
char_tölö endp

char_jélá proc near
    ; Verificar si el carácter es una letra (A-Z, a-z)
    pop ax
    cmp al, 'A'
    jl char_noEsLetra
    cmp al, 'Z'
    jle char_esLetra
    cmp al, 'a'
    jl char_noEsLetra
    cmp al, 'z'
    jg char_noEsLetra
char_esLetra:
    push ax
    ret
char_noEsLetra:
    push ax
    ret
char_jélá endp

char_júru proc near
    ; Convertir el carácter a mayúscula
    pop ax
    cmp al, 'a'
    jl char_noConvertir
    cmp al, 'z'
    jg char_noConvertir
    sub al, 32
char_noConvertir:
    push ax
    ret
char_júru endp

char_kári proc near
    ; Convertir el carácter a minúscula
    pop ax
    cmp al, 'A'
    jl char_noConvertir
    cmp al, 'Z'
    jg char_noConvertir
    add al, 32
char_noConvertir:
    push ax
    ret
char_kári endp

char_Biko proc near
    ; Tamaño de un carácter (1 byte)
    mov ax, 1
    ret
char_Biko endp

char_mayor proc near
    pop ax
    cmp al, char_predefinido
    jg char_esMayor
    ret
char_esMayor:
    ret
char_mayor endp

char_menor proc near
    pop ax
    cmp al, char_predefinido
    jl char_esMenor
    ret
char_esMenor:
    ret
char_menor endp

char_igual proc near
    pop ax
    cmp al, char_predefinido
    je char_esIgual
    ret
char_esIgual:
    ret
char_igual endp

char_mayorIgual proc near
    pop ax
    cmp al, char_predefinido
    jge char_esMayorIgual
    ret
char_esMayorIgual:
    ret
char_mayorIgual endp

char_menorIgual proc near
    pop ax
    cmp al, char_predefinido
    jle char_esMenorIgual
    ret
char_esMenorIgual:
    ret
char_menorIgual endp

char_diferente proc near
    pop ax
    cmp al, char_predefinido
    jne char_esDiferente
    ret
char_esDiferente:
    ret
char_diferente endp

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

str_bawa proc near
    ; Concatenar str_valor y str_segundo en str_resultado
    lea si, str_valor
    lea di, str_resultado

    ; Copiar str_valor a str_resultado
    str_copiarString:
        mov al, [si]
        cmp al, '$'
        je str_copiarString2
        mov [di], al
        inc si
        inc di
        jmp str_copiarString

    ; Concatenar str_segundo a str_resultado
    str_copiarString2:
        lea si, str_segundo
        str_copiarString2B:
            mov al, [si]
            cmp al, '$'
            je str_finConcatenar
            mov [di], al
            inc si
            inc di
            jmp str_copiarString2B

    str_finConcatenar:
        mov byte ptr [di], '$'  ; Terminar el string concatenado
        push offset str_resultado ; Empujar el resultado en la pila
        ret
str_bawa endp

str_matsií proc near
    ; Calcular el largo de str_valor
    lea si, str_valor
    xor cx, cx                 ; Inicializar el contador de caracteres

    str_contarCaracteres:
        mov al, [si]
        cmp al, '$'
        je str_finContar
        inc cx                  ; Incrementar el contador
        inc si
        jmp str_contarCaracteres

    str_finContar:
        push cx                 ; Empujar el largo en la pila
        ret
str_matsií endp

str_tiä proc near
    ; Cortar str_valor hasta una posición específica
    pop cx                      ; Obtener la posición a cortar
    lea si, str_valor
    lea di, str_resultado
    xor dx, dx                  ; Reiniciar el índice

    str_cortar:
        mov al, [si]
        cmp al, '$'
        je str_finCortar
        cmp dx, cx               ; Comparar con la posición
        je str_finCortar
        mov [di], al
        inc si
        inc di
        inc dx
        jmp str_cortar

    str_finCortar:
        mov byte ptr [di], '$'   ; Terminar el string cortado
        push offset str_resultado ; Empujar el string cortado en la pila
        ret
str_tiä endp

str_kjätiä proc near
    ; Recortar str_valor eliminando espacios al inicio y al final
    lea si, str_valor
    lea di, str_resultado

    ; Saltar los espacios iniciales
    str_recortarInicio:
        mov al, [si]
        cmp al, ' '
        jne str_copiarCaracter
        inc si
        jmp str_recortarInicio

    str_copiarCaracter:
        cmp al, '$'
        je str_finRecortar
        mov [di], al
        inc si
        inc di
        jmp str_recortarInicio

    str_finRecortar:
        ; Recortar los espacios finales
        dec di
        cmp byte ptr [di], ' '
        je str_finRecortar
        inc di
        mov byte ptr [di], '$'   ; Terminar el string recortado
        push offset str_resultado ; Empujar el string recortado en la pila
        ret
str_kjätiä endp

str_kua proc near
    ; Encontrar un carácter en str_valor
    pop cx                      ; Obtener el carácter a buscar
    lea si, str_valor
    xor dx, dx                  ; Reiniciar el índice

    str_buscarCaracter:
        mov al, [si]
        cmp al, '$'
        je str_noEncontrado
        cmp al, cl
        je str_encontrado
        inc si
        inc dx
        jmp str_buscarCaracter

    str_noEncontrado:
        ; Carácter no encontrado
        push -1                 ; Empujar -1 en la pila para indicar no encontrado
        ret

    str_encontrado:
        push dx                 ; Empujar la posición en la pila
        ret
str_kua endp

str_BikoString proc near
    lea si, str_valor          ; Apuntar al inicio de la cadena
    xor cx, cx                 ; Inicializar contador

    str_contarCaracteresS:
        mov al, [si]
        cmp al, '$'            ; Verificar el final de la cadena
        je str_finBikoString
        inc cx                 ; Incrementar contador
        inc si                 ; Mover al siguiente carácter
        jmp str_contarCaracteresS

    str_finBikoString:
        push cx                ; Empujar el tamaño en la pila
        ret
str_BikoString endp

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

bool_op_leer proc near
    ; Leer los valores booleanos desde la pila
    pop ax
    mov bool_valor1, al           ; Almacenar el primer valor booleano
    pop ax
    mov bool_valor2, al           ; Almacenar el segundo valor booleano
    ret
bool_op_leer endp

bool_op_mayor proc near
    ; Comparar si bool_valor1 > bool_valor2
    mov al, bool_valor1
    cmp al, bool_valor2
    jg esMayor
    ; Falso
    mov al, 0
    jmp finMayor
esMayor:
    ; Verdadero
    mov al, 1
finMayor:
    mov bool_valor, al
    ret
bool_op_mayor endp

bool_op_menor proc near
    ; Comparar si bool_valor1 < bool_valor2
    mov al, bool_valor1
    cmp al, bool_valor2
    jl esMenor
    ; Falso
    mov al, 0
    jmp finMenor
esMenor:
    ; Verdadero
    mov al, 1
finMenor:
    mov bool_valor, al
    ret
bool_op_menor endp

bool_op_igual proc near
    ; Comparar si bool_valor1 = bool_valor2
    mov al, bool_valor1
    cmp al, bool_valor2
    je esIgual
    ; Falso
    mov al, 0
    jmp finIgual
esIgual:
    ; Verdadero
    mov al, 1
finIgual:
    mov bool_valor, al
    ret
bool_op_igual endp

bool_op_mayorIgual proc near
    ; Comparar si bool_valor1 >= bool_valor2
    mov al, bool_valor1
    cmp al, bool_valor2
    jge esMayorIgual
    ; Falso
    mov al, 0
    jmp finMayorIgual
esMayorIgual:
    ; Verdadero
    mov al, 1
finMayorIgual:
    mov bool_valor, al
    ret
bool_op_mayorIgual endp

bool_op_menorIgual proc near
    ; Comparar si bool_valor1 <= bool_valor2
    mov al, bool_valor1
    cmp al, bool_valor2
    jle esMenorIgual
    ; Falso
    mov al, 0
    jmp finMenorIgual
esMenorIgual:
    ; Verdadero
    mov al, 1
finMenorIgual:
    mov bool_valor, al
    ret
bool_op_menorIgual endp

bool_op_distinto proc near
    ; Comparar si bool_valor1 >< bool_valor2
    mov al, bool_valor1
    cmp al, bool_valor2
    jne esDistinto
    ; Falso
    mov al, 0
    jmp finDistinto
esDistinto:
    ; Verdadero
    mov al, 1
finDistinto:
    mov bool_valor, al
    ret
bool_op_distinto endp

bool_op_BikoBoolean proc near
    mov al, 1                   ; Longitud fija de un booleano
    mov bool_valor, al
    ret
bool_op_BikoBoolean endp

; ---------------------------------------------------------------
; Sección 5: Rutinas para Conjuntos (Járjá)
; ---------------------------------------------------------------

; ---- Entrada/Salida ----
set_io_leer proc near
    ; Leer el conjunto desde la pila y almacenarlo en set_conjunto
    pop si                     ; Obtener el puntero al conjunto desde la pila
    lea di, set_conjunto       ; Apuntar al buffer de set_conjunto
    mov cx, 12                 ; Leer hasta 12 caracteres

    copiarConjunto:
        mov al, [si]
        mov [di], al
        inc si
        inc di
        loop copiarConjunto

    ; Terminar el string del conjunto
    mov byte ptr [di], '$'
    ret
set_io_leer endp

set_io_imprimir proc near
    ; Imprimir el conjunto contenido en set_conjunto
    lea dx, set_conjunto       ; Apuntar a set_conjunto
    mov ah, 09h
    int 21h
    ret
set_io_imprimir endp

; ---- Algoritmos de Conversión ----
; Incluir aquí los algoritmos de conversión para conjuntos

; ---- Operaciones ----

set_op_leerConjuntos proc near
    ; Leer dos conjuntos desde la pila y almacenarlos en set_conjunto1 y set_conjunto2
    pop si                     ; Puntero al primer conjunto en la pila
    lea di, set_conjunto1      ; Apuntar a set_conjunto1
    mov cx, 12                 ; Copiar hasta 12 caracteres

    copiarConjunto1:
        mov al, [si]
        mov [di], al
        inc si
        inc di
        loop copiarConjunto1

    ; Leer el segundo conjunto desde la pila
    pop si                     ; Puntero al segundo conjunto en la pila
    lea di, set_conjunto2      ; Apuntar a set_conjunto2
    mov cx, 12                 ; Copiar hasta 12 caracteres

    copiarConjunto2:
        mov al, [si]
        mov [di], al
        inc si
        inc di
        loop copiarConjunto2

    ret
set_op_leerConjuntos endp

; ---------------------------------------------------------------
; Operaciones de Conjuntos usando set_conjunto1 y set_conjunto2
; ---------------------------------------------------------------

set_op_wobogo proc near
    ; Agregar el elemento a set_conjunto1
    mov al, set_elemento
    lea di, set_conjunto1

    buscarPosicion:
        cmp byte ptr [di], '$'
        je agregarElemento
        inc di
        jmp buscarPosicion

    agregarElemento:
        mov [di], al
        inc di
        mov byte ptr [di], '$'
        ret
set_op_wobogo endp

set_op_shénawa proc near
    ; Borrar el elemento de set_conjunto1
    mov al, set_elemento
    lea di, set_conjunto1

    buscarElemento:
        cmp byte ptr [di], '$'
        je finBorrado
        cmp [di], al
        je borrarElemento
        inc di
        jmp buscarElemento

    borrarElemento:
        moverElementos:
            mov al, [di+1]
            mov [di], al
            cmp al, '$'
            je finBorrado
            inc di
            jmp moverElementos

    finBorrado:
        ret
set_op_shénawa endp

set_op_ñawötkö proc near
    ; Unión de set_conjunto1 y set_conjunto2 en set_resultado
    lea si, set_conjunto1
    lea di, set_resultado

    copiarConjunto1:
        mov al, [si]
        cmp al, '$'
        je copiarConjunto2
        mov [di], al
        inc si
        inc di
        jmp copiarConjunto1

    copiarConjunto2:
        lea si, set_conjunto2

    copiarNoRepetidos:
        mov al, [si]
        cmp al, '$'
        je finUnion
        lea bx, set_resultado

        buscarDuplicado:
            cmp [bx], '$'
            je agregarElemento
            cmp [bx], al
            je noAgregar
            inc bx
            jmp buscarDuplicado

        agregarElemento:
            mov [di], al
            inc di

        noAgregar:
            inc si
            jmp copiarNoRepetidos

    finUnion:
        mov byte ptr [di], '$'
        ret
set_op_ñawötkö endp

set_op_pakakirei proc near
    ; Intersección de set_conjunto1 y set_conjunto2 en set_resultado
    lea si, set_conjunto1
    lea di, set_resultado

    buscarInterseccion:
        mov al, [si]
        cmp al, '$'
        je finInterseccion
        lea bx, set_conjunto2

    compararConjunto2:
        cmp [bx], '$'
        je siguienteElemento
        cmp al, [bx]
        je agregarElemento
        inc bx
        jmp compararConjunto2

    agregarElemento:
        mov [di], al
        inc di

    siguienteElemento:
        inc si
        jmp buscarInterseccion

    finInterseccion:
        mov byte ptr [di], '$'
        ret
set_op_pakakirei endp

set_op_dodwa proc near
    ; Verificar si el elemento pertenece a set_conjunto1
    mov al, set_elemento
    lea si, set_conjunto1

    buscarElemento:
        cmp byte ptr [si], '$'
        je noPertenece
        cmp [si], al
        je siPertenece
        inc si
        jmp buscarElemento

    noPertenece:
        ret

    siPertenece:
        ret
set_op_dodwa endp

set_op_járebo proc near
    ; Verificar si set_conjunto1 está vacío
    lea si, set_conjunto1
    cmp byte ptr [si], '$'
    je estaVacio
    ret

    estaVacio:
        ret
set_op_járebo endp

set_op_BikoSet proc near
    lea si, set_conjunto1
    xor cx, cx

contarElementos:
    mov al, [si]
    cmp al, '$'
    je finBikoSet
    inc cx
    inc si
    jmp contarElementos

finBikoSet:
    mov ax, cx
    ret
set_op_BikoSet endp

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

rom_op_romanoAEntero proc near
    ; Conversión de un número romano a entero
    lea si, rom_conjunto          ; Apuntar a rom_conjunto en lugar de numeroRomano
    xor ax, ax                    ; AX almacena el número entero

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
        mov rom_entero, ax        ; Guardar el valor entero en rom_entero
        ret
rom_op_romanoAEntero endp

rom_op_convertirSegundoRomanoAEntero proc near
    ; Conversión del segundo número romano a entero
    lea si, rom_segundo           ; Apuntar a rom_segundo en lugar de segundoRomano
    xor ax, ax                    ; AX almacena el número entero

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
        mov rom_segundoEntero, ax  ; Guardar el valor entero del segundo número en rom_segundoEntero
        ret
rom_op_convertirSegundoRomanoAEntero endp

; ---------------------------------------------------------------
; Conversión de Entero a Romano
; ---------------------------------------------------------------
rom_op_enteroARomano proc near
    ; Convertir el entero a número romano
    mov ax, rom_entero
    lea di, rom_resultado          ; Apuntar al buffer para el número romano

    cmp ax, 1000
    jl cientos
    sub ax, 1000
    mov byte ptr [di], 'M'
    inc di
    jmp rom_op_enteroARomano

cientos:
    cmp ax, 900
    jl quinientos
    sub ax, 900
    mov byte ptr [di], 'C'
    mov byte ptr [di+1], 'M'
    add di, 2
    jmp rom_op_enteroARomano

quinientos:
    cmp ax, 500
    jl cuatrocientos
    sub ax, 500
    mov byte ptr [di], 'D'
    inc di
    jmp rom_op_enteroARomano

cuatrocientos:
    cmp ax, 400
    jl cientosMenores
    sub ax, 400
    mov byte ptr [di], 'C'
    mov byte ptr [di+1], 'D'
    add di, 2
    jmp rom_op_enteroARomano

cientosMenores:
    cmp ax, 100
    jl decenas
    sub ax, 100
    mov byte ptr [di], 'C'
    inc di
    jmp rom_op_enteroARomano

decenas:
    ; Continuar para convertir 50, 40, 10, 9, 5, 4, 1
    ret
rom_op_enteroARomano endp

rom_op_suma proc near
    ; Sumar los dos enteros
    mov ax, rom_entero
    add ax, rom_segundoEntero
    mov rom_entero, ax
    Print mensajeSuma
    ret
rom_op_suma endp

rom_op_resta proc near
    ; Restar los dos enteros
    mov ax, rom_entero
    sub ax, rom_segundoEntero
    mov rom_entero, ax
    Print mensajeResta
    ret
rom_op_resta endp

rom_op_producto proc near
    ; Multiplicar los dos enteros
    mov ax, rom_entero
    mul rom_segundoEntero
    mov rom_entero, ax
    Print mensajeProducto
    ret
rom_op_producto endp

rom_op_division proc near
    ; Dividir los dos enteros
    mov ax, rom_entero
    div rom_segundoEntero
    mov rom_entero, ax
    Print mensajeDivision
    ret
rom_op_division endp

rom_op_residuo proc near
    ; Obtener el residuo de dos enteros
    mov ax, rom_entero
    div rom_segundoEntero
    mov rom_entero, dx
    Print mensajeResiduo
    ret
rom_op_residuo endp

; ---------------------------------------------------------------
; Calcular la longitud del número romano (Biko)
; ---------------------------------------------------------------
rom_op_biko proc near
    lea si, rom_conjunto        ; Apuntar al número romano en rom_conjunto
    xor cx, cx                  ; Inicializar contador

contarSimbolos:
    mov al, [si]
    cmp al, '$'                 ; Verificar el final
    je finBikoRoman
    inc cx                      ; Incrementar contador
    inc si                      ; Mover al siguiente símbolo
    jmp contarSimbolos

finBikoRoman:
    mov ax, cx                  ; Guardar la longitud en AX
    Print mensajeTamaño
    ret
rom_op_biko endp

; ---------------------------------------------------------------
; Calcular la potencia (Kalwa)
; ---------------------------------------------------------------
rom_op_kalwa proc near
    mov ax, rom_entero          ; Cargar el número entero convertido desde rom_conjunto
    mov cx, rom_potencia        ; Cargar el valor de la potencia desde datos
    mov bx, ax                  ; Guardar el número base en BX

    cmp cx, 1                   ; Si la potencia es 1, no se necesita hacer nada
    je finPotencia

calcularPotencia:
    mul bx                      ; Multiplicar el número por sí mismo
    loop calcularPotencia

finPotencia:
    mov rom_entero, ax          ; Guardar el resultado en rom_entero
    Print mensajePotencia
    ret
rom_op_kalwa endp

; ---------------------------------------------------------------
; Calcular el MCD (Saka)
; ---------------------------------------------------------------
rom_op_saka proc near
    mov ax, rom_entero          ; Cargar el número entero convertido
    mov bx, rom_valorMCD        ; Cargar el valor predefinido desde datos

calcularMCD:
    cmp bx, 0                   ; Si el valor predefinido es 0, terminar
    je finMCD
    xor dx, dx                  ; Asegurarse de que DX esté limpio para la división
    div bx                      ; AX = AX / BX, DX = residuo

    mov ax, bx                  ; AX toma el valor de BX
    mov bx, dx                  ; BX toma el valor del residuo (DX)
    jmp calcularMCD             ; Repetir hasta que BX sea 0

finMCD:
    mov rom_entero, ax          ; Guardar el MCD en rom_entero
    Print mensajeMCD
    ret
rom_op_saka endp

; ---------------------------------------------------------------
; Invertir el número romano (Reverso)
; ---------------------------------------------------------------
rom_op_reverso proc near
    lea si, rom_conjunto        ; Apuntar al inicio de rom_conjunto
    lea di, rom_resultado       ; Apuntar al buffer donde se guardará el resultado
    mov cx, 0                   ; Inicializar el contador de longitud

    ; Calcular la longitud de rom_conjunto
    contarLongitud:
        mov al, [si + cx]
        cmp al, '$'
        je invertir
        inc cx
        jmp contarLongitud

    invertir:
        dec cx                   ; Ajustar el índice a la última posición
    invertirLoop:
        mov al, [si + cx]        ; Obtener el último carácter
        mov [di], al             ; Guardarlo en rom_resultado
        dec cx                   ; Retroceder en rom_conjunto
        inc di                   ; Avanzar en rom_resultado
        cmp cx, -1               ; Revisar si llegamos al inicio
        jne invertirLoop

    mov byte ptr [di], '$'       ; Terminar la cadena con '$'
    Print mensajeResultado
    ret
rom_op_reverso endp

; ---------------------------------------------------------------
; Cambiar el orden de los símbolos en el número romano (Switch Random)
; ---------------------------------------------------------------
rom_op_switchRandom proc near
    lea si, rom_conjunto        ; Apuntar al inicio de rom_conjunto
    lea di, rom_resultado       ; Apuntar al buffer de resultado
    mov cx, 0                   ; Contador de longitud
    mov bx, 0                   ; Índice temporal

    ; Contar la longitud de rom_conjunto
    contarLongitudSwitch:
        mov al, [si + cx]
        cmp al, '$'
        je intercambiar
        inc cx
        jmp contarLongitudSwitch

    intercambiar:
        mov bx, cx               ; Guardar la longitud total en BX
        dec bx                   ; Ajustar índice para intercambios

        ; Intercambiar símbolos
        mov al, [si]             ; Primer símbolo
        mov ah, [si + bx]        ; Último símbolo
        mov [di], ah             ; Escribe el último al inicio
        mov [di + bx], al        ; Escribe el primero al final

        ; Finalizar la cadena en rom_resultado
        mov byte ptr [di], '$'
        Print mensajeResultado
        ret
rom_op_switchRandom endp

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
