#include "CParser.h"

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <fstream>
#include "GRAMATIC.h"
#include "FOLLOW.h"
#include "CToken.h"
#include "CScanner.h"
#include "TreeNode.h"
#include "CSymbolInfo.h"
#include "CSymbolTable.h"

using namespace std;
using namespace ScannerLib;

//validar parametros en un llamado a una func o subrutina: cantidad y match de tipos
//validar existan las variables, constantes y funciones donde se usan
//el prototipo y la definicion de la función si se puede repetir el nombre
//falta revisar los valores iniciales de un array
//falta revisar los valores iniciales de un registro
//falta guardar los valores iniciales de un array
//falta guardar los valores iniciales de un registro.
namespace ParserLib {

	const int tam_memoria = 4096;
	int globalConstLiterales = 1;
	int globalEtiquetas = 1;

	bool isTerminal(int prod) {
		return TERMINAL(prod);
	}

	bool isSemanticRule(int prod) {
		return prod >= RS1001;
	}

	string getStrProduccion(int prod) {
		if (isTerminal(prod)) {
			Token token;
			token.setTipoToken((TipoToken)prod);
			return token.toString();
		}
		else if (isSemanticRule(prod)) {
			return "SR:" + std::to_string(prod);
		}
		else {
			return ListaNoTerminales[prod - SIMBOLOINICIAL];
		}
	}

	bool foundTokenIn(Token token, int top) {

		bool found = false;
		string infoTop = getStrProduccion(top);
		TipoToken tipoToken = token.getTipoToken();
		for (int i = 0; i < MAXFOLLOWS; i++) {
			if (TablaFollows[top - SIMBOLOINICIAL][i] == (int)tipoToken) {
				found = true;
				break;
			}
			else if (TablaFollows[top - SIMBOLOINICIAL][i] == -1) {
				break;
			}
		}
		return found;
	}

	string getTypeFromValue(string value) {
		string result = value;
		if (value == "TOKEN_INT") { result = "TOKEN_SHTAWA"; }
		else if (value == "TOKEN_CHAR") { result = "TOKEN_EKLA"; }
		else if (value == "TOKEN_STRING") { result = "TOKEN_SHEJ"; }
		else if (value == "TOKEN_YINAKULIWA") { result = "TOKEN_YINAKULIWA"; }
		else if (value == "TOKEN_SIWA") { result = "TOKEN_YINAKULIWA"; }
		else if (value == "TOKEN_KOYUWE") { result = "TOKEN_YINAKULIWA"; }
		else if (value == "TOKEN_ROMANO") { result = "TOKEN_WOMELE"; }
		else if (value == "TOKEN_ABRIR_CONJUNTO") { result = "TOKEN_JARJA"; }
		return result;
	}

	TreeNode * SearchInChildrenByData(TreeNode * parent, string data) {
		TreeNode * result = NULL;
		vector<TreeNode *> children = parent->GetChildren();
		for (TreeNode* child : children) {
			if (child->GetData() == data) {
				result = child;
				break;
			}
		}
		return result;
	}

	TreeNode * SearchPreviousSiblingNode(TreeNode * parent, TreeNode * node) {
		TreeNode * result = NULL;
		vector<TreeNode *> children = parent->GetChildren();
		TreeNode * previous = NULL;
		for (TreeNode* child : children) {
			if (child == node) {
				result = previous;
				break;
			}
			if (!isSemanticRule(child->GetIdRegla())) {
				previous = child;
			}
		}
		return result;
	}

