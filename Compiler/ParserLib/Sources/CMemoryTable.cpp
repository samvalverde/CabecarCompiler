#include <iostream>
#include <string>
#include <fstream>

#include "CSymbolInfo.h"
#include "CMemoryTable.h"
#include <unordered_map>

using namespace std;

namespace ParserLib
{
	CMemoryTable::CMemoryTable() {
	}

	// Insertar un símbolo en la tabla
	void CMemoryTable::addMemoryInfo(const std::string varname, int tipo, int size, int offset, string initialValue, int &errorCode) {
		errorCode = 0;
		if (table.find(varname) != table.end()) {
			errorCode = 1;
			//std::cerr << "Error: El símbolo '" << name << "' ya existe en Memory Table." << std::endl;
		}
		else {
			table[varname] = CMemoryInfo(varname, tipo, size, offset, initialValue);
			currentOffset += size;
		}
	}

	// Buscar un símbolo en la tabla
	CMemoryInfo* CMemoryTable::getMemoryInfo(const std::string varname) {
		auto it = table.find(varname);
		if (it != table.end()) {
			return &it->second;
		}
		else {
			return NULL;
		}
	}

	/*
	void CMemoryTable::genCodForConstVarValues(ofstream * genCodeFile) {
		CMemoryInfo info;
		for (const auto &entry : table) 
		{
			//(*genCodeFile) << "    " + entry.first + " ";
			info = ((CMemoryInfo)entry.second);
			if (info.getTipo() == 1) {
				//int:
				(*genCodeFile) << " MOV AX, " + info.getInitialValue() + "\n";
				(*genCodeFile) << " MOV DI, OFFSET memDatosProg\n";
				(*genCodeFile) << " ADD DI, " + to_string(info.getOffset()) + "\n";
				(*genCodeFile) << " MOV [DI], AX\n";
			}
			else if (info.getTipo() == 2) {
				//char:
				(*genCodeFile) << " MOV AL, " + info.getInitialValue() + "\n";
				(*genCodeFile) << " MOV DI, OFFSET memDatosProg\n";
				(*genCodeFile) << " ADD DI, " + to_string(info.getOffset()) + "\n";
				(*genCodeFile) << " MOV [DI], AL\n";
			}
			else if (info.getTipo() == 3) {
				info.setOffsetRef(this->currentOffset);
				(*genCodeFile) << " MOV AX, " + to_string(this->currentOffset) + "\n";
				(*genCodeFile) << " MOV DI, OFFSET memDatosProg\n";
				(*genCodeFile) << " ADD DI, AX\n";
			}
		}
	}
	*/

	int CMemoryTable::getCurrentOffset() {
		return this->currentOffset;
	}

	void CMemoryTable::addDataConstVar(string dato) {
		dataConstVar.insert(dataConstVar.end(), dato);
	}

	void CMemoryTable::genDataSegment(ofstream * genCodeFile) {
		(*genCodeFile) << ";------------------ DATA --------------------\n";
		for (string elem : dataConstVar) {
			(*genCodeFile) << elem + "\n";
		}
	}

}