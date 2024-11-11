; Proyecto Compilador Wogawg
; Compiladores e Intï¿½rpretes
  ; Erick Kauffmann Porcar c.2022180244
  ; Samuel Valverde Arguedas c.2022090162
;------------------ DATA --------------------
datos segment
  MemoriaDinamica db 4096 DUP(?)
  NextFree DW 0
  TmpLenStr DW 0
  TmpSeg DW 0
  TmpOff DW 0
  Num2 dw 35
  Constante1 dw 27
  Constante2 db 'A'
  strPrueba dw ?
  const_strPrueba db '7$', 0
  Constante3 db 'Hola mundo$', 0
  esMayor db 0
  boolConst1 db 0
  Num3 dw 42
  strTest dw ?
  const_strTest db '345$', 0
  esValido db 1
  const_1 db 'MENOR$', 0
  const_2 db 'MAYOR$', 0

;AGREGADO##############################################################################################
  buffy db 20 dup( ? ), '$'
;AGREGADO##############################################################################################

;AGREGADO##############################################################################################
  cambiolinea db 10,13,'$'
;AGREGADO##############################################################################################

;AGREGADO##############################################################################################
  bandera_Const1 db 'Constante1: $'
  bandera_Const2 db 'Constante2: $'
  bandera_Const3 db 'Constante3: $'
  bandera_Num2 db 'Num2: $'
  bandera_Num3 db 'Num3: $'
  bandera_constStrPrueba db 'const_strPrueba: $'
  bandera_constStrTest db 'const_strTest: $'
  bandera_StrPrueba db 'strPrueba: $'
  bandera_StrTest db 'strTest: $'
  bandera_esValido db 'esValido: $'
  bandera_esMayor db 'esMayor: $'
  bandera_boolConst1 db 'boolConst1: $'
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
    ; Cargar el tamaï¿½o del string desde la pila(parï¿½metro)
    ; Comprobar si hay suficiente espacio en MemoriaDinamica
    MOV AX, word ptr NextFree; Cargar el ï¿½ndice de la prï¿½xima asignaciï¿½n en AX
    ADD AX, CX; Calcular posiciï¿½n despuï¿½s de la asignaciï¿½n
    CMP AX, 4096; Comparar con el tamaï¿½o total de MemoriaDinamica
    JG NoSpace; Si excede 200 bytes, no hay suficiente espacio
    ; Realizar la asignaciï¿½n
    MOV DI, word ptr NextFree; DI apunta al inicio de la asignaciï¿½n actual
    ADD DI, OFFSET MemoriaDinamica; DI ahora apunta al bloque reservado en MemoriaDinamica
    ; Aquï¿½ DI es el puntero al bloque asignado que puedes usar para almacenar el string
    ; Actualizar NextFree para la prï¿½xima asignaciï¿½n
    MOV AX, word ptr NextFree
    ADD AX, CX; Sumar el tamaï¿½o reservado a NextFree
    MOV word ptr NextFree, AX; Guardar el nuevo valor en NextFree
    POP BP; Restaurar el puntero base original
    RET; 
    NoSpace:
    ; Manejo del caso en el que no hay espacio suficiente
    ; (por ejemplo, retornar un valor de error o manejar la situaciï¿½n de otra forma)
    POP BP; Restaurar el puntero base
    RET
  AllocateString ENDP

  ReserveStringOnStack PROC
    ; Este procedimiento recibe el tamaï¿½o del string en el registro CX
    ; Guardar el puntero base actual y establecer un nuevo marco de pila
    PUSH BP
    MOV BP, SP
    ; Reservar espacio en la pila para el string
    SUB SP, CX; Restar el valor en CX del puntero de pila para reservar espacio
    POP BP; Restaurar el puntero base original
    RET
  ReserveStringOnStack ENDP


  CopyStringToDynamicMemory PROC
    ; Parï¿½metros de entrada :
    ; DS:SI->Direcciï¿½n del string de origen en el segmento de datos
    ; DI->Offset en MemoriaDinamica donde iniciar la copia
    PUSH BP; Guardar el puntero base
    MOV BP, SP; Establecer el marco de pila
    ; Configurar ES para apuntar a la direcciï¿½n base de MemoriaDinamica
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
    RET; Retorno de la funciï¿½n
  CopyStringToDynamicMemory ENDP

  LenString PROC
    ; Parï¿½metros de entrada :
    ; DS:SI->Direcciï¿½n del string 
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
    RET; Retorno de la funciï¿½n
  LenString ENDP

  PrintStringFromDynamicMemory PROC
    ; Parï¿½metro de entrada :
    ; DI->Offset en MemoriaDinamica donde comienza el string
    PUSH BP; Guardar el puntero base
    MOV BP, SP; Establecer el marco de pila
    ; Configurar el segmento de datos para MemoriaDinamica
    MOV AX, SEG MemoriaDinamica; Cargar el segmento de MemoriaDinamica en AX
    MOV DS, AX; Mover el segmento a DS
    ; Configurar el offset del string en DX
    MOV DX, DI; Cargar el offset dentro de MemoriaDinamica en DX
    ; Llamar a la interrupciï¿½n DOS para imprimir el string
    MOV AH, 09h; Funciï¿½n DOS para imprimir un string terminado en $
    INT 21h; Llamar a la interrupciï¿½n DOS
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
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante N
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
  ; Inicia IF despuï¿½s de evaluar expresiï¿½n
  POP AX  ; Obtener el resultado de la expresiï¿½n booleana del IF 
  CMP AL, 1  ;verifica si la expresiï¿½n del IF es verdadera
  JNE etiq_ELSE_2
  MOV AX, [BP+4]
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante N
  JMP etiq_IF_F_2   ; finaliza parte verdadera del if 
