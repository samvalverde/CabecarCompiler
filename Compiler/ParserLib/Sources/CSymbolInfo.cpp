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

	bool CSymbolInfo::getIsReference() {
		return this->isReference;
	}

	void CSymbolInfo::setIsReference(bool isRef) {
		this->isReference = isRef;
	}

	vector<CSymbolInfo *> * CSymbolInfo::getFormalParams() {
		return &(this->lstFormalParams);
	}

	vector<CSymbolInfo *> * CSymbolInfo::getFunctionalParams() {
		return &(this->lstFunctionalParams);
	}


	void CSymbolInfo::addFormalParam(CSymbolInfo * param) {
		lstFormalParams.insert(lstFormalParams.end(), param);
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
