#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <unordered_map>
#include <string>
#include "CMemoryTable.h";
#include "CSymbolInfo.h"

using namespace std;

namespace ParserLib
{

	class CSymbolTable
	{
	private:
		// Mapa de símbolos, con el nombre como clave y la información del símbolo como valor
		std::unordered_map<std::string, CSymbolInfo> table;

	public:
		CSymbolTable * parent = nullptr;
		// Insertar un símbolo en la tabla
		void addSymbol(const int symbolType, const std::string name, const std::string type, std::string value, int &errorCode);

		// Buscar un símbolo en la tabla
		CSymbolInfo* getSymbol(const std::string name, bool total = false);
		//Reemplaza la variable temporal que se tuvo que usar para definir los campos del registro por el ID definido posteriormente

		CSymbolInfo* getSystemFunc(const std::string name);

		// Mostrar todos los símbolos en la tabla
		void display();

		void genVarCode(CMemoryTable * memoryTable);
		string genCodeAssignConstString();

		void genParamsCode(ofstream * genCodeFile);
		void genPosPila();

		void ReserveConstVarMemory(CMemoryTable * memoryTable);

		static string ReplaceAll(string str, const string& from, const string& to);

	};
}

#endif