etiq_ELSE_2:   ; inicia el else del if
  MOV DX, 0
  PUSH DX   ; espacio para retorno de la funcion 
  MOV AX, [BP+4]
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante N
  MOV AX, 1
  PUSH AX
  POP BX
  POP AX
  SUB AX, BX
  PUSH AX  ; Realiza una resta
  ; Fin de expresion parametro
  CALL fibonacci  ; llamado a Procedimiento o funciï¿½n
  MOV DX, 0
  PUSH DX   ; espacio para retorno de la funcion 
  MOV AX, [BP+4]
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante N
  MOV AX, 2
  PUSH AX
  POP BX
  POP AX
  SUB AX, BX
  PUSH AX  ; Realiza una resta
  ; Fin de expresion parametro
  CALL fibonacci  ; llamado a Procedimiento o funciï¿½n
  POP BX
  POP AX
  ADD AX, BX
  PUSH AX  ; Realiza una suma
etiq_IF_F_2:    ;finaliza else del if 

    ; ... cï¿½digo de la funciï¿½n ... 
  POP AX; 
  MOV [BP+6], AX 
  POP BP; Restaurar el puntero base 
  RET 2  ; Retornar de la funciï¿½n 
  fibonacci endp

main:


Inicio:
  mov ax, ds; Pone al ES a apuntar al inicio del PSP
  mov es, ax  
  mov ax, datos;  El DS que apuntaba al PSP se debe poner a apuntar al segmento de datos para poder usar las variables 
  mov ds, ax  
  mov ax, pila  
  mov ss, ax  

  MOV AX, NextFree
  MOV strPrueba, AX
  MOV CX, 3; Establece el tamaï¿½o del string a reservar en  bytes
  CALL AllocateString

  MOV AX, SEG const_strPrueba;Cargar el segmento de la constante en DS
  MOV DS, AX
  MOV SI, OFFSET const_strPrueba; Cargar el offset del string de origen en SI
  MOV AX, strPrueba
  MOV DI, AX ;Offset de inicio en MemoriaDinamica
  CALL CopyStringToDynamicMemory; Llamada a la funciï¿½n de copia

  MOV AX, NextFree
  MOV strTest, AX
  MOV CX, 5; Establece el tamaï¿½o del string a reservar en  bytes
  CALL AllocateString

  MOV AX, SEG const_strTest;Cargar el segmento de la constante en DS
  MOV DS, AX
  MOV SI, OFFSET const_strTest; Cargar el offset del string de origen en SI
  MOV AX, strTest
  MOV DI, AX ;Offset de inicio en MemoriaDinamica
  CALL CopyStringToDynamicMemory; Llamada a la funciï¿½n de copia

  MOV AX, word ptr Num2
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante Num2
  POP AX
  MOV word ptr Num3, AX   ; Asigna el resultado en la variable Num3

  MOV AX, SEG Constante3
  PUSH AX
  MOV AX, OFFSET Constante3
  PUSH AX  ;guardï¿½ en la pila el segment:offset del string Constante3
  ;toma de la pila el segment y offset del string, y cuenta cuï¿½ntos caracteres tiene
  POP AX
  MOV word ptr TmpOff, AX
  MOV SI, AX
  POP AX
  MOV word ptr TmpSeg, AX
  MOV DS, AX
  CALL LenString
  MOV word ptr TmpLenStr, CX

  ;reserva el espacio para el nuevo string
  MOV AX, word ptr NextFree
  MOV strPrueba, AX
  MOV CX, word ptr TmpLenStr
  INC CX   ;  hay que dejar el espacio para el 0 final
  CALL AllocateString
  ;copia el string del lado derecho de la asignaciï¿½n a la Memoria Dinamica
  MOV AX, word ptr TmpSeg ; Cargar el segmento de la constante en DS
  MOV DS, AX
  MOV SI, word ptr TmpOff ; Cargar el offset del string de origen en SI
  MOV AX, strPrueba
  MOV DI, AX ;Offset de inicio en MemoriaDinamica
  CALL CopyStringToDynamicMemory; Llamada a la funciï¿½n de copia

  MOV AX, word ptr Num3
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante Num3
  MOV AX, 5
  PUSH AX
  POP BX
  POP AX
  CMP AX, BX
  JG comp_3
  MOV AX, 0  ; asigna false
  JMP compF_3
