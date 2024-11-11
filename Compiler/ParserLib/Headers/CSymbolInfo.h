#ifndef SYMBOL_INFO_H
#define SYMBOL_INFO_H

#include <string>
#include <vector>

using namespace std;

namespace ParserLib
{

	enum ConstSymbolType {
		ST_Constante = 1,
		ST_Variable = 2,
		ST_UserType = 3,
		ST_Funcion = 4,
		ST_Rutina = 5,
		ST_Indefinido = 6
	};

	class CSymbolInfo
	{
	private:
		std::string name;
		std::string type;  // El tipo del símbolo (ej. "int", "float", etc.)
		std::string value;         // Valor asociado al símbolo (esto es solo un ejemplo, podrías hacer esto más general)
		std::string dims;
		bool isReference;  // esto es para parámetros por referencia.
		std::vector<CSymbolInfo *> lstFormalParams;
		std::vector<CSymbolInfo *> lstFunctionalParams;
		int symbolType; //1:constantes, 2: tipos del usuario, 3:variables, 4: prototipos, 5: Rutinas
		//para generacion de código
		int posPila = 0;
		string callValue;

	public:
		CSymbolInfo();
		CSymbolInfo(const std::string name, const std::string type, const std::string value, const int symbolType);

		std::string getName();
		std::string getType();
		std::string getValue();
		std::string getDims();
		int getPosPila();
		std::string getCallValue();
		bool getIsReference();
		void setName(string name);
		void setValue(string value);
		void setType(string type);
		void setDims(string dims);
		void setIsReference(bool isRef);
		void setPosPila(int posPila);
		void setCallValue(string value);
		int getSymbolType();
		void addFormalParam(CSymbolInfo * param, bool checkDuplicate = false);
		void addFunctionalParam(CSymbolInfo * param);
		vector<CSymbolInfo *> * getFormalParams();
		vector<CSymbolInfo *> * getFunctionalParams();
		int getFormalParamsSize();
		CSymbolInfo * clone();
	};
}

#endif