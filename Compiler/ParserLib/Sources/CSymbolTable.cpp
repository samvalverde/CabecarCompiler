#include <iostream>
#include <string>
#include <fstream>

#include "CSymbolInfo.h"
#include "CSymbolTable.h"
#include <unordered_map>

using namespace std;

namespace ParserLib
{

	// Insertar un símbolo en la tabla
	void CSymbolTable::addSymbol(const int symbolType, const std::string name, const std::string type, const std::string value, int & errorCode ) {
		errorCode = 0;
		if (table.find(name) != table.end()) {
			errorCode = 1;
			//std::cerr << "Error: El símbolo '" << name << "' ya existe en la tabla." << std::endl;
		}
		else {
			table[name] = CSymbolInfo(name, type, value, symbolType);
			//std::cout << "Símbolo '" << name << "' insertado correctamente." << std::endl;
		}
	}

	CSymbolInfo* CSymbolTable::getSystemFunc(const std::string name) {
		CSymbolInfo* result = nullptr;
		if (name == "BákShéj") {
			//subrutina para imprimir
			result = new CSymbolInfo(name, "", "", ST_Rutina);
			CSymbolInfo* param = new CSymbolInfo("msg", "", "", ST_Variable);
			result->addFormalParam(param);
		}
		return result;
	}

	// Buscar un símbolo en la tabla
	CSymbolInfo* CSymbolTable::getSymbol(const std::string name, bool total) {
		auto it = table.find(name);
		if (it != table.end()) {
			return &it->second;
		}
		else {
			CSymbolInfo* result;
			if (parent != nullptr && total) {
				result = parent->getSymbol(name, total);
			}
			else {
				result = getSystemFunc(name);
			}
			//std::cerr << "Error: El símbolo '" << name << "' no fue encontrado." << std::endl;
			return result;
		}
	}

	// Mostrar todos los símbolos en la tabla
	void CSymbolTable::display() {
		std::cout << "Tabla de símbolos:" << std::endl;
		for (const auto &entry : table) {
			std::cout << "Nombre: " << entry.first << ", Tipo: " << ((CSymbolInfo)entry.second).getSymbolType() << ", Valor: " << ((CSymbolInfo)entry.second).getValue() << std::endl;
		}
	}

	string CSymbolTable::ReplaceAll(string str, const string& from, const string& to)
	{
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		return str;
	}

	void CSymbolTable::ReserveConstVarMemory(CMemoryTable * memoryTable)
	{
		int size_tipo = 0;
		CSymbolInfo symbol;
		string strName;
		string value;
		int errorCode;
		int tipo = 0;

		for (const auto &entry : table) {
			strName = entry.first;
			symbol = ((CSymbolInfo)entry.second);
			if (symbol.getType() == "TOKEN_SHTAWA") 
			{	//integer
				size_tipo = 2;
				tipo = 1;
				value = symbol.getValue();
			}
			else if (symbol.getType() == "TOKEN_SHEJ") {
				//string
				tipo = 3;
				size_tipo = 2;
				value = symbol.getValue();
				value = ReplaceAll(value, "\"", "'") + ", 0";
				size_tipo = value.length()+1;
			}
			else if (symbol.getType() == "TOKEN_EKLA") {
				//char
				tipo = 2;
				size_tipo = 1;
				value = symbol.getValue();
			}
			else if (symbol.getType() == "TOKEN_YINAKULIWA") {
				//char
				tipo = 2;
				size_tipo = 1;
				value = symbol.getValue();
			}
			memoryTable->addMemoryInfo(strName, tipo, size_tipo, memoryTable->getCurrentOffset(), value, errorCode);
		}
	}

	/*
	void CSymbolTable::genVarCode(ofstream * genCodeFile ) {
		string tipo;
		string value;
		CSymbolInfo symbol;
		for (const auto &entry : table) {
			(*genCodeFile) << "    " + entry.first + " "; 
			symbol = ((CSymbolInfo)entry.second);
			value = "";
			if (symbol.getType() == "TOKEN_SHTAWA") {
				tipo = "dw ";
				value = symbol.getValue();
			}
			else if (symbol.getType() == "TOKEN_SHEJ") {
				tipo = "db ";
				value = symbol.getValue();
				value = ReplaceAll(value, "\"", "'") + ", 0";
			}
			else if (symbol.getType() == "TOKEN_EKLA") {
				tipo = "db ";
				value = symbol.getValue();
			}
			else if (symbol.getType() == "TOKEN_YINAKULIWA") {
				tipo = "db ";
				value = symbol.getValue();
			}
			(*genCodeFile) << tipo + value + "\n";
		}
	}
	*/