comp_3:
  MOV AX, 1  ; asigna true
compF_3:
  PUSH AX  ; Guarda el valor booleano (0 o 1)
  MOV AX, 0
  MOV AL, byte ptr esValido
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante esValido
  POP BX
  POP AX
  AND AX, BX
  PUSH AX  ; Realiza un AND
  MOV AX, 0
  MOV AL, byte ptr boolConst1
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante boolConst1
  POP BX
  POP AX
  OR AX, BX
  PUSH AX  ; Realiza un OR
  POP AX
  MOV byte ptr esMayor, AL   ; Asigna el resultado en la variable esMayor

  MOV AX, word ptr Num2
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante Num2
  MOV AX, 2
  PUSH AX
  POP BX
  POP AX
  CMP AX, BX
  JL comp_4
  MOV AX, 0  ; asigna false
  JMP compF_4
comp_4:
  MOV AX, 1  ; asigna true
compF_4:
  PUSH AX  ; Guarda el valor booleano (0 o 1)
  ; Inicia IF despuï¿½s de evaluar expresiï¿½n
  POP AX  ; Obtener el resultado de la expresiï¿½n booleana del IF 
  CMP AL, 1  ;verifica si la expresiï¿½n del IF es verdadera
  JNE etiq_ELSE_5
  MOV AX, 5
  PUSH AX
  POP AX
  MOV word ptr Num2, AX   ; Asigna el resultado en la variable Num2

  MOV AX, SEG const_1
  PUSH AX
  MOV AX, OFFSET const_1
  PUSH AX
  ;toma de la pila el segment y offset del string, y cuenta cuï¿½ntos caracteres tiene
  POP AX
  MOV word ptr TmpOff, AX
  MOV SI, AX
  POP AX
  MOV word ptr TmpSeg, AX
  MOV DS, AX
  CALL LenString
  MOV word ptr TmpLenStr, CX

  ;reserva el espacio para el nuevo string
  MOV AX, word ptr NextFree
  MOV strPrueba, AX
  MOV CX, word ptr TmpLenStr
  INC CX   ;  hay que dejar el espacio para el 0 final
  CALL AllocateString
  ;copia el string del lado derecho de la asignaciï¿½n a la Memoria Dinamica
  MOV AX, word ptr TmpSeg ; Cargar el segmento de la constante en DS
  MOV DS, AX
  MOV SI, word ptr TmpOff ; Cargar el offset del string de origen en SI
  MOV AX, strPrueba
  MOV DI, AX ;Offset de inicio en MemoriaDinamica
  CALL CopyStringToDynamicMemory; Llamada a la funciï¿½n de copia

  JMP etiq_IF_F_5   ; finaliza parte verdadera del if 
