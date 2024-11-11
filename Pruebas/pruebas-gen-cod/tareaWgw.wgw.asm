; Proyecto Compilador Wogawg
; Compiladores e Intérpretes
  ; Erick Kauffmann Porcar c.2022180244
  ; Samuel Valverde Arguedas c.2022090162
;------------------ DATA --------------------
datos segment
  MemoriaDinamica db 4096 DUP(?)
  NextFree DW 0
  TmpLenStr DW 0
  TmpSeg DW 0
  TmpOff DW 0
  Result dw 0
datos endS

pila segment stack 'stack'
  dw 256 dup(? )
pila endS

codigo segment
  assume cs : codigo, ds : datos, ss : pila

  AllocateString PROC
    PUSH BP; Guardar el puntero base actual
    MOV BP, SP; Establecer BP como base del marco de pila
    ; Cargar el tamaño del string desde la pila(parámetro)
    ; Comprobar si hay suficiente espacio en MemoriaDinamica
    MOV AX, word ptr NextFree; Cargar el índice de la próxima asignación en AX
    ADD AX, CX; Calcular posición después de la asignación
    CMP AX, 4096; Comparar con el tamaño total de MemoriaDinamica
    JG NoSpace; Si excede 200 bytes, no hay suficiente espacio
    ; Realizar la asignación
    MOV DI, word ptr NextFree; DI apunta al inicio de la asignación actual
    ADD DI, OFFSET MemoriaDinamica; DI ahora apunta al bloque reservado en MemoriaDinamica
    ; Aquí DI es el puntero al bloque asignado que puedes usar para almacenar el string
    ; Actualizar NextFree para la próxima asignación
    MOV AX, word ptr NextFree
    ADD AX, CX; Sumar el tamaño reservado a NextFree
    MOV word ptr NextFree, AX; Guardar el nuevo valor en NextFree
    POP BP; Restaurar el puntero base original
    RET; 
    NoSpace:
    ; Manejo del caso en el que no hay espacio suficiente
    ; (por ejemplo, retornar un valor de error o manejar la situación de otra forma)
    POP BP; Restaurar el puntero base
    RET
  AllocateString ENDP

  ReserveStringOnStack PROC
    ; Este procedimiento recibe el tamaño del string en el registro CX
    ; Guardar el puntero base actual y establecer un nuevo marco de pila
    PUSH BP
    MOV BP, SP
    ; Reservar espacio en la pila para el string
    SUB SP, CX; Restar el valor en CX del puntero de pila para reservar espacio
    POP BP; Restaurar el puntero base original
    RET
  ReserveStringOnStack ENDP


  CopyStringToDynamicMemory PROC
    ; Parámetros de entrada :
    ; DS:SI->Dirección del string de origen en el segmento de datos
    ; DI->Offset en MemoriaDinamica donde iniciar la copia
    PUSH BP; Guardar el puntero base
    MOV BP, SP; Establecer el marco de pila
    ; Configurar ES para apuntar a la dirección base de MemoriaDinamica
    MOV AX, SEG MemoriaDinamica; Cargar el segmento de MemoriaDinamica en AX
    MOV ES, AX; Mover el segmento a ES
    ; Inicio de la copia
    CopyLoop :
    MOV AL, DS:[SI]; Cargar un byte del string de origen en AL
    MOV ES:[DI], AL; Copiar el byte al destino en MemoriaDinamica
    INC SI; Avanzar al siguiente byte en el string de origen
    INC DI; Avanzar al siguiente byte en el destino
    CMP AL, 0; Verificar si es el final del string(byte nulo)
    JNZ CopyLoop; Si no es el final, continuar copiando
    POP BP; Restaurar el puntero base
    RET; Retorno de la función
  CopyStringToDynamicMemory ENDP

  LenString PROC
    ; Parámetros de entrada :
    ; DS:SI->Dirección del string 
    ; Contador queda en CX
    PUSH BP; Guardar el puntero base
    MOV BP, SP; Establecer el marco de pila
    MOV CX, 0; Establecer el marco de pila
    ; Inicio del contador
    ContLoop :
    MOV AL, DS:[SI]; Cargar un byte del string de origen en AL
    INC CX
    INC SI
    CMP AL, 0; Verificar si es el final del string(byte nulo)
    JNZ ContLoop; Si no es el final, continuar contando
    DEC CX
    POP BP; Restaurar el puntero base
    RET; Retorno de la función
  LenString ENDP

  PrintStringFromDynamicMemory PROC
    ; Parámetro de entrada :
    ; DI->Offset en MemoriaDinamica donde comienza el string
    PUSH BP; Guardar el puntero base
    MOV BP, SP; Establecer el marco de pila
    ; Configurar el segmento de datos para MemoriaDinamica
    MOV AX, SEG MemoriaDinamica; Cargar el segmento de MemoriaDinamica en AX
    MOV DS, AX; Mover el segmento a DS
    ; Configurar el offset del string en DX
    MOV DX, DI; Cargar el offset dentro de MemoriaDinamica en DX
    ; Llamar a la interrupción DOS para imprimir el string
    MOV AH, 09h; Función DOS para imprimir un string terminado en $
    INT 21h; Llamar a la interrupción DOS
    POP BP; Restaurar el puntero base
    RET; Retorno del procedimiento
  PrintStringFromDynamicMemory ENDP

  fibonacci proc
  PUSH BP; Guardar el puntero base 
  MOV BP, SP; Establecer el nuevo marco de pila 
    ;N 2
  MOV AX, [BP+4]
  PUSH AX  ;guardó en la pila el valor que está en la variable/constante N
  MOV AX, 2
  PUSH AX
  POP BX
  POP AX
  CMP AX, BX
  JL comp_1
  MOV AX, 0  ; asigna false
  JMP compF_1