	string CSymbolTable::genCodeAssignConstString() {
		string result = "";
		string dato;
		CSymbolInfo symbol;
		string tipo;
		string value;
		string nombreVar;
		for (const auto &entry : table) {
			dato = "";
			nombreVar = entry.first;
			symbol = ((CSymbolInfo)entry.second);
			dato = "";
			nombreVar = entry.first;
			symbol = ((CSymbolInfo)entry.second);
			if (symbol.getSymbolType() == ST_Variable && symbol.getValue() != "" && (symbol.getType() == "TOKEN_SHEJ")) {
				result += "  MOV AX, NextFree\n";
				result += "  MOV " + nombreVar + ", AX\n";//se guarda el offset donde inicia el nuevo string
				result += "  MOV CX, " + to_string(symbol.getValue().length()+1-2) + "; Establece el tamaño del string a reservar en  bytes\n";//el -2 es porque hay que quitar las 2 comillas
				result += "  CALL AllocateString\n\n";

				result += "  MOV AX, SEG const_" + nombreVar + ";Cargar el segmento de la constante en DS\n";
				result += "  MOV DS, AX\n";
				result += "  MOV SI, OFFSET const_" + nombreVar + "; Cargar el offset del string de origen en SI\n";
				result += "  MOV AX, " + nombreVar + "\n";
				result += "  MOV DI, AX ;Offset de inicio en MemoriaDinamica\n";
				result += "  CALL CopyStringToDynamicMemory; Llamada a la función de copia\n\n";
			}

		}
		return result;
	}

	void CSymbolTable::genVarCode(CMemoryTable * memoryTable) {
		string dato;
		CSymbolInfo symbol;
		string tipo;
		string value;
		string nombreVar;
		for (const auto &entry : table) {
			dato = "";
			nombreVar = entry.first;
			symbol = ((CSymbolInfo)entry.second);
			if (
				(symbol.getSymbolType() == ST_Constante) ||
				(symbol.getSymbolType() == ST_Variable)
				) {
				if (symbol.getType() == "TOKEN_SHTAWA") {
					tipo = "dw ";
					value = symbol.getValue();
				}
				else if (symbol.getType() == "TOKEN_SHEJ") {
					if (symbol.getSymbolType() == ST_Constante)  //constante
					{
						tipo = "db ";
						value = symbol.getValue();
						value = ReplaceAll(value, "\"", "'") + ", 0";
					}
					else if (symbol.getSymbolType() == ST_Variable //variable
						&& symbol.getValue() != "") {
						dato = "  " + nombreVar + " " + "dw ?";
						memoryTable->addDataConstVar(dato);//se crea la variable para guardar el offset
						nombreVar = "const_" + nombreVar;
						tipo = "db ";
						value = symbol.getValue();
						value = ReplaceAll(value, "\"", "'") + ", 0";
					}
					else if (symbol.getSymbolType() == ST_Variable //variable
						&& symbol.getValue() == "") {
						nombreVar = nombreVar;
						tipo = "dw ";
						value = "; es variable string que va a apuntar a algún string";
					}
					else {
						tipo = "";
					}
				}
				else if (symbol.getType() == "TOKEN_EKLA") {
					tipo = "db ";
					value = symbol.getValue();
				}
				else if (symbol.getType() == "TOKEN_YINAKULIWA") {
					tipo = "db ";
					value = symbol.getValue() == "Síwa" ? "1" : "0";
				}
				if (tipo != "") {
					dato = "  " + nombreVar + " " + tipo + value;
					memoryTable->addDataConstVar(dato);
				}
			}
		}
	}



	void CSymbolTable::genParamsCode(ofstream * genCodeFile) {
		int size_tipo;
		string value;
		CSymbolInfo symbol;
		for (const auto &entry : table) {
			(*genCodeFile) << "    ;" + entry.first + " ";
			symbol = ((CSymbolInfo)entry.second);
			value = "";
			if (symbol.getType() == "TOKEN_SHTAWA") {
				size_tipo = 2;
				value = symbol.getValue();
			}
			else if (symbol.getType() == "TOKEN_SHEJ") {
				size_tipo = 2;
				value = symbol.getValue();
				value = ReplaceAll(value, "\"", "'") + ", 0";
			}
			else if (symbol.getType() == "TOKEN_EKLA") {
				size_tipo = 1;
				value = symbol.getValue();
			}
			else if (symbol.getType() == "TOKEN_YINAKULIWA") {
				size_tipo = 1;
				value = symbol.getValue();
			}			
			else {
				size_tipo = 2;
				value = symbol.getValue();
			}
			(*genCodeFile) << to_string(size_tipo) + value + "\n";
		}
	}

	void CSymbolTable::genPosPila() {
		int size_tipo;
		string value;
		CSymbolInfo symbol;
		int posPila = 4;
		for (const auto &entry : table) {
			symbol = ((CSymbolInfo)entry.second);
			value = "";
			if (symbol.getType() == "TOKEN_SHTAWA") {
				size_tipo = 2;
			}
			else if (symbol.getType() == "TOKEN_SHEJ") {
				size_tipo = 2;
			}
			else if (symbol.getType() == "TOKEN_EKLA") {
				size_tipo = 1;
			}
			else if (symbol.getType() == "TOKEN_YINAKULIWA") {
				size_tipo = 1;
			}
			else {
				size_tipo = 2;
			}
			symbol.setPosPila(posPila);
			posPila += size_tipo;
		}
	}


}