etiq_ELSE_5:   ; inicia el else del if
  MOV AX, 2
  PUSH AX
  MOV AX, 7
  PUSH AX
  MOV AX, 3
  PUSH AX
  POP BX
  POP AX
  MUL BX
  PUSH AX  ; Realiza una multiplicacion
  POP BX
  POP AX
  ADD AX, BX
  PUSH AX  ; Realiza una suma
  POP AX
  MOV word ptr Num2, AX   ; Asigna el resultado en la variable Num2

  MOV AX, SEG const_2
  PUSH AX
  MOV AX, OFFSET const_2
  PUSH AX
  ;toma de la pila el segment y offset del string, y cuenta cuï¿½ntos caracteres tiene
  POP AX
  MOV word ptr TmpOff, AX
  MOV SI, AX
  POP AX
  MOV word ptr TmpSeg, AX
  MOV DS, AX
  CALL LenString
  MOV word ptr TmpLenStr, CX

  ;reserva el espacio para el nuevo string
  MOV AX, word ptr NextFree
  MOV strPrueba, AX
  MOV CX, word ptr TmpLenStr
  INC CX   ;  hay que dejar el espacio para el 0 final
  CALL AllocateString
  ;copia el string del lado derecho de la asignaciï¿½n a la Memoria Dinamica
  MOV AX, word ptr TmpSeg ; Cargar el segmento de la constante en DS
  MOV DS, AX
  MOV SI, word ptr TmpOff ; Cargar el offset del string de origen en SI
  MOV AX, strPrueba
  MOV DI, AX ;Offset de inicio en MemoriaDinamica
  CALL CopyStringToDynamicMemory; Llamada a la funciï¿½n de copia

etiq_IF_F_5:    ;finaliza else del if 

etiq_WHILE_6:
  ; inicio del while
  MOV AX, word ptr Num2
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante Num2
  MOV AX, 50
  PUSH AX
  POP BX
  POP AX
  CMP AX, BX
  JL comp_7
  MOV AX, 0  ; asigna false
  JMP compF_7
comp_7:
  MOV AX, 1  ; asigna true
compF_7:
  PUSH AX  ; Guarda el valor booleano (0 o 1)
  POP AX  ; el resultado de la evaluaciï¿½n de la expresiï¿½n booleana de un while 
  CMP AL, 1
  JNE etiq_WHILE_F_8
  MOV AX, word ptr Num2
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante Num2
  MOV AX, 1
  PUSH AX
  POP BX
  POP AX
  ADD AX, BX
  PUSH AX  ; Realiza una suma
  POP AX
  MOV word ptr Num2, AX   ; Asigna el resultado en la variable Num2

  MOV AX, word ptr Num3
  PUSH AX  ;guardï¿½ en la pila el valor que estï¿½ en la variable/constante Num3
  MOV AX, 1
  PUSH AX
  POP BX
  POP AX
  SUB AX, BX
  PUSH AX  ; Realiza una resta
  POP AX
  MOV word ptr Num3, AX   ; Asigna el resultado en la variable Num3

  JMP etiq_WHILE_6    ;inicie el ciclo del while nuevamente
