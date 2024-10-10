// ParserApp.cpp 

#include <iostream>
#include <string>
#include <list>
#include "CToken.h"
#include "CScanner.h"
#include "CParser.h"

using namespace ParserLib;
using namespace ScannerLib;

string coloresLadrillo[] =
{
	"gray",		//TOKEN_INT
	"green",	//TOKEN_ID
	"yellow",	//TOKEN_CHAR
	"IndianRed",	//TOKEN_STRING,					
	"LightCoral",	//TOKEN_TIPO,					
	"Salmon",	//TOKEN_INICIALIZAR,			
	"DarkSalmon",	//TOKEN_ABRIR_CONJUNTO,			
	"LightSalmon",	//TOKEN_CERRAR_CONJUNTO,		
	"Cyan",			//TOKEN_ABRIR_REGISTRO,			
	"DeepSkyBlue",	//TOKEN_CERRAR_REGISTRO,	
	"DarkMagenta", //TOKEN_PAR_REDONDO_ABRIR,
	"Indigo", //TOKEN_PAR_REDONDO_CERRAR,
	"HotPink", //TOKEN_PAR_CUADRADO_ABRIR,
	"DarkTurquoise", //TOKEN_PAR_CUADRADO_CERRAR,
	"MediumVioletRed", //TOKEN_ARROBA,	
	"PaleVioletRed", //TOKEN_MAS,
	"PaleVioletRed", //TOKEN_MENOS,
	"PaleVioletRed", //TOKEN_MULTIPLICACION,
	"PaleVioletRed", //TOKEN_DIVISION,
	"PaleVioletRed", //TOKEN_MODULO,
	//"PaleVioletRed", //TOKEN_OPERADOR,
	//"Coral", //TOKEN_OPERADOR_ROMANO,
	"Coral", //TOKEN_MAS_ROMANO,
	"Coral", //TOKEN_MENOS_ROMANO,
	"Coral", //TOKEN_MULTIPLICACION_ROMANO,
	"Coral", //TOKEN_DIVISION_ROMANO,
	"Coral", //TOKEN_MODULO_ROMANO,
	"LightGreen", //TOKEN_PUNTO,		
	"DarkOrange", //TOKEN_COMA,			
	"Orange", //TOKEN_COMPARADOR,	
	"Gold", //TOKEN_CHAR_POS,		
	"DarkKhaki", //TOKEN_ABRIR_ROMANO,	
	"Orchid", //TOKEN_CERRAR_ROMANO
};

string fontLadrillo[] =
{
	"white",	//TOKEN_INT
	"white",	//TOKEN_ID
	"black",	//TOKEN_CHAR
	"white",	//TOKEN_STRING,					
	"black",	//TOKEN_TIPO,					
	"black",	//TOKEN_INICIALIZAR,			
	"black",	//TOKEN_ABRIR_CONJUNTO,			
	"black",	//TOKEN_CERRAR_CONJUNTO,		
	"black",	//TOKEN_ABRIR_REGISTRO,			
	"black",	//TOKEN_CERRAR_REGISTRO,	
	"white", //TOKEN_PAR_REDONDO_ABRIR,
	"white", //TOKEN_PAR_REDONDO_CERRAR,
	"black", //TOKEN_PAR_CUADRADO_ABRIR,
	"black", //TOKEN_PAR_CUADRADO_CERRAR,
	"white", //TOKEN_ARROBA,	
	"black", //TOKEN_MAS,
	"black", //TOKEN_MENOS,
	"black", //TOKEN_MULTIPLICACION,
	"black", //TOKEN_DIVISION,
	"black", //TOKEN_MODULO,
	//"black", //TOKEN_OPERADOR,
	//"black", //TOKEN_OPERADOR_ROMANO,
	"black", //TOKEN_MAS_ROMANO,
	"black", //TOKEN_MENOS_ROMANO,
	"black", //TOKEN_MULTIPLICACION_ROMANO,
	"black", //TOKEN_DIVISION_ROMANO,
	"black", //TOKEN_MODULO_ROMANO,
	"black", //TOKEN_PUNTO,		
	"black", //TOKEN_COMA,			
	"black", //TOKEN_COMPARADOR,	
	"black", //TOKEN_CHAR_POS,		
	"black", //TOKEN_ABRIR_ROMANO,	
	"black", //TOKEN_CERRAR_ROMANO

};

string htmlLadrillo(Token token, string colorLadrillo) {
	string strOut = "<div class='ladrillo " + colorLadrillo +
		"'><span class='cssToken'>" + token.toString() +
		"</span><br><span class='cssLexema'>" + token.getLexema() +
		"</span><br><span class='cssLine'>Line:" + to_string(token.getCurrentLine()) +
		"</span><br><span class='cssPosInLine'>Col:" + to_string(token.getCurrentPosInLine()) +
		"</span></div>\n";
	return strOut;
}

void guardarTokenMuralla(FILE * fileHTML, Token token) {
	string colorLadrillo;
	if (token.isTokenKeyword()) {
		colorLadrillo = "ladrilloAzul";
	}
	else if (token.getTipoToken() >= TOKEN_INT && token.getTipoToken() <= TOKEN_ROMANO) {
		int i = (int)token.getTipoToken();
		colorLadrillo = "ladrillo_" + coloresLadrillo[i];
	}
	else {
		//errores
		colorLadrillo = "ladrilloRed";
	}
	//char sLadrillo[1024];
	//sprintf_s(sLadrillo, 1024, "<div class='ladrillo %s'><span class='cssToken'>%s</span><br><span class='cssLexema'>%s</span><br><span class='cssLine'>Line:" + %lu + "</span><br><span class='cssPosInLine'>Col:" + %lu + "</span></div>\n", colorLadrillo.c_str(), token.toString().c_str(), token.getLexema().c_str(), token.getCurrentLine(), token.getCurrentPosInLine() ) ;
	string sLadrillo = htmlLadrillo(token, colorLadrillo);
	std::fprintf(fileHTML, sLadrillo.c_str());
}


