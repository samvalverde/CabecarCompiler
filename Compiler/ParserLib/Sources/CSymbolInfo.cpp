#include <iostream>
#include <string>

#include "CSymbolInfo.h"

using namespace std;

namespace ParserLib
{
	CSymbolInfo::CSymbolInfo() {
		this->type = "";
		this->symbolType = 0;
		this->value = "";
	}

	CSymbolInfo::CSymbolInfo(const std::string name, const std::string type, const std::string value, const int symbolType) {
		this->name = name;
		this->type = type;
		this->value = value;
		this->symbolType = symbolType;
	}

	std::string CSymbolInfo::getType() {
		return this->type;
	}

	void CSymbolInfo::setType(string type) {
		this->type = type;
	}

	std::string CSymbolInfo::getValue() {
		return this->value;
	}

	void CSymbolInfo::setValue(string value) {
		this->value = value;
	}

	std::string CSymbolInfo::getDims() {
		return this->dims;
	}

	void CSymbolInfo::setDims(string dims) {
		this->dims = dims;
	}

	int CSymbolInfo::getSymbolType() {
		return this->symbolType;
	}

	std::string CSymbolInfo::getName() {
		return this->name;
	}

	void CSymbolInfo::setName(string name) {
		this->name = name;
	}

	void CSymbolInfo::setPosPila(int posPila) {
		this->posPila = posPila;
	}

	void CSymbolInfo::setCallValue(string value) {
		this->callValue = value;
	}

	int CSymbolInfo::getPosPila() {
		return this->posPila;
	}

	std::string CSymbolInfo::getCallValue() {
		return this->callValue;
	}


	bool CSymbolInfo::getIsReference() {
		return this->isReference;
	}

	void CSymbolInfo::setIsReference(bool isRef) {
		this->isReference = isRef;
	}

	vector<CSymbolInfo *> * CSymbolInfo::getFormalParams() {
		return &(this->lstFormalParams);
	}

	int CSymbolInfo::getFormalParamsSize() {
		int size_tipo = 0;
		int cont = 0;
		for (CSymbolInfo * symbol : this->lstFormalParams) {
			if (symbol->getType() == "TOKEN_SHTAWA") {
				size_tipo = 2;
			}
			else if (symbol->getType() == "TOKEN_SHEJ") {
				size_tipo = 4;//se guarda el segmento y el offset.
			}
			else if (symbol->getType() == "TOKEN_EKLA") {
				size_tipo = 2;
			}
			else if (symbol->getType() == "TOKEN_YINAKULIWA") {
				size_tipo = 2;
			}
			else {
				size_tipo = 2;
			}
			cont += size_tipo;
		}
		return cont;
	}

	vector<CSymbolInfo *> * CSymbolInfo::getFunctionalParams() {
		return &(this->lstFunctionalParams);
	}


	void CSymbolInfo::addFormalParam(CSymbolInfo * param, bool checkDuplicate) {
		bool insert = true;
		if (checkDuplicate) {
			insert = true;
			for (CSymbolInfo * symbol : lstFormalParams) {
				if (symbol->getName() == param->getName()) {
					insert = false;
					break;
				}
			}
		}
		if (insert) {
			lstFormalParams.insert(lstFormalParams.end(), param);
		}
	}

	void CSymbolInfo::addFunctionalParam(CSymbolInfo * param) {
		lstFunctionalParams.insert(lstFunctionalParams.end(), param);
	}

	CSymbolInfo * CSymbolInfo::clone() {
		CSymbolInfo * result = new CSymbolInfo(this->getName(), this->getType(), this->getValue(), this->getSymbolType());
		result->setDims(this->getDims());
		result->setIsReference(this->getIsReference());
		for (CSymbolInfo * p : this->lstFormalParams) {
			result->lstFormalParams.insert(result->lstFormalParams.end(), p->clone());
		}
		for (CSymbolInfo * p : this->lstFunctionalParams) {
			result->lstFunctionalParams.insert(result->lstFunctionalParams.end(), p->clone());
		}
		return result;
	}


}