etiq_WHILE_F_8:    ;fin del while

  MOV DX, 0
  PUSH DX   ; espacio para retorno de la funcion 
  MOV AX, 4
  PUSH AX
  ; Fin de expresion parametro
  CALL fibonacci  ; llamado a Procedimiento o funciï¿½n
  POP AX
  MOV word ptr Num2, AX   ; Asigna el resultado en la variable Num2

;AGREGADO##############################################################################################
   mov cx, ax
  CALL ConvertirNumero

  mov ah, 09h
  lea dx, bandera_fib
  int 21h

  mov ah, 09h
  lea dx, cambiolinea
  int 21h

  mov ah, 09h
	lea dx, bandera_Const1
	int 21h

  mov cx, word ptr Constante1
  CALL ConvertirNumero

  mov ah, 09h
	lea dx, cambiolinea
	int 21h


  mov ah, 09h
	lea dx, bandera_Const2
	int 21h

  mov ah, 09h
	lea dx, Constante2
	int 21h

  mov ah, 09h
	lea dx, cambiolinea
	int 21h


  mov ah, 09h
	lea dx, bandera_Const3
	int 21h

  mov ah, 09h
	lea dx, Constante3
	int 21h

  mov ah, 09h
	lea dx, cambiolinea
	int 21h

  mov ah, 09h
	lea dx, bandera_boolConst1
	int 21h

  xor cx, cx
  mov cl, byte ptr boolConst1
  CALL ConvertirNumero

  mov ah, 09h
	lea dx, cambiolinea
	int 21h


  mov ah, 09h
	lea dx, bandera_Num2
	int 21h

  mov cx, word ptr Num2
  CALL ConvertirNumero

  mov ah, 09h
	lea dx, cambiolinea
	int 21h


  mov ah, 09h
	lea dx, bandera_Num3
	int 21h

  mov cx, word ptr Num3
  CALL ConvertirNumero

  mov ah, 09h
	lea dx, cambiolinea
	int 21h


  mov ah, 09h
	lea dx, bandera_StrPrueba
	int 21h

  ;mov ah, 09h
	;lea dx, strPrueba
	;int 21h
  mov ax, SEG MemoriaDinamica
  mov ds, ax
  mov dx, strPrueba
  mov ah, 09h
  int 21h

  mov ah, 09h
	lea dx, cambiolinea
	int 21h


  mov ah, 09h
	lea dx, bandera_constStrPrueba
	int 21h

  mov ah, 09h
	lea dx, const_strPrueba
	int 21h

  mov ah, 09h
	lea dx, cambiolinea
	int 21h


  mov ah, 09h
	lea dx, bandera_StrTest
	int 21h

  ;mov ah, 09h
	;lea dx, strTest
	;int 21h
  mov ax, SEG MemoriaDinamica
  mov ds, ax
  mov dx, strTest
  mov ah, 09h
  int 21h

  mov ah, 09h
	lea dx, cambiolinea
	int 21h


  mov ah, 09h
	lea dx, bandera_constStrTest
	int 21h

  mov ah, 09h
	lea dx, const_strTest
	int 21h

  mov ah, 09h
	lea dx, cambiolinea
	int 21h


  mov ah, 09h
	lea dx, bandera_esMayor
	int 21h

  xor cx, cx
  mov cl, byte ptr esMayor
  CALL ConvertirNumero

  mov ah, 09h
	lea dx, cambiolinea
	int 21h


  mov ah, 09h
	lea dx, bandera_esValido
	int 21h

  xor cx, cx
  mov cl, byte ptr esValido
  CALL ConvertirNumero

  mov ah, 09h
	lea dx, cambiolinea
	int 21h

  mov ah, 09h
  lea dx, cambiolinea
  int 21h


  
;AGREGADO##############################################################################################

  

  mov ax, 4C00h
  int 21h


codigo endS

end main
