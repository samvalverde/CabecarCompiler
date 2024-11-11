#include <iostream>
#include <string>

#include "CMemoryInfo.h"

using namespace std;

namespace ParserLib
{
	CMemoryInfo::CMemoryInfo() {
		this->varname = "";
		this->size = 1;
		this->offset = 0;
		this->initialValue = "";
		this->tipo = 0;
	}

	CMemoryInfo::CMemoryInfo(const std::string varname, int tipo, int size, int offset, string initialValue) {
		this->varname = varname;
		this->size = size;
		this->offset = offset;
		this->initialValue = initialValue;
		this->tipo = tipo;
	}

	std::string CMemoryInfo::getVarName() {
		return this->varname;
	}

	void CMemoryInfo::setVarName(string varname) {
		this->varname = varname;
	}

	int CMemoryInfo::getSize() {
		return this->size;
	}

	void CMemoryInfo::setSize(int size) {
		this->size = size;
	}

	int CMemoryInfo::getOffset() {
		return this->offset;
	}

	void CMemoryInfo::setOffset(int offset) {
		this->offset = offset;
	}

	int CMemoryInfo::getTipo() {
		return this->tipo;
	}

	void CMemoryInfo::setTipo(int tipo) {
		this->tipo = tipo;
	}

	int CMemoryInfo::getOffsetRef() {
		return this->offsetRef;
	}

	void CMemoryInfo::setOffsetRef(int offsetRef) {
		this->offsetRef = offsetRef;
	}



}