	void CParser::fnGC0001(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			//(*genCodeFile) << ".model small\n";
			//(*genCodeFile) << ".stack 100h\n";
			(*genCodeFile) << "pila segment stack 'stack'\n";
			(*genCodeFile) << "  dw 256 dup(? )\n";
			(*genCodeFile) << "pila endS\n\n";

			(*genCodeFile) << "codigo segment\n";
			(*genCodeFile) << "  assume cs : codigo, ds : datos, ss : pila\n\n";

			//para llamar a AllocateString:
			//MOV CX, 50             ; Establece el tamaño del string a reservar en 50 bytes
			//CALL AllocateString
			(*genCodeFile) << "  AllocateString PROC\n";
			(*genCodeFile) << "    PUSH BP; Guardar el puntero base actual\n";
			(*genCodeFile) << "    MOV BP, SP; Establecer BP como base del marco de pila\n";
			(*genCodeFile) << "    ; Cargar el tamaño del string desde la pila(parámetro)\n";
			//(*genCodeFile) << "    MOV CX, [BP + 4]; CX contiene el tamaño del string solicitado\n";
			(*genCodeFile) << "    ; Comprobar si hay suficiente espacio en MemoriaDinamica\n";
			(*genCodeFile) << "    MOV AX, word ptr NextFree; Cargar el índice de la próxima asignación en AX\n";
			(*genCodeFile) << "    ADD AX, CX; Calcular posición después de la asignación\n";
			(*genCodeFile) << "    CMP AX, " + to_string(tam_memoria) + "; Comparar con el tamaño total de MemoriaDinamica\n";
			(*genCodeFile) << "    JG NoSpace; Si excede 200 bytes, no hay suficiente espacio\n";
			(*genCodeFile) << "    ; Realizar la asignación\n";
			(*genCodeFile) << "    MOV DI, word ptr NextFree; DI apunta al inicio de la asignación actual\n";
			(*genCodeFile) << "    ADD DI, OFFSET MemoriaDinamica; DI ahora apunta al bloque reservado en MemoriaDinamica\n";
			(*genCodeFile) << "    ; Aquí DI es el puntero al bloque asignado que puedes usar para almacenar el string\n";
			(*genCodeFile) << "    ; Actualizar NextFree para la próxima asignación\n";
			(*genCodeFile) << "    MOV AX, word ptr NextFree\n";
			(*genCodeFile) << "    ADD AX, CX; Sumar el tamaño reservado a NextFree\n";
			(*genCodeFile) << "    MOV word ptr NextFree, AX; Guardar el nuevo valor en NextFree\n";
			(*genCodeFile) << "    POP BP; Restaurar el puntero base original\n";
			(*genCodeFile) << "    RET; \n";
			(*genCodeFile) << "    NoSpace:\n";
			(*genCodeFile) << "    ; Manejo del caso en el que no hay espacio suficiente\n";
			(*genCodeFile) << "    ; (por ejemplo, retornar un valor de error o manejar la situación de otra forma)\n";
			(*genCodeFile) << "    POP BP; Restaurar el puntero base\n";
			(*genCodeFile) << "    RET\n";
			(*genCodeFile) << "  AllocateString ENDP\n\n";

			//para llamar a ReserveStringOnStack 
			//MOV CX, 50             ; Establece el tamaño del string a reservar en 50 bytes en la pila
			//CALL AllocateString
			(*genCodeFile) << "  ReserveStringOnStack PROC\n";
			(*genCodeFile) << "    ; Este procedimiento recibe el tamaño del string en el registro CX\n";
			(*genCodeFile) << "    ; Guardar el puntero base actual y establecer un nuevo marco de pila\n";
			(*genCodeFile) << "    PUSH BP\n";
			(*genCodeFile) << "    MOV BP, SP\n";
			(*genCodeFile) << "    ; Reservar espacio en la pila para el string\n";
			(*genCodeFile) << "    SUB SP, CX; Restar el valor en CX del puntero de pila para reservar espacio\n";


			//(*genCodeFile) << "    ; En este punto, SP apunta al inicio del espacio reservado en la pila.\n";
			//(*genCodeFile) << "    ; Puedes usar SP o[BP - CX] como puntero al espacio de string reservado.\n";
			//(*genCodeFile) << "    ; ... (código que utiliza el espacio reservado) ...\n";
			//(*genCodeFile) << "    ; Ejemplo: almacenar datos en el espacio reservado\n"; 
			//(*genCodeFile) << "    ; MOV[SP], 'H'; Colocar una letra en el inicio del espacio reservado\n";
			//(*genCodeFile) << "    ; Liberar el espacio reservado antes de salir\n";
			//(*genCodeFile) << "    ADD SP, CX; Restaurar SP al valor original liberando el espacio reservado\n";

			(*genCodeFile) << "    POP BP; Restaurar el puntero base original\n";
			(*genCodeFile) << "    RET\n";
			(*genCodeFile) << "  ReserveStringOnStack ENDP\n\n\n";

			//Para llamar a CopyStringToDynamicMemory
			//MOV AX, SEG SourceString; Cargar el segmento de SourceString en DS
			//MOV DS, AX
			//MOV SI, OFFSET SourceString; Cargar el offset del string de origen en SI
			//MOV DI, 20; Offset de inicio en MemoriaDinamica
			//CALL CopyStringToDynamicMemory; Llamada a la función de copia
			(*genCodeFile) << "  CopyStringToDynamicMemory PROC\n";
			(*genCodeFile) << "    ; Parámetros de entrada :\n";
			(*genCodeFile) << "    ; DS:SI->Dirección del string de origen en el segmento de datos\n";
			(*genCodeFile) << "    ; DI->Offset en MemoriaDinamica donde iniciar la copia\n";

			(*genCodeFile) << "    PUSH BP; Guardar el puntero base\n";
			(*genCodeFile) << "    MOV BP, SP; Establecer el marco de pila\n";

			(*genCodeFile) << "    ; Configurar ES para apuntar a la dirección base de MemoriaDinamica\n";
			(*genCodeFile) << "    MOV AX, SEG MemoriaDinamica; Cargar el segmento de MemoriaDinamica en AX\n";
			(*genCodeFile) << "    MOV ES, AX; Mover el segmento a ES\n";

			(*genCodeFile) << "    ; Inicio de la copia\n";
			(*genCodeFile) << "    CopyLoop :\n";
			(*genCodeFile) << "    MOV AL, DS:[SI]; Cargar un byte del string de origen en AL\n";
			(*genCodeFile) << "    MOV ES:[DI], AL; Copiar el byte al destino en MemoriaDinamica\n";
			(*genCodeFile) << "    INC SI; Avanzar al siguiente byte en el string de origen\n";
			(*genCodeFile) << "    INC DI; Avanzar al siguiente byte en el destino\n";
			(*genCodeFile) << "    CMP AL, 0; Verificar si es el final del string(byte nulo)\n";
			(*genCodeFile) << "    JNZ CopyLoop; Si no es el final, continuar copiando\n";

			(*genCodeFile) << "    POP BP; Restaurar el puntero base\n";
			(*genCodeFile) << "    RET; Retorno de la función\n";
			(*genCodeFile) << "  CopyStringToDynamicMemory ENDP\n\n";

			//Para llamar a LenString
			//MOV AX, SEG string; Cargar el segmento de SourceString en DS
			//MOV DS, AX
			//MOV SI, OFFSET string; Cargar el offset del string de origen en SI
			//CALL LenString; Llamada a la función de copia
			//el resultado queda en CX
			(*genCodeFile) << "  LenString PROC\n";
			(*genCodeFile) << "    ; Parámetros de entrada :\n";
			(*genCodeFile) << "    ; DS:SI->Dirección del string \n";
			(*genCodeFile) << "    ; Contador queda en CX\n";

			(*genCodeFile) << "    PUSH BP; Guardar el puntero base\n";
			(*genCodeFile) << "    MOV BP, SP; Establecer el marco de pila\n";

			(*genCodeFile) << "    MOV CX, 0; Establecer el marco de pila\n";
			(*genCodeFile) << "    ; Inicio del contador\n";
			(*genCodeFile) << "    ContLoop :\n";
			(*genCodeFile) << "    MOV AL, DS:[SI]; Cargar un byte del string de origen en AL\n";
			(*genCodeFile) << "    INC CX\n";
			(*genCodeFile) << "    INC SI\n";
			(*genCodeFile) << "    CMP AL, 0; Verificar si es el final del string(byte nulo)\n";
			(*genCodeFile) << "    JNZ ContLoop; Si no es el final, continuar contando\n";
			(*genCodeFile) << "    DEC CX\n";

			(*genCodeFile) << "    POP BP; Restaurar el puntero base\n";
			(*genCodeFile) << "    RET; Retorno de la función\n";
			(*genCodeFile) << "  LenString ENDP\n\n";


			(*genCodeFile) << "  PrintStringFromDynamicMemory PROC\n";
			(*genCodeFile) << "    ; Parámetro de entrada :\n";

			(*genCodeFile) << "    ; DI->Offset en MemoriaDinamica donde comienza el string\n";

			(*genCodeFile) << "    PUSH BP; Guardar el puntero base\n";
			(*genCodeFile) << "    MOV BP, SP; Establecer el marco de pila\n";

			(*genCodeFile) << "    ; Configurar el segmento de datos para MemoriaDinamica\n";
			(*genCodeFile) << "    MOV AX, SEG MemoriaDinamica; Cargar el segmento de MemoriaDinamica en AX\n";
			(*genCodeFile) << "    MOV DS, AX; Mover el segmento a DS\n";

			(*genCodeFile) << "    ; Configurar el offset del string en DX\n";
			(*genCodeFile) << "    MOV DX, DI; Cargar el offset dentro de MemoriaDinamica en DX\n";

			(*genCodeFile) << "    ; Llamar a la interrupción DOS para imprimir el string\n";
			(*genCodeFile) << "    MOV AH, 09h; Función DOS para imprimir un string terminado en $\n";
			(*genCodeFile) << "    INT 21h; Llamar a la interrupción DOS\n";

			(*genCodeFile) << "    POP BP; Restaurar el puntero base\n";
			(*genCodeFile) << "    RET; Retorno del procedimiento\n";
			(*genCodeFile) << "  PrintStringFromDynamicMemory ENDP\n\n";

		}
	}

	void CParser::fnGC0002(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		int defData = 0;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {

			(*genCodeFile) << "main:\n\n\n";
			//asignar los valores a los espacios de memoria de las variables o constantes.
			//memoryTable->genCodForConstVarValues(this->genCodeFile);

			(*genCodeFile) << "Inicio:\n";

			(*genCodeFile) << "  mov ax, ds; Pone al ES a apuntar al inicio del PSP\n";
			(*genCodeFile) << "  mov es, ax  \n";

			(*genCodeFile) << "  mov ax, datos;  El DS que apuntaba al PSP se debe poner a apuntar al segmento de datos para poder usar las variables \n";
			(*genCodeFile) << "  mov ds, ax  \n";

			(*genCodeFile) << "  mov ax, pila  \n";
			(*genCodeFile) << "  mov ss, ax  \n\n";

			if (currentSymbolTable != NULL && currentSymbolTable->parent == NULL) {
				//variables globales
				(*genCodeFile) << currentSymbolTable->genCodeAssignConstString();
			}

			//definiendo variables globales
			if (currentSymbolTable->parent == NULL) {
				//currentSymbolTable->ReserveConstVarMemory(memoryTable);
				currentSymbolTable->genVarCode(memoryTable);
			}
		}
	}

	void CParser::fnGC0003(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			(*genCodeFile) << "\n  mov ax, 4C00h\n";
			(*genCodeFile) << "  int 21h\n\n";

			(*genCodeFile) << "\ncodigo endS\n\n";
			(*genCodeFile) << "end main\n";		

			memoryTable->addDataConstVar("datos endS\n");
		}
	}

	void CParser::fnGC0004(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			TreeNode * nodeProcId = SearchInChildrenByData(parent, "TOKEN_ID");
			if (nodeProcId != NULL)
			{
				(*genCodeFile) << "  " + nodeProcId->GetChildren()[0]->GetValue() + " proc\n";

				(*genCodeFile) << "  PUSH BP; Guardar el puntero base \n";
				(*genCodeFile) << "  MOV BP, SP; Establecer el nuevo marco de pila \n"; 
/*
				int offset = 4;//el retorno son los primeros 4 bytes de la pila
				TreeNode * defParams = SearchInChildrenByData(parent, "<DefFuncFormalParams>");
				if (defParams == NULL) {
					defParams = SearchInChildrenByData(parent, "<DefProcFormalParams>");
				}
				if (defParams != NULL) {
					while (defParams != NULL) {
						TreeNode * nodeTipoSimple = SearchInChildrenByData(defParams, "<TipoSimple>");
						//aqui sé cual es el tipo (tamaño a reservar en la pila
						int tipo_size = 2;//por el momento asumo enteros de 2 bytes. 
						if (nodeTipoSimple != NULL) {
							TreeNode * Ids = SearchInChildrenByData(defParams, "<ListaIDs>");
							if (Ids == NULL) {
								Ids = SearchInChildrenByData(defParams, "<ListaProcIDs>");
							}
							while (Ids != NULL) {
								TreeNode * nombreId = SearchInChildrenByData(Ids, "TOKEN_ID");
								if (nombreId != NULL) {
									//aqui sé los nombres para saber en qué posición de la pila queda el parametro
									//(*genCodeFile) << "  ; Acceso a los parámetros : \n";
									//(*genCodeFile) << "  ; Suponiendo que el primer parámetro(a) está en BP + 4 \n";
									//(*genCodeFile) << "  ; y el segundo parámetro(b) está en BP + 6 \n";
									string strId = nombreId->GetChildren()[0]->GetValue();
									(*genCodeFile) << "  ; id = " + strId + " = [BP + " + (to_string(offset)) + "]; Cargar el parámetro a en AX \n";
									offset += tipo_size;
								}
								TreeNode * IdsTmp = SearchInChildrenByData(Ids, "<SiguientesListaIDs>");
								if (IdsTmp == NULL) {
									IdsTmp = SearchInChildrenByData(Ids, "<SiguientesListaProcIDs>");
								}
								Ids = IdsTmp;
								if (Ids != NULL) {
									IdsTmp = SearchInChildrenByData(Ids, "<ListaIDs>");
									if (IdsTmp == NULL) {
										IdsTmp = SearchInChildrenByData(Ids, "<ListaProcIDs>");
									}
									Ids = IdsTmp;
								}
							}
						}
						TreeNode * defParamsTmp = SearchInChildrenByData(defParams, "<SigDefFuncFormalParams>");
						if (defParamsTmp == NULL) {
							defParamsTmp = SearchInChildrenByData(defParams, "<SigDefProcFormalParams>");
						}
						defParams = defParamsTmp;

						if (defParams != NULL) {
							defParamsTmp = SearchInChildrenByData(defParams, "<DefFuncFormalParams>");
							if (defParamsTmp == NULL) {
								defParamsTmp = SearchInChildrenByData(defParams, "<DefProcFormalParams>");
							}
							defParams = defParamsTmp;
						}
					}
				}
*/
			}

			currentSymbolTable->genParamsCode(this->genCodeFile);
		}
	}

	void CParser::fnGC0005(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			TreeNode * nodeProcId = SearchInChildrenByData(parent, "TOKEN_ID");
			if (nodeProcId != NULL)
			{
				CSymbolInfo * symbol = currentSymbolTable->getSymbol(nodeProcId->GetChildren()[0]->GetValue(), true);
				if (symbol != NULL)
				{
					(*genCodeFile) << "    ; ... código de la función ... \n";

					if (symbol->getSymbolType() == ST_Funcion) {

						//contar el espacio de los parámetros.
						int cantBytes = symbol->getFormalParamsSize();

						(*genCodeFile) << "  POP AX; \n";
						(*genCodeFile) << "  MOV [BP+" + to_string(4 + cantBytes) + "], AX \n";

						(*genCodeFile) << "  POP BP; Restaurar el puntero base \n";


						if (cantBytes > 0) {
							(*genCodeFile) << "  RET " + to_string(cantBytes) + "  ; Retornar de la función \n";
						}
						else {
							(*genCodeFile) << "  RET; Retornar de la función \n";
						}
					}
					else {
						(*genCodeFile) << "  POP BP; Restaurar el puntero base \n";
						(*genCodeFile) << "  RET; Retornar de la función \n";
					}

					//????? Ojo antes del Retorno debe meter el resultado de la función, y en el retorno debe liberar la pila
					(*genCodeFile) << "  " + nodeProcId->GetChildren()[0]->GetValue() + " endp\n\n";
				}
			}
		}
	}

	void CParser::fnGC0006(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {

		}
	}

	void CParser::fnGC0007(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			if (parent->GetData() == "<FactorIzq>") {
				TreeNode * node = SearchInChildrenByData(parent, "<LiteralConst>");
				if (node != NULL) {
					if (node->GetType() == "TOKEN_SHTAWA") {
						//integer
						(*genCodeFile) << "  MOV AX, " + node->GetChildren()[0]->GetValue() + "\n";
						(*genCodeFile) << "  PUSH AX\n";
					}
					else if (node->GetType() == "TOKEN_EKLA") {
						(*genCodeFile) << "  MOV AX, " + node->GetChildren()[0]->GetValue() + "\n";
						(*genCodeFile) << "  PUSH AX\n";
					}
					else if (node->GetType() == "TOKEN_YINAKULIWA") {
						string valBool = ((node->GetValue() == "Síwa" || node->GetValue() == "síwa") ? "1" : "0");
						(*genCodeFile) << "  MOV AX, " + valBool + "\n";
						(*genCodeFile) << "  PUSH AX\n";
					}
					else if (node->GetType() == "TOKEN_SHEJ") {
						//agregarlo a las constantes y luego usarlo
						string nombreVar = "const_" + to_string(globalConstLiterales);
						globalConstLiterales += 1;
						string dato = "  " + nombreVar + " " + "db " + CSymbolTable::ReplaceAll(node->GetChildren()[0]->GetValue(), "\"", "'") + ", 0";
						memoryTable->addDataConstVar(dato);//se crea la variable para guardar el offset
						//para un string se guarda el segmento y el offset, como es una constante
						(*genCodeFile) << "  MOV AX, SEG " + nombreVar + "\n";
						(*genCodeFile) << "  PUSH AX\n";
						(*genCodeFile) << "  MOV AX, OFFSET " + nombreVar + "\n";;
						(*genCodeFile) << "  PUSH AX\n";
					}
				}
				else {
					node = SearchInChildrenByData(parent, "TOKEN_ID");
					if (node != NULL) {
						bool esParamVarLocal = false;
						//variable
						node = node->GetChildren()[0];
						CSymbolInfo * symbol = currentSymbolTable->getSymbol(node->GetValue());
						if (symbol == NULL) {
							if (currentSymbolTable->parent != NULL) {
								symbol = currentSymbolTable->getSymbol(node->GetValue(), true);
								esParamVarLocal = false;
							}
						}
						else {
							if (currentSymbolTable->parent != NULL) {
								esParamVarLocal = true;
							}
							else {
								esParamVarLocal = false;
							}
						}
						if (symbol != NULL) {
							if (symbol->getSymbolType() == ST_Constante || symbol->getSymbolType() == ST_Variable) {
								string tipo = symbol->getType();
								if (tipo == "TOKEN_SHTAWA") {
									//integer
									if (esParamVarLocal) {
										(*genCodeFile) << "  MOV AX, [BP+" + to_string(4 + symbol->getPosPila()) + "]\n";
									}
									else {
										(*genCodeFile) << "  MOV AX, word ptr " + node->GetValue() + "\n";
									}
									(*genCodeFile) << "  PUSH AX  ;guardó en la pila el valor que está en la variable/constante " + node->GetValue() + "\n";
								}
								else if (tipo == "TOKEN_EKLA") {
									//char
									if (esParamVarLocal) {
										(*genCodeFile) << "  MOV AX, [BP+" + to_string(4 + symbol->getPosPila()) + "]\n";
									}
									else {
										(*genCodeFile) << "  MOV AX, 0\n";
										(*genCodeFile) << "  MOV AL, byte ptr " + node->GetValue() + "\n";
									}
									(*genCodeFile) << "  PUSH AX  ;guardó en la pila el valor que está en la variable/constante " + node->GetValue() + "\n";
								}
								else if (tipo == "TOKEN_YINAKULIWA") {
									//bool
									if (esParamVarLocal) {
										(*genCodeFile) << "  MOV AX, [BP+" + to_string(4 + symbol->getPosPila()) + "]\n";
									}
									else {
										(*genCodeFile) << "  MOV AX, 0\n";
										(*genCodeFile) << "  MOV AL, byte ptr " + node->GetValue() + "\n";
									}
									(*genCodeFile) << "  PUSH AX  ;guardó en la pila el valor que está en la variable/constante " + node->GetValue() + "\n";
								}
								else if (tipo == "TOKEN_SHEJ") {
									//para un string se guarda el segmento y el offset, como es una constante
									if (symbol->getSymbolType() == ST_Constante) {
										(*genCodeFile) << "  MOV AX, SEG " + node->GetValue() + "\n";
										(*genCodeFile) << "  PUSH AX\n";
										(*genCodeFile) << "  MOV AX, OFFSET " + node->GetValue() + "\n";
										(*genCodeFile) << "  PUSH AX  ;guardó en la pila el segment:offset del string " + node->GetValue() + "\n";
									}
									else if (symbol->getSymbolType() == ST_Variable) {
										if (esParamVarLocal) {
											(*genCodeFile) << "  MOV AX, [BP+" + to_string(4 + symbol->getPosPila()) + "]\n";
											(*genCodeFile) << "  PUSH AX\n";
											(*genCodeFile) << "  MOV AX, [BP+" + to_string(4 + symbol->getPosPila() + 2) + "]\n";
											(*genCodeFile) << "  PUSH AX\n";
										}
										else {
											(*genCodeFile) << "  MOV AX, SEG MemoriaDinamica\n";
											(*genCodeFile) << "  PUSH AX\n";
											(*genCodeFile) << "  MOV AX, word ptr " + node->GetValue() + "\n";
											(*genCodeFile) << "  PUSH AX  ;guardó en la pila el segment:offset del string " + node->GetValue() + "\n";
										}
									}
								}
							}
						}
						else {
							//error no se encontró la variable o constante
						}
					}
				}
			}
		}
	}

	void CParser::fnGC0008(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			if (parent->GetData() == "<RestoExpresionSimple>") {
				TreeNode * node = SearchInChildrenByData(parent, "TOKEN_MAS");
				if (node != NULL) {
					if (parent->GetType() == "TOKEN_SHTAWA") {
						(*genCodeFile) << "  POP BX\n";
						(*genCodeFile) << "  POP AX\n";
						(*genCodeFile) << "  ADD AX, BX\n";
						(*genCodeFile) << "  PUSH AX  ; Realiza una suma\n";
					}
				}
				if (node == NULL) {
					node = SearchInChildrenByData(parent, "TOKEN_MENOS");
					if (node != NULL) {
						if (parent->GetType() == "TOKEN_SHTAWA") {
							(*genCodeFile) << "  POP BX\n";
							(*genCodeFile) << "  POP AX\n";
							(*genCodeFile) << "  SUB AX, BX\n";
							(*genCodeFile) << "  PUSH AX  ; Realiza una resta\n";
						}
					}
				}
				if (node == NULL) {
					node = SearchInChildrenByData(parent, "TOKEN_MAS_ROMANO");
					if (node != NULL) {
						if (parent->GetType() == "TOKEN_SHTAWA") {
							(*genCodeFile) << "  POP BX\n";
							(*genCodeFile) << "  POP AX\n";
							(*genCodeFile) << "  ADD AX, BX\n";
							(*genCodeFile) << "  PUSH AX  ; Realiza una suma romana\n";
						}
					}
				}
				if (node == NULL) {
					node = SearchInChildrenByData(parent, "TOKEN_MENOS_ROMANO");
					if (node != NULL) {
						if (parent->GetType() == "TOKEN_SHTAWA") {
							(*genCodeFile) << "  POP BX\n";
							(*genCodeFile) << "  POP AX\n";
							(*genCodeFile) << "  SUB AX, BX\n";
							(*genCodeFile) << "  PUSH AX  ; Realiza una resta romana\n";
						}
					}
				}
			}
			else if (parent->GetData() == "<RestoTermino>") {
				TreeNode * node = SearchInChildrenByData(parent, "TOKEN_MULTIPLICACION");
				if (node != NULL) {
					if (parent->GetType() == "TOKEN_SHTAWA") {
						(*genCodeFile) << "  POP BX\n";
						(*genCodeFile) << "  POP AX\n";
						(*genCodeFile) << "  MUL BX\n";
						(*genCodeFile) << "  PUSH AX  ; Realiza una multiplicacion\n";
					}
				}
				if (node == NULL) {
					node = SearchInChildrenByData(parent, "TOKEN_DIVISION");
					if (node != NULL) {
						if (parent->GetType() == "TOKEN_SHTAWA") {
							(*genCodeFile) << "  POP BX\n";
							(*genCodeFile) << "  POP AX\n";
							(*genCodeFile) << "  DIV BX\n";
							(*genCodeFile) << "  PUSH AX  ; Realiza una division\n";
						}
					}
				}
				if (node == NULL) {
					node = SearchInChildrenByData(parent, "TOKEN_MODULO");
					if (node != NULL) {
						if (parent->GetType() == "TOKEN_SHTAWA") {
							(*genCodeFile) << "  POP BX\n";
							(*genCodeFile) << "  POP AX\n";
							(*genCodeFile) << "  DIV BX\n";
							(*genCodeFile) << "  MOV AL, AH\n";//mover el resto al al
							(*genCodeFile) << "  MOV AH, 0\n";//limpiar la parte alta del AX
							(*genCodeFile) << "  PUSH AX  ; Realiza un modulo\n";
						}
					}
				}
				if (node == NULL) {
					node = SearchInChildrenByData(parent, "TOKEN_MULTIPLICACION_ROMANO");
					if (node != NULL) {
						if (parent->GetType() == "TOKEN_SHTAWA") {
							(*genCodeFile) << "  POP BX\n";
							(*genCodeFile) << "  POP AX\n";
							(*genCodeFile) << "  MUL BX\n";
							(*genCodeFile) << "  PUSH AX  ; Realiza una multiplicacion romana\n";
						}
					}
				}
				if (node == NULL) {
					node = SearchInChildrenByData(parent, "TOKEN_DIVISION_ROMANO");
					if (node != NULL) {
						if (parent->GetType() == "TOKEN_SHTAWA") {
							(*genCodeFile) << "  POP BX\n";
							(*genCodeFile) << "  POP AX\n";
							(*genCodeFile) << "  DIV BX\n";
							(*genCodeFile) << "  PUSH AX  ; Realiza una division romana\n";
						}
					}
				}
				if (node == NULL) {
					node = SearchInChildrenByData(parent, "TOKEN_MODULO_ROMANO");
					if (node != NULL) {
						if (parent->GetType() == "TOKEN_SHTAWA") {
							(*genCodeFile) << "  POP BX\n";
							(*genCodeFile) << "  POP AX\n";
							(*genCodeFile) << "  DIV BX   ; Realiza una division romana\n";
							(*genCodeFile) << "  MOV AL, AH\n";//mover el resto al al
							(*genCodeFile) << "  MOV AH, 0\n";//limpiar la parte alta del AX
							(*genCodeFile) << "  PUSH AX\n";
						}
					}
				}
			}
			else if (parent->GetData() == "<RestoExpresion>") {
				TreeNode * node = SearchInChildrenByData(parent, "TOKEN_IRA");
				if (node != NULL) {
					(*genCodeFile) << "  POP BX\n";
					(*genCodeFile) << "  POP AX\n";
					(*genCodeFile) << "  AND AX, BX\n";
					(*genCodeFile) << "  PUSH AX  ; Realiza un AND\n";
				}
				if (node == NULL) {
					node = SearchInChildrenByData(parent, "TOKEN_IRALE");
					if (node != NULL) {
						(*genCodeFile) << "  POP BX\n";
						(*genCodeFile) << "  POP AX\n";
						(*genCodeFile) << "  OR AX, BX\n";
						(*genCodeFile) << "  PUSH AX  ; Realiza un OR\n";
					}
				}
				if (node == NULL) {
					node = SearchInChildrenByData(parent, "TOKEN_JEBA");
					if (node != NULL) {
						(*genCodeFile) << "  POP BX\n";
						(*genCodeFile) << "  POP AX\n";
						(*genCodeFile) << "  XOR AX, BX\n";
						(*genCodeFile) << "  PUSH AX  ; Realiza un XOR\n";
					}
				}
			}
			else if (parent->GetData() == "<ExpresionOperRel>") {
				TreeNode * node = SearchInChildrenByData(parent, "TOKEN_COMPARADOR");
				if (node != NULL) {
					string strComp = node->GetChildren()[0]->GetValue();
					(*genCodeFile) << "  POP BX\n";
					(*genCodeFile) << "  POP AX\n";
					(*genCodeFile) << "  CMP AX, BX\n";
					if (strComp == "=") {
						(*genCodeFile) << "  JE comp_" + to_string(globalEtiquetas) + "\n";
					}
					else if (strComp == ">") {
						(*genCodeFile) << "  JG comp_" + to_string(globalEtiquetas) + "\n";
					}
					else if (strComp == ">=") {
						(*genCodeFile) << "  JGE comp_" + to_string(globalEtiquetas) + "\n";
					}
					else if (strComp == "<") {
						(*genCodeFile) << "  JL comp_" + to_string(globalEtiquetas) + "\n";
					}
					else if (strComp == "<=") {
						(*genCodeFile) << "  JLE comp_" + to_string(globalEtiquetas) + "\n";
					}
					(*genCodeFile) << "  MOV AX, 0  ; asigna false\n";
					(*genCodeFile) << "  JMP compF_" + to_string(globalEtiquetas) + "\n";
					(*genCodeFile) << "comp_" + to_string(globalEtiquetas) + ":\n";
					(*genCodeFile) << "  MOV AX, 1  ; asigna true\n";
					(*genCodeFile) << "compF_" + to_string(globalEtiquetas) + ":\n";
					(*genCodeFile) << "  PUSH AX  ; Guarda el valor booleano (0 o 1)\n";
					globalEtiquetas += 1;

				}
			}
		}
	}

	void CParser::fnGC0009(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			TreeNode * node = SearchInChildrenByData(parent, "<Asignacion>");
			TreeNode * nodeId = parent->GetParent();//es <sentencia>
			if (node != NULL && nodeId != NULL)
			{
				TreeNode * nodeExp = SearchInChildrenByData(node, "<Expresion>");
				if (nodeExp != NULL) {
					nodeId = SearchInChildrenByData(nodeId, "TOKEN_ID");
					if (nodeId != NULL) {
						if (nodeExp->GetType() == "TOKEN_SHTAWA")
						{
							//el resultado de esa expresión tiene que estar en el top de la pila
							(*genCodeFile) << "  POP AX\n";
							(*genCodeFile) << "  MOV word ptr " + nodeId->GetChildren()[0]->GetValue () + ", AX   ; Asigna el resultado en la variable " + nodeId->GetChildren()[0]->GetValue() +  "\n\n";
							//falta analizar si la variable es global o local
						}
						else if (nodeExp->GetType() == "TOKEN_EKLA") {
							(*genCodeFile) << "  POP AX\n";
							(*genCodeFile) << "  MOV byte ptr " + nodeId->GetChildren()[0]->GetValue() + ", AL   ; Asigna el resultado en la variable " + nodeId->GetChildren()[0]->GetValue() + "\n\n";
						}
						else if (nodeExp->GetType() == "TOKEN_YINAKULIWA") {
							(*genCodeFile) << "  POP AX\n";
							(*genCodeFile) << "  MOV byte ptr " + nodeId->GetChildren()[0]->GetValue() + ", AL   ; Asigna el resultado en la variable " + nodeId->GetChildren()[0]->GetValue() + "\n\n";
						}
						else if (nodeExp->GetType() == "TOKEN_SHEJ") {
							//en la pila está el segmento y el offset del string
							(*genCodeFile) << "  ;toma de la pila el segment y offset del string, y cuenta cuántos caracteres tiene\n";
							(*genCodeFile) << "  POP AX\n";//saca el offset
							(*genCodeFile) << "  MOV word ptr TmpOff, AX\n"; //guardar el offset del string origen
							(*genCodeFile) << "  MOV SI, AX\n";//deja el offset en el SI
							(*genCodeFile) << "  POP AX\n";//saca el segment
							(*genCodeFile) << "  MOV word ptr TmpSeg, AX\n"; //guardar el segment del string origen
							(*genCodeFile) << "  MOV DS, AX\n";//deja el segment en el DS
							(*genCodeFile) << "  CALL LenString\n";//deja el resultado en CX
							(*genCodeFile) << "  MOV word ptr TmpLenStr, CX\n\n"; //guardar el largo del string
							//en CX está la cantidad de bytes que hay que reservar para el nuevo string

							string nombreVar = nodeId->GetChildren()[0]->GetValue();
							(*genCodeFile) << "  ;reserva el espacio para el nuevo string\n";
							(*genCodeFile) << "  MOV AX, word ptr NextFree\n";
							(*genCodeFile) << "  MOV " + nombreVar + ", AX\n";//se guarda el offset donde inicia el nuevo string dentro de MemoriaDinamica
							(*genCodeFile) << "  MOV CX, word ptr TmpLenStr\n";
							(*genCodeFile) << "  INC CX   ;  hay que dejar el espacio para el 0 final\n";
							(*genCodeFile) << "  CALL AllocateString\n";

							(*genCodeFile) << "  ;copia el string del lado derecho de la asignación a la Memoria Dinamica\n";
							(*genCodeFile) << "  MOV AX, word ptr TmpSeg ; Cargar el segmento de la constante en DS\n";
							(*genCodeFile) << "  MOV DS, AX\n";
							(*genCodeFile) << "  MOV SI, word ptr TmpOff ; Cargar el offset del string de origen en SI\n";
							(*genCodeFile) << "  MOV AX, " + nombreVar + "\n";
							(*genCodeFile) << "  MOV DI, AX ;Offset de inicio en MemoriaDinamica\n";
							(*genCodeFile) << "  CALL CopyStringToDynamicMemory; Llamada a la función de copia\n\n";

						}
						//falta otros tipos
					}
				}
			}
		}
	}



	void CParser::fnGC0010(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			topNode->SetValue(to_string(globalEtiquetas));
			(*genCodeFile) << "  ; Inicia IF después de evaluar expresión\n";
			(*genCodeFile) << "  POP AX  ; Obtener el resultado de la expresión booleana del IF \n";
			(*genCodeFile) << "  CMP AL, 1  ;verifica si la expresión del IF es verdadera\n";
			(*genCodeFile) << "  JNE etiq_ELSE_" + to_string(globalEtiquetas) + "\n";
			globalEtiquetas += 1;
		}
	}

	void CParser::fnGC0011(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			TreeNode * node = SearchInChildrenByData(parent, "SR:" + to_string(GC0010));
			if (node != NULL) {
				string etiq = node->GetValue();
				(*genCodeFile) << "  JMP etiq_IF_F_" + etiq + "   ; finaliza parte verdadera del if \n";
				(*genCodeFile) << "etiq_ELSE_" + etiq + ":   ; inicia el else del if\n";
			}
		}
	}

	void CParser::fnGC0012(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			TreeNode * node = SearchInChildrenByData(parent, "SR:" + to_string(GC0010));
			if (node != NULL) {
				string etiq = node->GetValue();
				(*genCodeFile) << "etiq_IF_F_" + etiq + ":    ;finaliza else del if \n\n";
			}
		}
	}

	void CParser::fnGC0013(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			topNode->SetValue(to_string(globalEtiquetas));
			(*genCodeFile) << "etiq_WHILE_" + to_string(globalEtiquetas) + ":\n  ; inicio del while\n";
			globalEtiquetas += 1;
		}
	}

	void CParser::fnGC0014(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			topNode->SetValue(to_string(globalEtiquetas));
			(*genCodeFile) << "  POP AX  ; el resultado de la evaluación de la expresión booleana de un while \n";
			(*genCodeFile) << "  CMP AL, 1\n";
			(*genCodeFile) << "  JNE etiq_WHILE_F_" + to_string(globalEtiquetas) + "\n";
			globalEtiquetas += 1;
		}
	}

	void CParser::fnGC0015(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			TreeNode * nodeInic = SearchInChildrenByData(parent, "SR:" + to_string(GC0013));
			TreeNode * node = SearchInChildrenByData(parent, "SR:" + to_string(GC0014));
			if (nodeInic != NULL && node != NULL) {
				string etiqInic = nodeInic->GetValue();
				string etiq = node->GetValue();
				(*genCodeFile) << "  JMP etiq_WHILE_" + etiqInic + "    ;inicie el ciclo del while nuevamente\n";
				(*genCodeFile) << "etiq_WHILE_F_" + etiq + ":    ;fin del while\n\n";
			}
		}
	}

	void CParser::fnGC0016(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			(*genCodeFile) << "Falta";
		}
	}

	void CParser::fnGC0017(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			(*genCodeFile) << "Falta";
		}
	}

	void CParser::fnGC0018(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			(*genCodeFile) << "Falta";
		}
	}

	void CParser::fnGC0019(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		//manejo de expresión en el llamado de una función.
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			(*genCodeFile) << "  ; Fin de expresion parametro\n";
		}
	}

	void CParser::fnGC0020(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		//manejo de expresión en el llamado de una función.
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		bool treeOK = false;
		bool comoSentencia = false;
		if (parent != NULL) {
			if (parent->GetData() == "<LlamadaProcFunc>") {
				parent = parent->GetParent();
				if (parent != NULL && parent->GetData() == "<SentenciaInicId>") {
					parent = parent->GetParent();
					if (parent != NULL && parent->GetData() == "<Sentencia>") {
						comoSentencia = true;
						treeOK = true;
					}
				}
			}
			else {
				if (parent->GetData() == "<RestoFactorId>") {
					parent = parent->GetParent();
					if (parent != NULL && parent->GetData() == "<FactorIzq>") {
						treeOK = true;
					}
				}
			}
			if (treeOK) {
				TreeNode * nodeTokenId = SearchInChildrenByData(parent, "TOKEN_ID");
				if (nodeTokenId != NULL) {
					(*genCodeFile) << "  CALL " + nodeTokenId->GetChildren()[0]->GetValue() + "  ; llamado a Procedimiento o función\n";
					if (comoSentencia) 
					{
						CSymbolInfo * symbol = currentSymbolTable->getSymbol(nodeTokenId->GetChildren()[0]->GetValue(), true);
						if (symbol != NULL) {
							if (symbol->getSymbolType() == ST_Funcion) {
								string tipo = symbol->getType();
								//si es llamado como si fuera un proc, hay que sacar el resultado de la pila.
								if (tipo == "TOKEN_SHTAWA") {
									//integer 2 bytes
									(*genCodeFile) << "  POP AX  ;  Saca de la pila el resultado de la funcion\n";
								}
								else if (tipo == "TOKEN_EKLA") {
									//char 2 bytes
									(*genCodeFile) << "  POP AX  ;  Saca de la pila el resultado de la funcion\n";
								}
								else if (tipo == "TOKEN_YINAKULIWA") {
									//bool 2 bytes;
									(*genCodeFile) << "  POP AX  ;  Saca de la pila el resultado de la funcion\n";
								}
								else if (tipo == "TOKEN_SHEJ") {
									//string 4 bytes: 2 de segmento y 2 de offset
									(*genCodeFile) << "  POP AX\n";
									(*genCodeFile) << "  POP AX  ;  Saca de la pila el resultado de la funcion\n";
								}
							}
						}
					}
				}
			}
		}
	}

	void CParser::fnGC0021(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		//manejo de expresión en el llamado de una función.
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			TreeNode * previous = SearchPreviousSiblingNode(parent, topNode);
			if (previous != NULL && previous->GetData() == "<Expresion>") {
				if (previous->GetType() == "TOKEN_SHTAWA") {
					//integer 2 bytes
				}
				else if (previous->GetType() == "TOKEN_EKLA") {
					//char 2 bytes
				}
				else if (previous->GetType() == "TOKEN_YINAKULIWA") {
					//bool 2 bytes;
				}
				else if (previous->GetType() == "TOKEN_SHEJ") {
					//string 4 bytes: 2 de segmento y 2 de offset
				}
			}
			//no hay que hacer nada porque la evaluación de la expresión dejó el resultado en la pila.
		}
	}

	void CParser::fnGC0022(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		//manejo de expresión en el llamado de una función.
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		bool treeOK = false;
		if (parent != NULL) {
			if (parent->GetData() == "<RestoFactorId>") {
				parent = parent->GetParent();
				if (parent != NULL && parent->GetData() == "<FactorIzq>") {
					treeOK = true;
				}
			}
		}
		if (treeOK) {
			TreeNode * nodeTokenId = SearchInChildrenByData(parent, "TOKEN_ID");
			if (nodeTokenId != NULL) {
				CSymbolInfo * symbol = currentSymbolTable->getSymbol(nodeTokenId->GetChildren()[0]->GetValue(), true);
				if (symbol->getSymbolType() == ST_Funcion) {
					if (symbol->getType() == "TOKEN_SHTAWA") {
						//integer 2 bytes
						(*genCodeFile) << "  MOV DX, 0\n";
						(*genCodeFile) << "  PUSH DX   ; espacio para retorno de la funcion \n";
					}
					else if (symbol->getType() == "TOKEN_EKLA") {
						//char 2 bytes
						(*genCodeFile) << "  MOV DX, 0\n";
						(*genCodeFile) << "  PUSH DX   ; espacio para retorno de la funcion \n";
					}
					else if (symbol->getType() == "TOKEN_YINAKULIWA") {
						//bool 2 bytes;
						(*genCodeFile) << "  MOV DX, 0\n";
						(*genCodeFile) << "  PUSH DX   ; espacio para retorno de la funcion \n";
					}
					else if (symbol->getType() == "TOKEN_SHEJ") {
						//string 4 bytes: 2 de segmento y 2 de offset
						(*genCodeFile) << "  MOV DX, 0\n";
						(*genCodeFile) << "  PUSH DX \n; ";
						(*genCodeFile) << "  MOV DX, 0\n";
						(*genCodeFile) << "  PUSH DX   ; espacio para retorno de la funcion de tipo string \n";
					}
				}
			}
		}
	}


	void CParser::fnRS1003(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			TreeNode * nodeTipo = parent->GetChildren()[1];
			TreeNode * nodeId = parent->GetChildren()[2];
			TreeNode * nodeValue = parent->GetChildren()[4];
			//validar tipos entre nodeTipo y nodeValue
			if (nodeTipo->GetType() != nodeValue->GetType()) {
				//error semántico
				strErrSem = "Error semantico: No coincide el tipo de dato.  La constante se definió de tipo " + nodeTipo->GetType() + " y el valor es de tipo " + nodeValue->GetType();
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
			else {
				//insertar en la tabla de símbolos la constante.
				int errorCode = 0;
				string constant_name = nodeId->GetChildren()[0]->GetValue();
				string tipo = nodeTipo->GetType();
				CSymbolInfo * symbol = currentSymbolTable->getSymbol(constant_name);
				if (symbol != NULL) {
					strErrSem = "Error semantico: Está definiendo una variable o constante que ya fue definida anteriormente.(" + constant_name + ")";
					strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
					erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
				}
				else {
					currentSymbolTable->addSymbol((int)ST_Constante, constant_name, nodeTipo->GetType(), nodeValue->GetValue(), errorCode);
					if (errorCode != 0) {
						strErrSem = "Error semantico: Error inesperado insertando símbolo en el diccionario de datos";
						strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
						erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
					}
					else {
						cout << "Agregada a la tabla de símbolos la constante: " + constant_name + " de tipo " + tipo + "\n";
					}
				}
			}
		}
	}

	void CParser::fnRS1004(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		TreeNode * parent = topNode->GetParent();
		TreeNode * tokenLit = parent->GetChildren()[0]->GetChildren()[0];
		parent->SetType(getTypeFromValue(tokenLit->GetData()));
		parent->SetValue(tokenLit->GetValue());
	}

	void CParser::fnRS1005(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		TreeNode * parent = topNode->GetParent();
		TreeNode * node;
		if (parent->GetData() == "<DefTipoCompuesto>") {
			node = SearchInChildrenByData(parent,"<TipoSimple>");
		}
		else {
			node = parent->GetChildren()[0];
		}
		if (node != NULL) {
			parent->SetType(node->GetType());
			parent->SetValue(node->GetValue());
		}
	}

	void CParser::fnRS1006(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		TreeNode * parent = topNode->GetParent();
		TreeNode * node;

		if (parent->GetData() == "<SeccionTipos>") {
			node = SearchInChildrenByData(parent, "<TipoDefinido>");// ->GetChildren()[0];
		}
		else 
		{
			node = parent->GetChildren()[0];
			if (node->GetData() == "TOKEN_KICHANA") {
				node = parent->GetChildren()[1];
			}
		}
		definingVarsCurrentType = node->GetType();

	}

	void CParser::fnRS1007(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		TreeNode * node = parent->GetChildren()[0];
		//insertar en la tabla de símbolos la constante.
		int errorCode = 0;
		string var_name = node->GetChildren()[0]->GetValue();
		CSymbolInfo * symbol = currentSymbolTable->getSymbol(var_name);
		if (symbol != NULL) {
			strErrSem = "Error semantico: Está definiendo una variable o constante que ya fue definida anteriormente.(" + var_name + ")";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
		else {
			currentSymbolTable->addSymbol((int)ST_Variable, var_name, definingVarsCurrentType, "", errorCode);
			if (errorCode != 0) {
				strErrSem = "Error semantico: Error inesperado insertando símbolo en el diccionario de datos.";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
			else {
				cout << "Agregada a la tabla de símbolos la variable: " + var_name + " de tipo " + definingVarsCurrentType + "\n";
			}
		}
	}

	void CParser::fnRS1008(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		if (parent != NULL) {
			TreeNode * nodeValue = parent->GetChildren()[1];
			//validar tipos entre nodeTipo y nodeValue
			if (definingVarsCurrentType != nodeValue->GetType()) {
				//error semántico
				strErrSem = "Error semantico: No coincide el tipo de dato.  La variable se definió de tipo " + definingVarsCurrentType + " y el valor es de tipo " + nodeValue->GetType();
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
			else {
				parent = parent->GetParent();
				TreeNode * nodeId = parent->GetChildren()[0]->GetChildren()[0];
				//asignar el valor de inicialización a la variable.
				string nombreVariable = nodeId->GetValue();
				CSymbolInfo * symbol = currentSymbolTable->getSymbol(nombreVariable);
				if (symbol == NULL) {
					cout << "Está inicializando una variable que no está definida.(" << nodeId->GetValue() << ")\n";
				}
				else {
					string valorInicial = nodeValue->GetValue();
					symbol->setValue(valorInicial);
					cout << "Variable " + nombreVariable + " inicializada con el valor: " + valorInicial << "\n";
				}
			}
		}
	}

	void CParser::fnRS1009(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		TreeNode * node = SearchInChildrenByData(parent, "TOKEN_ID");  //parent->GetChildren()[3];
		//insertar en la tabla de símbolos la constante.
		int errorCode = 0;
		string var_name = node->GetChildren()[0]->GetValue();
		CSymbolInfo * symbol = currentSymbolTable->getSymbol(var_name);
		if (symbol != NULL) {
			strErrSem = "Error semantico: Está definiendo una variable o constante que ya fue definida anteriormente.(" + var_name + ")";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);

		}
		else {
			if (parent->GetData() == "<SeccionTipos>") {
				currentSymbolTable->addSymbol((int)ST_UserType, var_name, definingVarsCurrentType, "", errorCode);
				if (errorCode != 0) {
					strErrSem = "Error semantico: Error inesperado insertando símbolo en el diccionario de datos";
					strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
					erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
				}
				else {
					cout << "Agregada a la tabla de símbolos el tipo de dato: " + var_name + "\n";
				}
			}
			else {
				currentSymbolTable->addSymbol((int)ST_Variable, var_name, definingVarsCurrentType, "", errorCode);
				if (errorCode != 0) {
					strErrSem = "Error semantico: Error inesperado insertando símbolo en el diccionario de datos";
					strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
					erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
				}
				else {
					cout << "Agregada a la tabla de símbolos la variable ARRAY: " + var_name + " de tipo " + definingVarsCurrentType + "\n";
				}
			}
		}
	}

	void CParser::fnRS1010(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		//concatenar en el data del padre, el char que acaba de ser leído y que será parte del conjunto
		TreeNode * parent = topNode->GetParent();
		TreeNode * node;
		bool isResto = false;
		if (parent->GetData() == "<ListaConstConjunto>") {
			node = parent->GetChildren()[0]->GetChildren()[0];
		}
		else {
			node = parent->GetChildren()[1]->GetChildren()[0];
			isResto = true;
		}
		if (isResto) {
			parent = parent->GetParent();
		}
		string concatChar = parent->GetValue();
		if (concatChar.length() > 0) {
			concatChar = concatChar + ',';
		}
		concatChar += node->GetValue();
		parent->SetValue(concatChar);
	}

	void CParser::fnRS1011(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		//Copia la lista de chars del conjunto al árbol de arriba (ConstConjunto)
		TreeNode * parent = topNode->GetParent();
		TreeNode * node = parent->GetChildren()[1];
		parent->SetType(node->GetType());
		parent->SetValue(node->GetValue());
		parent->GetChildren()[0]->SetType(node->GetType());
		parent->GetChildren()[0]->SetValue(node->GetValue());
	}

	void CParser::fnRS1012(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		//concatenar en el data del padre, el char que acaba de ser leído y que será parte del conjunto
		TreeNode * parent = topNode->GetParent();
		TreeNode * node;
		node = parent->GetChildren()[1]->GetChildren()[0];
		TreeNode * lastDefDimensiones = NULL;
		while (parent != NULL && 
			   ( parent->GetData() == "<DefDimensionesTipo>"  || 
				 parent->GetData() == "<OtrasDefDimensTipo>")
			  ) 
		{
			if (parent->GetData() == "<DefDimensionesTipo>") {
				lastDefDimensiones = parent;
			}
			parent = parent->GetParent();
		}
		if (lastDefDimensiones != NULL) {
			string concatChar = lastDefDimensiones->GetDims();
			if (concatChar.length() > 0) {
				concatChar = concatChar + ',';
			}
			concatChar += node->GetValue();
			lastDefDimensiones->SetDims(concatChar);
		}
	}

	void CParser::fnRS1013(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		//concatenar en el data del padre, el char que acaba de ser leído y que será parte del conjunto
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		TreeNode * nodeId;
		if (parent->GetData() == "<SeccionTipos>") {
			nodeId = SearchInChildrenByData(parent, "TOKEN_ID");
			if (nodeId != NULL) {
				nodeId = nodeId->GetChildren()[0];
				TreeNode * tipoDefinido = SearchInChildrenByData(parent, "<TipoDefinido>");
				if (tipoDefinido != NULL) {
					TreeNode * defTipoCompuesto = SearchInChildrenByData(tipoDefinido, "<DefTipoCompuesto>");
					if (defTipoCompuesto != NULL) {
						TreeNode * dimTipo = SearchInChildrenByData(defTipoCompuesto, "<DefDimensionesTipo>");
						if (dimTipo != NULL) {
							string dims = dimTipo->GetDims();
							nodeId->SetDims(dims);
							CSymbolInfo * symbol = currentSymbolTable->getSymbol(nodeId->GetValue());
							if (symbol == NULL) {
								strErrSem = "Error semantico: Está definiendo una variable o constante que ya fue definida anteriormente.(" + nodeId->GetValue() + ")";
								strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
								erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
							}
							else {
								symbol->setDims(dims);
								cout << "Definida las dimensiones del tipo.(" << nodeId->GetValue() << "): " + dims + "\n";
							}
						}
					}
				}
			}
		}
		else if (parent->GetData() == "<SeccionVariables>") {
			nodeId = SearchInChildrenByData(parent, "TOKEN_ID");
			if (nodeId != NULL) {
				nodeId = nodeId->GetChildren()[0];
				TreeNode * dimTipo = SearchInChildrenByData(parent, "<DefDimensionesTipo>");
				if (dimTipo != NULL) {
					string dims = dimTipo->GetDims();
					nodeId->SetDims(dims);
					CSymbolInfo * symbol = currentSymbolTable->getSymbol(nodeId->GetValue());
					if (symbol == NULL) {
						strErrSem = "Error semantico: Está definiendo una variable o constante que ya fue definida anteriormente.(" + nodeId->GetValue() + ")";
						strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
						erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
					}
					else {
						symbol->setDims(dims);
						cout << "Definida las dimensiones de la variable.(" << nodeId->GetValue() << "): " + dims + "\n";
					}
				}
			}
		}
	}

	void CParser::fnRS1014(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		//guardar temporalmente el ID del Tipo que se está comenzando a definir
		TreeNode * parent = topNode->GetParent();
		TreeNode * nodeId = SearchInChildrenByData(parent, "TOKEN_ID");
		if (nodeId != NULL) {
			nodeId = nodeId->GetChildren()[0];
			definingCurrentId = nodeId->GetValue();
		}
		else {
			definingCurrentId = "";
		}
	}

	void CParser::fnRS1015(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		string strErrSem;
		//guardar en la tabla de símbolos el ID del Tipo que se está definidiendo
		if (definingCurrentId != "")
		{
			CSymbolInfo * symbol = currentSymbolTable->getSymbol(definingCurrentId);
			if (symbol != NULL) {
				strErrSem = "Error semantico: Está definiendo una variable o constante que ya fue definida anteriormente.(" + definingCurrentId + ")";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
			else {
				int errorCode;
				currentSymbolTable->addSymbol(ST_UserType, definingCurrentId, "BOK", "", errorCode);
			}
		}
	}

	CSymbolInfo * getTemporalVar(vector<CSymbolInfo *> temporalRegVar, string regVar) {
		CSymbolInfo * result = NULL;
		for (CSymbolInfo * it : temporalRegVar) {
			if (it->getValue() == regVar) {
				result = it;
				break;
			}
		}
		return result;
	}

	void CParser::fnRS1016(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		//guardar en la tabla de símbolos el ID del campo del registro que se está definidiendo
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		TreeNode * nodeTipo = SearchInChildrenByData(parent, "<TipoSimple>");//tipo del campo
		TreeNode * nodeId = SearchInChildrenByData(parent, "TOKEN_ID");//id del campo
		if (nodeId != NULL && nodeTipo != NULL) {
			nodeId = nodeId->GetChildren()[0];
			string regVar;
			if (currentSection == 2) {
				regVar = definingCurrentId + "." + nodeId->GetValue();
				//aqui se debe saber qué estamos definidiendo, si tipo o variable
				CSymbolInfo * symbol = currentSymbolTable->getSymbol(regVar);
				if (symbol != NULL) {
					strErrSem = "Error semantico: Está definiendo una variable o constante que ya fue definida anteriormente.(" + regVar + ")";
					strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
					erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
				}
				else {
					int errorCode;
					currentSymbolTable->addSymbol(ST_Variable, regVar, nodeTipo->GetType(), "", errorCode);
				}
			}
			else if (currentSection == 3) {
				regVar = nodeId->GetValue();
				CSymbolInfo * symbol = getTemporalVar(temporalRegVar, regVar);
				if (symbol != NULL) {
					strErrSem = "Error semantico: Está definiendo una variable o constante que ya fue definida anteriormente.(" + regVar + ")";
					strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
					erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
				}
				else {
					symbol = new CSymbolInfo();
					symbol->setValue(regVar);
					symbol->setType(nodeTipo->GetType());
					temporalRegVar.insert(temporalRegVar.end(), symbol);
				}
			}
		}
	}


	void CParser::fnRS1017(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) 
	{
		//guardar en la tabla de símbolos el ID del campo del registro que se está definidiendo
		TreeNode * parent = topNode->GetParent();
		TreeNode * node;
		currentSection = 0;
		node = SearchInChildrenByData(parent, "TOKEN_KARA");
		if (node != NULL) {
			currentSection = 1;
		}
		if (currentSection == 0) {
			node = SearchInChildrenByData(parent, "TOKEN_KLO");
			if (node != NULL) {
				currentSection = 2;
			}
		}
		if (currentSection == 0) {
			node = SearchInChildrenByData(parent, "TOKEN_KASKA");
			if (node != NULL) {
				currentSection = 3;
			}
		}
		if (currentSection == 0) {
			node = SearchInChildrenByData(parent, "TOKEN_WAKIRI");
			if (node != NULL) {
				currentSection = 4;
			}
		}
		if (currentSection == 0) {
			node = SearchInChildrenByData(parent, "TOKEN_PRA");
			if (node != NULL) {
				currentSection = 5;
			}
		}
	}

	void CParser::fnRS1018(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		temporalRegVar.clear();
	}

	void CParser::fnRS1019(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		TreeNode * parent = topNode->GetParent();
		TreeNode * nodeId = SearchInChildrenByData(parent, "TOKEN_ID");
		string regVar;
		if (nodeId != NULL) {
			nodeId = nodeId->GetChildren()[0];
			regVar = nodeId->GetValue();
			int errorCode;
			for (CSymbolInfo * it : temporalRegVar) {
				currentSymbolTable->addSymbol(ST_Variable, it->getValue()/*regVar + "." + it->getValue()*/, it->getType(), "", errorCode);
			}
		}
	}

	void CParser::fnRS1020(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//insertar el nombre de una funcion o subrutina a la tabla de símbolos
		string strErrSem;
		TreeNode * parent = topNode->GetParent();
		TreeNode * nodeId = SearchInChildrenByData(parent, "TOKEN_ID");
		TreeNode * node = SearchInChildrenByData(parent, "TOKEN_KIANA");
		int errorCode;
		int cual = 0;
		if (node != NULL) {
			cual = 1;
		}
		else {
			node = SearchInChildrenByData(parent, "TOKEN_WAYUA");
			if (node != NULL) {
				cual = 2;
			}
		}
		if (cual != 0 && nodeId != NULL) {
			nodeId = nodeId->GetChildren()[0];
			definingCurrentId = nodeId->GetValue();
			if (cual == 1) {
				if (currentSymbolTable->getSymbol(definingCurrentId) == NULL) {
					currentSymbolTable->addSymbol(ST_Funcion, definingCurrentId, "", "", errorCode);
				}
				else {
					strErrSem = "Error semantico: Está definiendo una función que ya fue definida anteriormente.(" + definingCurrentId + ")";
					strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
					erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
				}
			}
			else if (cual == 2) {
				if (currentSymbolTable->getSymbol(definingCurrentId) == NULL) {
					currentSymbolTable->addSymbol(ST_Rutina, definingCurrentId, "", "", errorCode);
				}
				else {
					strErrSem = "Error semantico: Está definiendo una subrutina que ya fue definida anteriormente.(" + definingCurrentId + ")";
					strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
					erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
				}
			}
		}
	}

	void CParser::fnRS1021(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//insertar un parámetro de una función o subrutina
		TreeNode * parent = topNode->GetParent();

		bool esPrototipo = false;
		TreeNode * parentProto = parent->GetParent();
		if (parentProto != NULL) {
			parentProto = parentProto->GetParent();
			if (parentProto != NULL && parentProto->GetData() == "<SeccionPrototipos>") {
				esPrototipo = true;
			}
		}

		CSymbolInfo * symbolFunc;
		CSymbolInfo * symbolParam;
		TreeNode * nodeId = SearchInChildrenByData(parent, "TOKEN_ID");
		TreeNode * nodeRef = SearchInChildrenByData(parent, "TOKEN_IAWA");
		int errorCode;
		if (nodeId != NULL) {
			nodeId = nodeId->GetChildren()[0];
			string id = nodeId->GetValue(); //definingCurrentId + "." + nodeId->GetValue();
			if (currentSymbolTable->getSymbol(id) == NULL) {
				currentSymbolTable->addSymbol(ST_Variable, id, definingVarsCurrentType, "", errorCode);
				symbolFunc = currentSymbolTable->getSymbol(definingCurrentId, true);//el true se tuvo que porque el cambio de tabla de símbolo se hace después de la definición de la función.  Los parámetros son parte de la tabla de símbolos local, pero la función es parte de la tabla de simbolos global
				if (symbolFunc != NULL) {
					symbolParam = new CSymbolInfo();
					symbolParam->setName(nodeId->GetValue());
					symbolParam->setType(definingVarsCurrentType);
					if (nodeRef != NULL) {
						symbolParam->setIsReference(true);
					}
					else {
						symbolParam->setIsReference(false);
					}
					if (esPrototipo) {
						symbolFunc->addFormalParam(symbolParam);
					}
					else {
						symbolFunc->addFormalParam(symbolParam, true);
					}
				}
			}
			else {
				string strErrSem = "Error semantico: Está definiendo un parámetro que ya fue definido anteriormente.(" + id + ")";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
		}
	}

	void CParser::fnRS1022(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//insertar un parámetro de una función o subrutina
		TreeNode * parent = topNode->GetParent();
		TreeNode * node = SearchInChildrenByData(parent, "<TipoSimple>");
		if (node != NULL) {
			CSymbolInfo * symbol = currentSymbolTable->getSymbol(definingCurrentId);
			if (symbol != NULL) {
				symbol->setType(node->GetType());
			}
		}
	}

	void CParser::fnRS1023(int top, TreeNode * topNode, Token currentToken)
	{
		//cambiar a tabla de simbolo local
		symbolTableLocal = new CSymbolTable();
		symbolTableLocal->parent = symbolTableGlobal;
		currentSymbolTable = symbolTableLocal;
	}

	void CParser::fnRS1024(int top, TreeNode * topNode, Token currentToken)
	{
		//cambiar de tabla de simbolo a global
		symbolTableLocal = NULL;
		currentSymbolTable = symbolTableGlobal;
	}

	void CParser::fnRS1025(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//Se da cuenta que está en una asignación: hay que validar que:
		//exista el ID en la tabla de símbolos como variable
		//guardar que se está trabajando con ese ID
		TreeNode * parent = topNode->GetParent();//obtiene asignación
		parent = parent->GetParent();//obtiene SentenciaInicialId
		parent = parent->GetParent();//obtiene Sentencia
		TreeNode * nodeId = SearchInChildrenByData(parent, "TOKEN_ID");
		if (nodeId != NULL) {
			nodeId = nodeId->GetChildren()[0];
			string id = nodeId->GetValue();
			CSymbolInfo * symbol = currentSymbolTable->getSymbol(id, true);
			if (symbol != NULL) {
				if (symbol->getSymbolType() == ST_Variable || symbol->getSymbolType() == ST_Indefinido) {
					pilaLlamados.push(symbol->clone());
				}
				else {
					string strErrSem = "Error semantico:Del lado izquierdo de una asignación, solamente puede ir una variable.";
					strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
					erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
				}
			}
			else {
				string strErrSem = "Error semantico: Id no definido.(" + id + ")";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
		}
	}

	void CParser::fnRS1026(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//Se da cuenta que está en un llamado a funcion o rutina, hay que validar que:
		//guardar que se está trabajando con ese ID
		//validar que si es func-subrutina, que el nombre con lo que se está trabajando es una func o subrutina 
		TreeNode * parent = topNode->GetParent();//Obtiene RestoFactorId o LlamadaProcFunc
		parent = parent->GetParent();//Obtiene FactorIzq(RestoFactorId) o SentenciaInicId(LlamadaProcFunc)
		if (parent->GetData() == "<SentenciaInicId>") {
			parent = parent->GetParent();//Obtiene Sentencia
		}
		TreeNode * nodeId = SearchInChildrenByData(parent, "TOKEN_ID");
		if (nodeId != NULL) {
			nodeId = nodeId->GetChildren()[0];
			if (nodeId != NULL) {
				string id = nodeId->GetValue();
				CSymbolInfo * symbol = currentSymbolTable->getSymbol(id, true);
				if (symbol != NULL) {
					//validar que si es func-subrutina
					if (symbol->getSymbolType() != ST_Funcion && symbol->getSymbolType() != ST_Rutina && symbol->getSymbolType() != ST_Indefinido) {
						//validar que si es asignación, tiene que ser variable
						string strErrSem = "Error semantico: Se espera que llame a una función o rutina.(" + symbol->getName() + ")";
						strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
						erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
					}
					else {
						pilaLlamados.push(symbol->clone());
					}
				}
			}
		}
	}

	void CParser::fnRS1027(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//validar que la variable y la expresión tienen tipos compatibles.
		TreeNode * parent = topNode->GetParent();
		TreeNode * nodeExp = SearchInChildrenByData(parent, "<Expresion>");
		if (nodeExp != NULL) {
			if (!pilaLlamados.empty()) {
				CSymbolInfo * var = pilaLlamados.top();
				if (var != NULL) {
					if (var->getType() != nodeExp->GetType()) {
						string strErrSem = "Error semantico: No son compatibles el tipo de la variable al lado izquierdo (" + var->getType() + ") y la expresión del lado derecho (" + nodeExp->GetType() + ")";
						strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
						erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
					}
				}
				else
				{
					string strErrSem = "Error semantico: estructura inesperada. No encontró la variable en la pila";
					strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
					erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
				}
			}
		}
		else
		{
			string strErrSem = "Error semantico: estructura inesperada. Falta Expresion en la asignación";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
	}

	void CParser::fnRS1028(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//sacar de la llamada de la pila de llamadas, el top.
		if (!pilaLlamados.empty()) {
			CSymbolInfo * symbol = pilaLlamados.top();
			pilaLlamados.pop();
		}
		else {
			string strErrSem = "Error semantico:Inesperado. Se esperaba la finalización de una expresión.";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
	}

	void CParser::fnRS1029(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//validar que los parámetros formales y funcionales sean la misma cantidad y los mismos tipos.
		if (!pilaLlamados.empty()) {
			CSymbolInfo * func = pilaLlamados.top();
			if (func != NULL) {
				vector<CSymbolInfo *> * lstFormal = func->getFormalParams();
				vector<CSymbolInfo *> * lstFunc = func->getFunctionalParams();
				if (lstFormal->size() != lstFunc->size()) {
					string strErrSem = "Error semantico: La cantidad de parametros formales y funcionales no coincide en '" + func->getName() + "'. Se esperan " + to_string(lstFormal->size()) + " parámetros y se usaron " + to_string(lstFunc->size());
					strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
					erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
				}
			}
		}
		else {
			string strErrSem = "Error semantico:Se esperaba una función o subrutina para analizar pero no llegó ninguna.";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
	}

	void CParser::fnRS1030(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//agregar el tipo de la expresion en la lista de parametros funcionales para poderla comparar contra los tipos de parametros formales
		TreeNode * parent = topNode->GetParent();
		if (!pilaLlamados.empty()) {
			CSymbolInfo * func = pilaLlamados.top();

			TreeNode * nodeExp = SearchInChildrenByData(parent, "<Expresion>");
			if (nodeExp != NULL)
			{
				int num = func->getFunctionalParams()->size();
				CSymbolInfo * param = new CSymbolInfo(to_string(num+1), nodeExp->GetType(), "", ST_Variable);
				func->addFunctionalParam(param);
			}
		}
	}

	void CParser::fnRS1031(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//valida que el TOKEN_ID exista definido como variable/constante o como función/rutina.
		TreeNode * parent = topNode->GetParent();
		TreeNode * nodeId = SearchInChildrenByData(parent, "TOKEN_ID");
		if (nodeId != NULL) {
			nodeId = nodeId->GetChildren()[0];
			string id = nodeId->GetValue();
			int cual = 0;
			CSymbolInfo * symbol = currentSymbolTable->getSymbol(id, true);
			if (symbol == NULL) {
				int errorCode;

				string strErrSem = "Error semantico: Identificador " + id + " no definido.";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);

				currentSymbolTable->addSymbol((int)ST_Indefinido, id, "", "", errorCode);//se inserta la variable para que pueda continuar trabajando 
			}
		}
	}

	void CParser::fnRS1032(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//subir el type del treenode hermano de topNode al padre
		TreeNode * parent = topNode->GetParent();
		TreeNode * previous = SearchPreviousSiblingNode(parent, topNode);
		if (previous != NULL) {
			if (previous->GetData() == "TOKEN_ID") {
				TreeNode * nodeId = previous->GetChildren()[0];
				CSymbolInfo * symbol = currentSymbolTable->getSymbol(nodeId->GetValue(), true);
				if (symbol != NULL) {
					parent->SetType(symbol->getType());
				}
			}
			else 
			{
				parent->SetType(previous->GetType());
			}
		}
	}

	void CParser::fnRS1033(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//Validar la operación de concatenar y asignar el type = STRING.
		TreeNode * parent = topNode->GetParent();  //El padre es <OperacionesInfijas>
		TreeNode * oper2 = SearchPreviousSiblingNode(parent, topNode);//El previous es <Factor>  : Factor ya debería tener tipo y debería ser STRING.
		if (oper2 != NULL) {
			string factorType = oper2->GetType();
			if (oper2->GetType() == "TOKEN_SHEJ" || oper2->GetType() == "TOKEN_JARJA") //para el operador CONCATENAR, ambos operadores deben ser string
			{
				parent->SetType(oper2->GetType());
			}
			else {
				string strErrSem = "Error semantico: Los operadores para concatenación deben ser STRING";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
		}
		else {
			string strErrSem = "Error semantico: estructura inesperada. Falta segundo operador";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
	}

	void CParser::fnRS1034(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//Validar los parámetros de TIA .. TIA deben ser INT.
		//Asignar al OperacionesInfijas el tipo String
		TreeNode * parent = topNode->GetParent();  //El padre es <OperacionesInfijas>
		TreeNode * oper1 = SearchInChildrenByData(parent, "<ExpresionTia>" );//El oper1 es <ExpresionTia>  : ExpresionTia ya debería tener tipo
		TreeNode * oper2 = SearchPreviousSiblingNode(parent, topNode);//El oper2 es <ExpresionTia>  : ExpresionTia ya debería tener tipo
		if (oper1 != NULL && oper2 != NULL) {
			string factorType = "TOKEN_SHEJ";
			if (oper1->GetType() == "TOKEN_SHTAWA" && oper2->GetType() == "TOKEN_SHTAWA") //para el operador TIA ... TIA, ambos operadores deben ser int
			{
				//todo OK
				parent->SetType(factorType); //<OperacionesInfijas>
			}
			else {
				string strErrSem = "Error semantico: Los operadores para cortar deben ser INT";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
		}
		else {
			string strErrSem = "Error semantico: estructura inesperada. Faltan operadores adecuados en el cortar o recortar strings";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
	}

	void CParser::fnRS1035(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//Termino -> Factor RestoTermino (donde RestoTermino son operaciones con operadores fuertes *,/,%
		//Validar que RestoTermino sea compatible con Factor
		//Asignar el tipo de Factor RestoTermino al padre "<Termino>"

		TreeNode * parent = topNode->GetParent();  //El padre es <Termino> o <RestoTermino>
		TreeNode * oper1 = SearchInChildrenByData(parent, "<Factor>");//El oper1 es <Factor>
		TreeNode * oper2 = SearchInChildrenByData(parent, "<RestoTermino>");//El oper2 es <RestoTermino> el cual puede tener epsilon
		if (oper1 != NULL && oper2 != NULL) {
			if ((oper1->GetType() == oper2->GetType()) || 
				(oper2->GetData() == "<RestoTermino>" && oper2->GetChildren().size() == 0) ) //Si los tipos de los dos operadores no son compatibles, debería ser porque el segundo operador es un epsilon
			{
				//todo OK
				parent->SetType(oper1->GetType()); //<OperacionesInfijas>
			}
			else {
				string strErrSem = "Error semantico: Los operadores aritméticos no son compatibles.";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
		}
		else {
			string strErrSem = "Error semantico: estructura inesperada. Faltan operadores adecuados  en operacion aritmética";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
	}

	void CParser::fnRS1036(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//ExpresionSimple -> Termino RestoExpresionSimple (donde RestoExpresionSimple son operaciones con operadores débiles +,-
		//Validar que RestoExpresionSimple sea compatible con Termino
		//Asignar el tipo de Termino RestoExpresionSimple al padre "<ExpresionSimple>"

		TreeNode * parent = topNode->GetParent();  //El padre es <Termino> o <RestoTermino>
		TreeNode * oper1 = SearchInChildrenByData(parent, "<Termino>");//El oper1 es <Factor>
		TreeNode * oper2 = SearchInChildrenByData(parent, "<RestoExpresionSimple>");//El oper2 es <RestoTermino> el cual puede tener epsilon
		if (oper1 != NULL && oper2 != NULL) {
			if ((oper1->GetType() == oper2->GetType()) ||
				(oper2->GetData() == "<RestoExpresionSimple>" && oper2->GetChildren().size() == 0)) //Si los tipos de los dos operadores no son compatibles, debería ser porque el segundo operador es un epsilon
			{
				//todo OK
				parent->SetType(oper1->GetType()); //<OperacionesInfijas>
			}
			else {
				string strErrSem = "Error semantico: Los operadores dos operadores aritméticos no son compatibles.";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
		}
		else {
			string strErrSem = "Error semantico: estructura inesperada. Faltan operadores adecuados en operacion aritmética";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
	}


	void CParser::fnRS1037(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//Factor -> FactorIzq OperacionesInfijas (donde OperacionesInfijas son operaciones con operadores de string o de conjuntos
		//Validar que OperacionesInfijas sea compatible con FactorIzq
		//Asignar el tipo de FactorIzq OperacionesInfijas al padre "<Factor>"

		TreeNode * parent = topNode->GetParent();  //El padre es <Factor>
		TreeNode * oper1 = SearchInChildrenByData(parent, "<FactorIzq>");//El oper1 es <Factor>
		TreeNode * oper2 = SearchInChildrenByData(parent, "<OperacionesInfijas>");//El oper2 es <OperacionesInfijas> el cual puede tener epsilon
		if (oper1 != NULL && oper2 != NULL) {
			if ((oper1->GetType() == oper2->GetType()) ||
				(oper2->GetData() == "<OperacionesInfijas>" && oper2->GetChildren().size() == 0) ) //Si los tipos de los dos operadores no son compatibles, debería ser porque el segundo operador es un epsilon
			{
				//todo OK
				parent->SetType(oper1->GetType()); //<FactorIzq>
			}
			else {
				string strErrSem = "Error semantico: Los operadores dos operadores infijos no son compatibles.";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
		}
		else {
			string strErrSem = "Error semantico: estructura inesperada. Faltan operadores adecuados en operacion infija";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
	}

	void CParser::fnRS1038(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//ExpresionOperRel -> TOKEN_COMPARADOR ExpresionSimple 
		//Asignar el tipo de ExpresionSimple al padre "<ExpresionOperRel>"

		TreeNode * parent = topNode->GetParent();  
		TreeNode * oper1 = SearchInChildrenByData(parent, "<ExpresionSimple>");
		if (oper1 != NULL ) {
			parent->SetType(oper1->GetType()); 
		}
		else {
			string strErrSem = "Error semantico: estructura inesperada. Faltan operadores adecuados  en operacion de comparacion";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
	}


	void CParser::fnRS1039(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//ExpresionRelacional -> ExpresionSimple ExpresionOperRel 
		//Validar que ExpresionOralRel sea compatible con ExpresionSimple
		//Asignar el tipo de Factor RestoTermino al padre "<ExpresionRelacional>"

		TreeNode * parent = topNode->GetParent();  //El padre es <Termino> o <RestoTermino>
		TreeNode * oper1 = SearchInChildrenByData(parent, "<ExpresionSimple>");//El oper1 es <Factor>
		TreeNode * oper2 = SearchInChildrenByData(parent, "<ExpresionOperRel>");//El oper2 es <RestoTermino> el cual puede tener epsilon
		if (oper1 != NULL && oper2 != NULL) {
			if (oper1->GetType() == oper2->GetType() && oper1->GetType() != "") 
			{
				//todo OK
				parent->SetType("TOKEN_YINAKULIWA"); //la comparacion de dos expresiones es un resultado booleano
			}
			else if (oper2->GetData() == "<ExpresionOperRel>" && oper2->GetChildren().size() == 0) //Si los tipos de los dos operadores no son compatibles, debería ser porque el segundo operador es un epsilon
			{
				//todo OK
				parent->SetType(oper1->GetType()); //solamente era la ExpresionSimple, por tanto se traspasa el tipo de ExpresionSimple
			}
			else {
				string strErrSem = "Error semantico: Los operadores aritméticos no son compatibles.";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
		}
		else {
			string strErrSem = "Error semantico: estructura inesperada. Faltan operadores adecuados  en operacion aritmética";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
	}

	void CParser::fnRS1040(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable)
	{
		//Expresion -> ExpresionRelacional RestoExpresion
		//Validar que RestoExpresion sea compatible con ExpresionRelacional
		//Asignar el tipo de ExpresionRelacional RestoExpresion al padre "<Expresion>"

		TreeNode * parent = topNode->GetParent();  //El padre es <Termino> o <RestoTermino>
		TreeNode * oper1 = SearchInChildrenByData(parent, "<ExpresionRelacional>");
		TreeNode * oper2 = SearchInChildrenByData(parent, "<RestoExpresion>");
		if (oper1 != NULL && oper2 != NULL) {
			if ((oper1->GetType() == oper2->GetType()) ||
				(oper2->GetData() == "<RestoExpresion>" && oper2->GetChildren().size() == 0)) //Si los tipos de los dos operadores no son compatibles, debería ser porque el segundo operador es un epsilon
			{
				//todo OK
				parent->SetType(oper1->GetType()); //<OperacionesInfijas>
			}
			else {
				string strErrSem = "Error semantico: Los operadores dos operadores aritméticos no son compatibles";
				strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
				erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
			}
		}
		else {
			string strErrSem = "Error semantico: estructura inesperada. Faltan operadores adecuados en operacion aritmética";
			strErrSem += ". Line:" + std::to_string(currentToken.getCurrentLine()) + " Col:" + std::to_string(currentToken.getCurrentPosInLine()) + "\n";
			erroresSemanticos.insert(erroresSemanticos.end(), strErrSem);
		}
	}


	void CParser::ejecutarAccionSemantica(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable) {
		if (top == GC0001) {
			fnGC0001(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0002) {
			fnGC0002(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0003) {
			fnGC0003(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0004) {
			fnGC0004(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0005) {
			fnGC0005(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0006) {
			fnGC0006(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0007) {
			fnGC0007(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0008) {
			fnGC0008(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0009) {
			fnGC0009(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0010) {
			fnGC0010(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0011) {
			fnGC0011(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0012) {
			fnGC0012(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0013) {
			fnGC0013(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0014) {
			fnGC0014(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0015) {
			fnGC0015(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0016) {
			fnGC0016(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0017) {
			fnGC0017(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0018) {
			fnGC0018(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0019) {
			fnGC0019(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0020) {
			fnGC0020(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0021) {
			fnGC0021(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == GC0022) {
			fnGC0022(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1003) {
			//Revisar la compatibilidad del tipo en la definición de una constante
			//Crear un registro en la tabla de símbolos para la constante
			fnRS1003(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1004) {
			//Subir la información del tipo en el token al nodo padre TipoSistema
			fnRS1004(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1005) {
			//Subir la información de TipoSistema a TipoSimple
			fnRS1005(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1006) {
			//guardar, en alguna variable del parser, el TipoSimple que se va a usar para definir las variables
			this->fnRS1006(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1007) {
			//insertar variable en tabla de símbolos con el tipo de datos que está guardado en la variable del parser para el tipo
			this->fnRS1007(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1008) {
			//ojo, creo que es similar al RS1003
			//Validar que el valor constante con el que se está inicializando la variable, sea compatible con el tipo de dato de la variable.
			//guardar en la tabla de símbolos, el valor inicial de la variable.
			this->fnRS1008(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1009) {
			//Insertar variable ARRAY en la tabla de simbolos con el tipo de datos que está guardado en la variable del parser para el tipo
			this->fnRS1009(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1010) {
			//va a agrupar el conjunto de caracteres en el padre
			this->fnRS1010(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1011) {
			//va a agrupar el conjunto de caracteres en el padre
			this->fnRS1011(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1012) {
			//va a agrupar el conjunto de dimensiones en el padre
			this->fnRS1012(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1013) {
			//va a pasar la información de dimensiones al ID de la variable o al ID del tipo.
			this->fnRS1013(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1014) {
			//va a pasar la información de dimensiones al ID de la variable o al ID del tipo.
			this->fnRS1014(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1015) {
			//va a pasar la información de dimensiones al ID de la variable o al ID del tipo.
			this->fnRS1015(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1016) {
			//va a pasar la información de dimensiones al ID de la variable o al ID del tipo.
			this->fnRS1016(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1017) {
			//va a pasar la información de dimensiones al ID de la variable o al ID del tipo.
			this->fnRS1017(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1018) {
			//va a pasar la información de dimensiones al ID de la variable o al ID del tipo.
			this->fnRS1018(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1019) {
			//va a pasar la información de dimensiones al ID de la variable o al ID del tipo.
			this->fnRS1019(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1020) {
			//va a pasar la información de dimensiones al ID de la variable o al ID del tipo.
			this->fnRS1020(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1021) {
			//va a pasar la información de dimensiones al ID de la variable o al ID del tipo.
			this->fnRS1021(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1022) {
			//va a pasar la información de dimensiones al ID de la variable o al ID del tipo.
			this->fnRS1022(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1023) {
			//la tabla de simbolos actual pasa a ser la local
			this->fnRS1023(top, topNode, currentToken);
		}
		else if (top == RS1024) {
			//la tabla de simbolos actual pasa a ser la global
			this->fnRS1024(top, topNode, currentToken);
		}
		else if (top == RS1025) {
			//buscar el ID en la tabla de símbolos -> variable o funcion-subrutina
			//guardar que se está trabajando con ese ID
			this->fnRS1025(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1026) {
			//validar que si es asignación, que el nombre con lo que se está trabajando es una variable 
			//validar que si es func-subrutina, que el nombre con lo que se está trabajando es una func o subrutina 
			this->fnRS1026(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1027) {
			//validar que la variable y la expresión tienen tipos compatibles.
			this->fnRS1027(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1028) {
			//sacar de la llamada de la pila de llamadas, el top.
			this->fnRS1028(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1029) {
			//validar que los parámetros formales y funcionales sean la misma cantidad y los mismos tipos.
			this->fnRS1029(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1030) {
			//agregar el tipo de la expresion en la lista de parametros funcionales para poderla comparar contra los tipos de parametros formales
			this->fnRS1030(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1031) {
		//Validar que el Id variable o función ya está definido antes de usarlo
			this->fnRS1031(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1032) {
			//subir el type de la expresión al padre
			this->fnRS1032(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1033) {
			//manejo de la contactenación de strings
			this->fnRS1033(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1034) {
			//manejo del operador cut de strings
			this->fnRS1034(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1035) {
			//manejo de revisión de tipos en: <Termino> que son Factor OPERADOR Resto, y asignación del tipo de los hijos al padre <Termino>
			this->fnRS1035(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1036) {
			//manejo de revisión de tipos en: <ExpresionSimple> que son Termino OPERADOR Resto, y asignación del tipo de los hijos al padre <ExpresionSimple>
			this->fnRS1036(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1037) {
			//manejo de revisión de tipos en: <Factor> que son FactorIzq OPERADOR-Strings-Conjunto Resto, y asignación del tipo de los hijos al padre <Factor>
			this->fnRS1037(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1038) {
		//manejo de revisión de tipos en: <Termino> que son Factor OPERADOR Resto, y asignación del tipo de los hijos al padre <Termino>
		this->fnRS1038(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1039) {
		//manejo de revisión de tipos en: <ExpresionSimple> que son Termino OPERADOR Resto, y asignación del tipo de los hijos al padre <ExpresionSimple>
		this->fnRS1039(top, topNode, currentToken, currentSymbolTable);
		}
		else if (top == RS1040) {
		//manejo de revisión de tipos en: <Factor> que son FactorIzq OPERADOR-Strings-Conjunto Resto, y asignación del tipo de los hijos al padre <Factor>
		this->fnRS1040(top, topNode, currentToken, currentSymbolTable);
		}
	}

	CParser::CParser() {
		symbolTableGlobal = new CSymbolTable();
	}

	void CParser::initDataSegment() {
		memoryTable->addDataConstVar("datos segment");
		memoryTable->addDataConstVar("  MemoriaDinamica db " + to_string(tam_memoria) + " DUP(?)");
		memoryTable->addDataConstVar("  NextFree DW 0");
		memoryTable->addDataConstVar("  TmpLenStr DW 0");
		memoryTable->addDataConstVar("  TmpSeg DW 0");
		memoryTable->addDataConstVar("  TmpOff DW 0");
	}

	// TODO: This is an example of a library function
	int CParser::Parse(string fileName, ofstream * genCodeFile)
	{
		int result = 0;
		int contError = 0;
		stack<TreeNode * > pila;
		int top;
		int produccion;
		Token current_token;
		bool pedirToken;
		CScanner scanner;
		TreeNode * topNode;

		this->genCodeFile = genCodeFile;

		this->currentSymbolTable = this->symbolTableGlobal;
		symbolTableLocal = NULL;

		int resInit = scanner.inicializarScanner(fileName);

		if (resInit == 1) {
			return 1;
		}

		initDataSegment();

		TreeNode * syntaxTree = new TreeNode();
		syntaxTree->SetData(SIMBOLOINICIAL, getStrProduccion(SIMBOLOINICIAL));
		pila.push(syntaxTree);
		pedirToken = true;

		while (!pila.empty()) {
			topNode = pila.top();
			top = topNode->GetIdRegla();
			pila.pop();
			if (pedirToken) {
				current_token = scanner.demeToken();
				string strLine = std::to_string(current_token.getCurrentLine());
				string strPos = std::to_string(current_token.getCurrentPosInLine());
				cout << "leyendo token: " << current_token.toString() << " " << current_token.getLexema() << "  , Line:" << strLine << " Col:" << strPos  <<"\n";
			}
			pedirToken = false;

			if (isTerminal(top))  // Terminal check
			{
				if (top == (int)current_token.getTipoToken()) {
					if (top == 0) {
						top = 0;
					}
					cout << "MATCH TOKEN : " << current_token.toString() << "\n";
					pedirToken = true; //Advance input
					TreeNode * tokenNode = new TreeNode(topNode);
					tokenNode->SetData(top, getStrProduccion(top));
					tokenNode->SetValue(current_token.getLexema());
					topNode->AddChildren(tokenNode);
				}
				else {
					cout << "Error: Se esperaba " << getStrProduccion(top) << ", pero se encontró " << current_token.toString() << ". Saltando token" << "\n";
					contError += 1;
					pedirToken = true;//se indica que debe pedir otro token
				}
			}
			else if (isSemanticRule(top)) {
				//ejecute la regla semantica.
				cout << "ejecute regla semantica: " << std::to_string(top) << "\n";
				this->ejecutarAccionSemantica(top, topNode, current_token, this->currentSymbolTable);
			}
			else // Non - terminal 
			{
				int tipoToken = (int)current_token.getTipoToken();
				produccion = TablaParsing[top-SIMBOLOINICIAL][tipoToken];
				string strProd;
				strProd = getStrProduccion(top);
				if (produccion != -1) {
					//Push production to stack
					cout << "REPLACE STACK : " << strProd << " por: \n";
					TreeNode * ruleNode;
					int iSemantic = 0;
					for (int i = 0; i < MAXLADODER; i++) {
						iSemantic = i;
						if (LadosDerechos[produccion][i] == -1) {
							break;
						}
						else {
							/*
							if (ReglasSemanticas[produccion][iSemantic] != -1) {
								ruleNode = new TreeNode(topNode);
								ruleNode->SetData(ReglasSemanticas[produccion][iSemantic], getStrProduccion(ReglasSemanticas[produccion][iSemantic]));
								topNode->AddChildren(ruleNode);
								pila.push(ruleNode);
							}
							*/

							strProd = getStrProduccion(LadosDerechos[produccion][i]);
							cout << "  " << strProd << "\n";
							ruleNode = new TreeNode(topNode);
							ruleNode->SetData(LadosDerechos[produccion][i], getStrProduccion(LadosDerechos[produccion][i]));
							topNode->AddChildren(ruleNode);
							pila.push(ruleNode);
						}
					}
					/*
					if (iSemantic < MAXLADODER + 1) {
						//agregar a la pila una regla semantica.
						if (ReglasSemanticas[produccion][iSemantic] != -1) {
							ruleNode = new TreeNode(topNode);
							ruleNode->SetData(ReglasSemanticas[produccion][iSemantic], getStrProduccion(ReglasSemanticas[produccion][iSemantic]));
							topNode->AddChildren(ruleNode);
							pila.push(ruleNode);
						}
					}
					*/
				}
				else {
					cout << "Error: No hay ninguna producción para " << strProd << " para el token: " << current_token.toString() << "\n";
					contError += 1;

					while (current_token.getTipoToken() != (TipoToken)TOKEN_EOF && !foundTokenIn(current_token, top)) {
						cout << "Saltando token: " << current_token.toString() << "\n";
						current_token = scanner.demeToken();
						pedirToken = false;
						string strLine = std::to_string(current_token.getCurrentLine());
						string strPos = std::to_string(current_token.getCurrentPosInLine());
						cout << "leyendo token: " << current_token.toString() << " " << current_token.getLexema() << "  , Line:" << strLine << ", Col:" << strPos << "\n";
					}
					if (foundTokenIn(current_token, top)) {
						cout << "Recuperado en token: " << current_token.toString() << "\n";
					}
					else {
						cout << "No se pudo recuperar del error para el no terminal: " << getStrProduccion(top) << "\n"; 
						result = 1;
						break;
					}
				}
			}
		}//while
		cout << "***************************************************************\n";
		if (pila.empty()) {
			if (contError == 0) {
				cout << "Parsing exitoso!" << "\n";
				result = 0;
			}
			else {
				cout << "Parsing con " << std::to_string(contError) << " error(es)\n";
			}
		}
		else {
			cout << "Parsing falló.\n";
			result = 3;
		}
		if (!erroresSemanticos.empty()) {
			cout << "Análisis Semántico: " + to_string(erroresSemanticos.size()) + " errores semánticos:\n";
			for (string strErr : erroresSemanticos) {
				cout << strErr;
			}
		}
		else {
			cout << "Análisis Semántico: No hay errores semánticos\n";
		}
		cout << "***************************************************************\n";

		cout << "\n" << "------------------------"  << "\n" << "Árbol Sintáctico: " << "\n" << "------------------------" << "\n";
		if (syntaxTree != NULL) {
			syntaxTree->print(0);
		}

		return result;
	}//Parse

	void CParser::genDataSegment(ofstream * genCodeFile) {
		memoryTable->genDataSegment(genCodeFile);
	}
}