void generarHeaderStyleHTML(FILE * fileHTML) {
	std::fprintf(fileHTML, "<html><head>");
	std::fprintf(fileHTML, "<style>");
	std::fprintf(fileHTML, ".ladrillo {width:fit-content; display:inline-block; border: solid 1px black; padding-top:10px; padding-bottom:10px; padding-left:10px; padding-right:10px; margin: 5px; }\n");

	int length = sizeof(coloresLadrillo) / sizeof(coloresLadrillo[0]);

	for (int i = 0; i < length; i++) {
		string sColor = ".ladrillo_" + coloresLadrillo[i] + " {background-color: " + coloresLadrillo[i] + "; color:" + fontLadrillo[i] + ";}\n";
		std::fprintf(fileHTML,  sColor.c_str());
	}
	std::fprintf(fileHTML, ".ladrilloAzul {background-color: blue; color:white;}\n");
	std::fprintf(fileHTML, ".ladrilloRed {background-color: red; color:white;}\n");
	std::fprintf(fileHTML, "</style>");
	std::fprintf(fileHTML, "</head><body>");

}

string desplegarInfoToken(Token token) {

	string strOut = "Token: " + token.toString();

	strOut += ", posic:" + to_string(token.getGlobalPosic());
	strOut += ", Token Value: " + token.getLexema();
	strOut += " Line:" + to_string(token.getCurrentLine());
	strOut += " Posic-in-line:" + to_string(token.getCurrentPosInLine());
	strOut += "\n";

	return strOut;
}


int main(int argc, char *argv[])
{

	setlocale(LC_ALL, "");
	string fileName;
	std::cout << "Hola!! Soy el Parser Cabecar\n";

	if (argc > 1) {
		fileName = string(argv[1]);
	}
	else {
		std::cout << "Archivo de entrada no especificado\n";
		return 1;
	}

	std::cout << "Parsing: " << fileName << "\n";

	CParser parser;


	int result = parser.Parse(fileName);
	if (result == 1) {
		std::cout << "Error abriendo archivo: " << fileName << "\n";
	}

}

int mainScanner(int argc, char *argv[])
{
	setlocale(LC_ALL, "");
	string fileName;
	std::cout << "Hola!! Soy el scanner Cabecar\n";

	if (argc > 1) {
		fileName = string(argv[1]);
	}
	else {
		std::cout << "Archivo de entrada no especificado\n";
		return 1;
	}

	std::cout << "Scanning: " << fileName << "\n";

	CScanner scanner;

	int resInit = scanner.inicializarScanner(fileName);

	if (resInit == 1) {
		std::cout << "Error abriendo archivo: " << fileName << "\n";
		return 1;
	}

	/* Archivo de Salida*/
	FILE *fileTokens;
	string outFileName(fileName);
	outFileName += ".out";
	errno_t res = fopen_s(&fileTokens, outFileName.c_str(), "w");
	if (res != 0) {
		printf("Error creando archivo de salida");
		return 1;
	}

	/* Archivo de Muralla de bloques*/
	FILE *fileHTML;
	string htmlFileName(fileName);
	htmlFileName += ".html";
	res = fopen_s(&fileHTML, htmlFileName.c_str(), "w");
	if (res != 0) {
		printf("Error creando archivo HTML de salida");
		return 1;
	}
	generarHeaderStyleHTML(fileHTML);

	// Extract tokens
	//std::list<Token> listaTokens;
	Token token;
	int EstadisticaTokens[CANT_TOKEN];
	for (int i = 0; i < CANT_TOKEN; i++)
	{
		EstadisticaTokens[i] = 0;
	}
	int iToken = 0;
	while ( (token = scanner.demeToken()).getTipoToken() != TOKEN_EOF) {
		//	listaTokens.push_back(token);
		iToken++;

		string strOut = desplegarInfoToken(token);
		std::cout << strOut;

		fprintf(fileTokens, strOut.c_str());

		guardarTokenMuralla(fileHTML, token);
		EstadisticaTokens[token.getTipoToken()] += 1;
	}

	scanner.finalizarScanner();

	std::cout << "\nFin de escaneo\n";

	std::cout << "\n**** Estadísticas **** \n";

	std::cout << "Cantidad de Caracteres: " << scanner.getCantidadChars() << "\n";
	std::cout << "Cantidad de Líneas: " << scanner.getCantidadLineas() << "\n";
	std::cout << "Cantidad de Tokens: " << scanner.getCantidadTokens() << "\n";
	std::cout << "Cantidad de Comentarios: " << scanner.getCantidadComentarios() << "\n";

	for (int i = 0; i < CANT_TOKEN; i++)
	{
		Token tokenStat;
		tokenStat.setTipoToken((TipoToken)i);
		string strOut = "Token: " + tokenStat.toString() + ", cantidad:" + to_string(EstadisticaTokens[i]) + "\n";
		cout << strOut;
		fprintf(fileTokens, strOut.c_str());
	}
	fclose(fileTokens);//cierra archivo de salida

	fprintf(fileHTML, "</body></html>\n");
	fclose(fileHTML);//cierra archivo de salida HTML

	return 0;

}
