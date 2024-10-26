#ifndef CPARSER_H
#define CPARSER_H

#include <string>
#include <vector>
#include <stack>

#include "CSymbolTable.h"
#include "CSymbolInfo.h"
#include "TreeNode.h"
#include "CToken.h"

using namespace std;
using namespace ScannerLib;

namespace ParserLib
{
	class CParser {
		private:
			CSymbolTable * symbolTableGlobal;
			CSymbolTable * symbolTableLocal;
			CSymbolTable * currentSymbolTable;
			std::string definingVarsCurrentType;
			std::string definingCurrentId;
			vector<CSymbolInfo *> temporalRegVar;
			stack<CSymbolInfo *> pilaLlamados;
			vector<string> erroresSemanticos;

			int currentSection;
			void fnRS1003(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1004(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1005(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1006(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1007(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1008(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1009(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1010(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1011(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1012(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1013(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1014(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1015(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1016(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1017(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1018(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1019(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1020(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1021(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1022(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1023(int top, TreeNode * topNode, Token currentToken);
			void fnRS1024(int top, TreeNode * topNode, Token currentToken);
			void fnRS1025(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1026(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1027(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1028(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1029(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1030(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1031(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1032(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1033(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1034(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1035(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1036(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1037(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1038(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1039(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void fnRS1040(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
			void ejecutarAccionSemantica(int top, TreeNode * topNode, Token currentToken, CSymbolTable * currentSymbolTable);
		public:
			CParser();
			int Parse(string fileName);
	};
}
#endif

