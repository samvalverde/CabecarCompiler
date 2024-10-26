#include <iostream>
#include <string>

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
}