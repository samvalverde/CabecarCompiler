#ifndef MEMORY_TABLE_H
#define MEMORY_TABLE_H

#include <iostream>
#include <unordered_map>
#include <string>
#include "CMemoryInfo.h"

using namespace std;

namespace ParserLib
{

	class CMemoryTable
	{
	private:
		// Mapa de s�mbolos, con el nombre como clave y la informaci�n del s�mbolo como valor
		std::unordered_map<std::string, CMemoryInfo> table;
		int currentOffset = 0;

		std::list<string> dataConstVar;

	public:

		CMemoryTable();

		// Insertar un s�mbolo en la Memoria
		void addMemoryInfo(const std::string varname, int tipo, int size, int offset, string initialValue, int &errorCode);

		// Buscar un s�mbolo en la tabla
		CMemoryInfo* getMemoryInfo(const std::string varname);

		//void genCodForConstVarValues(ofstream * genCodeFile);

		int getCurrentOffset();

		void addDataConstVar(string dato);

		void genDataSegment(ofstream * genCodeFile);

	};
}

#endif

