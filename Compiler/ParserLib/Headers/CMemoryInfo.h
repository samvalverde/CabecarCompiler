#ifndef MEMORY_INFO_H
#define MEMORY_INFO_H

#include <string>
#include <vector>

using namespace std;

namespace ParserLib
{
;

	class CMemoryInfo
	{
	private:
		std::string varname;
		int size;//cuánto espacio ocupa la variable como tal
		int offset;//dónde está la variable
		int offsetRef; //dónde inicia el valor
		int length;
		int tipo; //1:int, 2:char, 3:string...
		string initialValue;

	public:
		CMemoryInfo();
		CMemoryInfo(const std::string varname, int tipo, int size, int offset, string initialValue);
		std::string getVarName();
		int getSize();
		int getOffset();
		string getInitialValue();
		void setVarName(string name);
		void setSize(int);
		void setOffset(int);
		void setInitialValue(int);
		int getTipo();
		void setTipo(int);
		int getOffsetRef();
		void setOffsetRef(int);
	};
}

#endif