comp_1:
  MOV AX, 1  ; asigna true
compF_1:
  PUSH AX  ; Guarda el valor booleano (0 o 1)
  ; Inicia IF después de evaluar expresión
  POP AX  ; Obtener el resultado de la expresión booleana del IF 
  CMP AL, 1  ;verifica si la expresión del IF es verdadera
  JNE etiq_ELSE_2
  MOV AX, [BP+4]
  PUSH AX  ;guardó en la pila el valor que está en la variable/constante N
  JMP etiq_IF_F_2   ; finaliza parte verdadera del if 
etiq_ELSE_2:   ; inicia el else del if
  MOV DX, 0
  PUSH DX   ; espacio para retorno de la funcion 
  MOV AX, [BP+4]
  PUSH AX  ;guardó en la pila el valor que está en la variable/constante N
  MOV AX, 1
  PUSH AX
  POP BX
  POP AX
  SUB AX, BX
  PUSH AX  ; Realiza una resta
  ; Fin de expresion parametro
  CALL fibonacci  ; llamado a Procedimiento o función
  MOV DX, 0
  PUSH DX   ; espacio para retorno de la funcion 
  MOV AX, [BP+4]
  PUSH AX  ;guardó en la pila el valor que está en la variable/constante N
  MOV AX, 2
  PUSH AX
  POP BX
  POP AX
  SUB AX, BX
  PUSH AX  ; Realiza una resta
  ; Fin de expresion parametro
  CALL fibonacci  ; llamado a Procedimiento o función
  POP BX
  POP AX
  ADD AX, BX
  PUSH AX  ; Realiza una suma
etiq_IF_F_2:    ;finaliza else del if 

    ; ... código de la función ... 
  POP AX; 
  MOV [BP+6], AX 
  POP BP; Restaurar el puntero base 
  RET 2  ; Retornar de la función 
  fibonacci endp

main:


Inicio:
  mov ax, ds; Pone al ES a apuntar al inicio del PSP
  mov es, ax  
  mov ax, datos;  El DS que apuntaba al PSP se debe poner a apuntar al segmento de datos para poder usar las variables 
  mov ds, ax  
  mov ax, pila  
  mov ss, ax  

  MOV DX, 0
  PUSH DX   ; espacio para retorno de la funcion 
  MOV AX, 6
  PUSH AX
  ; Fin de expresion parametro
  CALL fibonacci  ; llamado a Procedimiento o función
  POP AX
  MOV word ptr Result, AX   ; Asigna el resultado en la variable Result


  mov ax, 4C00h
  int 21h


codigo endS

end main
