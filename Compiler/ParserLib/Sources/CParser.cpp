#include "CParser.h"

#include <iostream>
#include <string>
#include <stack>
#include <list>
#include "GRAMATIC.h"
#include "FOLLOW.h"
#include "CToken.h"
#include "CScanner.h"
#include "TreeNode.h"

using namespace std;
using namespace ScannerLib;


namespace ParserLib {

	bool isTerminal(int prod) {
		return TERMINAL(prod);
	}

	string getStrProduccion(int prod) {
		if (isTerminal(prod)) {
			Token token;
			token.setTipoToken((TipoToken)prod);
			return token.toString();
		}
		else {
			return ListaTerminales[prod - SIMBOLOINICIAL];
		}
	}

	bool foundTokenIn(Token token, int top) {

		bool found = false;
		string infoTop = getStrProduccion(top);
		TipoToken tipoToken = token.getTipoToken();
		for (int i = 0; i < MAXFOLLOWS; i++) {
			if (TablaFollows[top - SIMBOLOINICIAL][i] == (int)tipoToken) {
				found = true;
				break;
			}
			else if (TablaFollows[top - SIMBOLOINICIAL][i] == -1) {
				break;
			}
		}
		return found;
	}

	// TODO: This is an example of a library function
	int CParser::Parse(string fileName)
	{
		int result = 0;
		int contError = 0;
		stack<TreeNode * > pila;
		int top;
		int produccion;
		Token current_token;
		bool pedirToken;
		CScanner scanner;
		TreeNode * topNode;


		int resInit = scanner.inicializarScanner(fileName);

		if (resInit == 1) {
			return 1;
		}

		TreeNode * syntaxTree = new TreeNode();
		syntaxTree->SetData(SIMBOLOINICIAL, getStrProduccion(SIMBOLOINICIAL));
		pila.push(syntaxTree);
		pedirToken = true;

		while (!pila.empty()) {
			topNode = pila.top();
			top = topNode->GetIdRegla();
			pila.pop();
			if (pedirToken) {
				current_token = scanner.demeToken();
				string strLine = std::to_string(current_token.getCurrentLine());
				string strPos = std::to_string(current_token.getCurrentPosInLine());
				cout << "leyendo token: " << current_token.toString() << " " << current_token.getLexema() << "  , Line:" << strLine << "Col:" << strPos  <<"\n";
			}
			pedirToken = false;

			if (isTerminal(top))  // Terminal check
			{
				if (top == (int)current_token.getTipoToken()) {
					cout << "MATCH TOKEN : " << current_token.toString() << "\n";
					pedirToken = true; //Advance input
					TreeNode * tokenNode = new TreeNode(topNode);
					tokenNode->SetData(top, getStrProduccion(top));
					topNode->AddChildren(tokenNode);
				}
				else {
					cout << "Error: Se esperaba " << getStrProduccion(top) << ", pero se encontró " << current_token.toString() << ". Saltando token" << "\n";
					contError += 1;
					pedirToken = true;//se indica que debe pedir otro token
				}
			}
			else // Non - terminal 
			{
				int tipoToken = (int)current_token.getTipoToken();
				produccion = TablaParsing[top-SIMBOLOINICIAL][tipoToken];
				string strProd;
				strProd = getStrProduccion(top);
				if (produccion != -1) {
					//Push production to stack
					cout << "REPLACE STACK : " << strProd << " por: \n";
					TreeNode * ruleNode;
					for (int i = 0; i < MAXLADODER; i++) {
						if (LadosDerechos[produccion][i] == -1) {
							break;
						}
						else {
							strProd = getStrProduccion(LadosDerechos[produccion][i]);
							cout << "  " << strProd << "\n";
							ruleNode = new TreeNode(topNode);
							ruleNode->SetData(LadosDerechos[produccion][i], getStrProduccion(LadosDerechos[produccion][i]));
							topNode->AddChildren(ruleNode);
							pila.push(ruleNode);
						}
					}
				}
				else {
					cout << "Error: No hay ninguna producción para " << strProd << " para el token: " << current_token.toString() << "\n";
					contError += 1;

					while (current_token.getTipoToken() != (TipoToken)TOKEN_EOF && !foundTokenIn(current_token, top)) {
						cout << "Saltando token: " << current_token.toString() << "\n";
						current_token = scanner.demeToken();
						pedirToken = false;
						string strLine = std::to_string(current_token.getCurrentLine());
						string strPos = std::to_string(current_token.getCurrentPosInLine());
						cout << "leyendo token: " << current_token.toString() << " " << current_token.getLexema() << "  , Line:" << strLine << ", Col:" << strPos << "\n";
					}
					if (foundTokenIn(current_token, top)) {
						cout << "Recuperado en token: " << current_token.toString() << "\n";
					}
					else {
						cout << "No se pudo recuperar del error para el no terminal: " << getStrProduccion(top) << "\n"; 
						result = 1;
						break;
					}
				}
			}
		}//while
		if (pila.empty()) {
			if (contError == 0) {
				cout << "Parsing exitoso!" << "\n";
				result = 0;
			}
			else {
				cout << "Parsing con " << std::to_string(contError) << " error(es)\n";
			}
		}
		else {
			cout << "Parsing falló.\n";
			result = 3;
		}

		cout << "\n" << "------------------------"  << "\n" << "Árbol Sintáctico: " << "\n" << "------------------------" << "\n";
		if (syntaxTree != NULL) {
			syntaxTree->print(0);
		}

		return result;
	}//Parse


}