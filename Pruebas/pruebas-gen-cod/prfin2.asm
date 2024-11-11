; Proyecto Compilador Wogawg
; Compiladores e Int�rpretes
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

;AGREGADO##############################################################################################
  buffy db 20 dup( ? ), '$'
  bandera_fib db ' = result fibonacci$'
;AGREGADO##############################################################################################

datos endS

pila segment stack 'stack'
  dw 256 dup(? )
pila endS

codigo segment
  assume cs : codigo, ds : datos, ss : pila

  AllocateString PROC
    PUSH BP; Guardar el puntero base actual
    MOV BP, SP; Establecer BP como base del marco de pila
    ; Cargar el tama�o del string desde la pila(par�metro)
    ; Comprobar si hay suficiente espacio en MemoriaDinamica
    MOV AX, word ptr NextFree; Cargar el �ndice de la pr�xima asignaci�n en AX
    ADD AX, CX; Calcular posici�n despu�s de la asignaci�n
    CMP AX, 4096; Comparar con el tama�o total de MemoriaDinamica
    JG NoSpace; Si excede 200 bytes, no hay suficiente espacio
    ; Realizar la asignaci�n
    MOV DI, word ptr NextFree; DI apunta al inicio de la asignaci�n actual
    ADD DI, OFFSET MemoriaDinamica; DI ahora apunta al bloque reservado en MemoriaDinamica
    ; Aqu� DI es el puntero al bloque asignado que puedes usar para almacenar el string
    ; Actualizar NextFree para la pr�xima asignaci�n
    MOV AX, word ptr NextFree
    ADD AX, CX; Sumar el tama�o reservado a NextFree
    MOV word ptr NextFree, AX; Guardar el nuevo valor en NextFree
    POP BP; Restaurar el puntero base original
    RET; 
    NoSpace:
    ; Manejo del caso en el que no hay espacio suficiente
    ; (por ejemplo, retornar un valor de error o manejar la situaci�n de otra forma)
    POP BP; Restaurar el puntero base
    RET
  AllocateString ENDP

  ReserveStringOnStack PROC
    ; Este procedimiento recibe el tama�o del string en el registro CX
    ; Guardar el puntero base actual y establecer un nuevo marco de pila
    PUSH BP
    MOV BP, SP
    ; Reservar espacio en la pila para el string
    SUB SP, CX; Restar el valor en CX del puntero de pila para reservar espacio
    POP BP; Restaurar el puntero base original
    RET
  ReserveStringOnStack ENDP


  CopyStringToDynamicMemory PROC
    ; Par�metros de entrada :
    ; DS:SI->Direcci�n del string de origen en el segmento de datos
    ; DI->Offset en MemoriaDinamica donde iniciar la copia
    PUSH BP; Guardar el puntero base
    MOV BP, SP; Establecer el marco de pila
    ; Configurar ES para apuntar a la direcci�n base de MemoriaDinamica
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
    RET; Retorno de la funci�n
  CopyStringToDynamicMemory ENDP

  LenString PROC
    ; Par�metros de entrada :
    ; DS:SI->Direcci�n del string 
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
    RET; Retorno de la funci�n
  LenString ENDP

  PrintStringFromDynamicMemory PROC
    ; Par�metro de entrada :
    ; DI->Offset en MemoriaDinamica donde comienza el string
    PUSH BP; Guardar el puntero base
    MOV BP, SP; Establecer el marco de pila
    ; Configurar el segmento de datos para MemoriaDinamica
    MOV AX, SEG MemoriaDinamica; Cargar el segmento de MemoriaDinamica en AX
    MOV DS, AX; Mover el segmento a DS
    ; Configurar el offset del string en DX
    MOV DX, DI; Cargar el offset dentro de MemoriaDinamica en DX
    ; Llamar a la interrupci�n DOS para imprimir el string
    MOV AH, 09h; Funci�n DOS para imprimir un string terminado en $
    INT 21h; Llamar a la interrupci�n DOS
    POP BP; Restaurar el puntero base
    RET; Retorno del procedimiento
  PrintStringFromDynamicMemory ENDP

;AGREGADO##############################################################################################
  ConvertirNumero PROC
      ; Guardar registros y segmentos
      push ax
      push bx
      push dx
      push si
      push ds
      push es

      ; Mover el valor en CX a AX para la conversión
      mov ax, cx

      ; Preparar el puntero para el buffer (comienza al final)
      mov si, offset buffy + 20  ; posición al final del buffer
      mov byte ptr [si], '$'           ; terminador de cadena

  convertir:
      xor dx, dx          ; limpiar DX para la división
      mov bx, 10
      div bx              ; dividir AX entre 10
      add dl, '0'         ; convertir el residuo a ASCII
      dec si              ; mover el puntero hacia atrás
      mov [si], dl        ; almacenar el carácter en el buffer
      cmp ax, 0           ; verificar si AX es cero
      jne convertir       ; si no es cero, continuar dividiendo

      ;imprimir la conversión
      mov ah, 09h
      mov dx, si
      int 21h

      ; Restaurar registros y segmentos
      pop es
      pop ds
      pop si
      pop dx
      pop bx
      pop ax
      ret
  ConvertirNumero ENDP
;AGREGADO##############################################################################################


  fibonacci proc
  PUSH BP; Guardar el puntero base 
  MOV BP, SP; Establecer el nuevo marco de pila 
    ;N 2
  MOV AX, [BP+4]
  PUSH AX  ;guard� en la pila el valor que est� en la variable/constante N
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
  ; Inicia IF despu�s de evaluar expresi�n
  POP AX  ; Obtener el resultado de la expresi�n booleana del IF 
  CMP AL, 1  ;verifica si la expresi�n del IF es verdadera
  JNE etiq_ELSE_2
  MOV AX, [BP+4]
  PUSH AX  ;guard� en la pila el valor que est� en la variable/constante N
  JMP etiq_IF_F_2   ; finaliza parte verdadera del if 
etiq_ELSE_2:   ; inicia el else del if
  MOV DX, 0
  PUSH DX   ; espacio para retorno de la funcion 
  MOV AX, [BP+4]
  PUSH AX  ;guard� en la pila el valor que est� en la variable/constante N
  MOV AX, 1
  PUSH AX
  POP BX
  POP AX
  SUB AX, BX
  PUSH AX  ; Realiza una resta
  ; Fin de expresion parametro
  CALL fibonacci  ; llamado a Procedimiento o funci�n
  MOV DX, 0
  PUSH DX   ; espacio para retorno de la funcion 
  MOV AX, [BP+4]
  PUSH AX  ;guard� en la pila el valor que est� en la variable/constante N
  MOV AX, 2
  PUSH AX
  POP BX
  POP AX
  SUB AX, BX
  PUSH AX  ; Realiza una resta
  ; Fin de expresion parametro
  CALL fibonacci  ; llamado a Procedimiento o funci�n
  POP BX
  POP AX
  ADD AX, BX
  PUSH AX  ; Realiza una suma
etiq_IF_F_2:    ;finaliza else del if 

    ; ... c�digo de la funci�n ... 
  POP AX; 
  MOV [BP+6], AX 
  POP BP; Restaurar el puntero base 
  RET 2  ; Retornar de la funci�n 
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
  CALL fibonacci  ; llamado a Procedimiento o funci�n
  POP AX
  MOV word ptr Result, AX   ; Asigna el resultado en la variable Result

  mov cx, ax
  CALL ConvertirNumero

  mov ah, 09h
  lea dx, bandera_fib
  int 21h

  mov ax, 4C00h
  int 21h


codigo endS

end main
