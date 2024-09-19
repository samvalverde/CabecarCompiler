#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#define TOKEN_SIZE 256

using namespace std;

namespace ScannerLib
{

	// Tipo o familia de Token 
	typedef enum {
		TOKEN_INT,						//	###, -###, 0xABC, -0xABC
		TOKEN_ID,						//	abc, _abc
		TOKEN_CHAR,						//	'x'
		TOKEN_STRING,					//	"x"
		TOKEN_TIPO,						//	:>
		TOKEN_INICIALIZAR,				//	:=

		TOKEN_ABRIR_CONJUNTO,			//	{:
		TOKEN_CERRAR_CONJUNTO,			//	:}
		TOKEN_ABRIR_REGISTRO,			//	{*
		TOKEN_CERRAR_REGISTRO,			//	*}

		TOKEN_PAR_REDONDO_ABRIR,		//	(
		TOKEN_PAR_REDONDO_CERRAR,		//	)
		TOKEN_PAR_CUADRADO_ABRIR,		//	[
		TOKEN_PAR_CUADRADO_CERRAR,		//	]

		TOKEN_ARROBA,					//	@

		TOKEN_MAS,						//	+
		TOKEN_MENOS,					//	-
		TOKEN_MULTIPLICACION,			//	*
		TOKEN_DIVISION,					//	/
		TOKEN_MODULO,					//	%

		//TOKEN_OPERADOR,				// +, -, *, /, %
		//TOKEN_OPERADOR_ROMANO,		// wö+, wö-, wö*, wö/, wö%

		TOKEN_MAS_ROMANO,				//	wö+
		TOKEN_MENOS_ROMANO,				//	wö-
		TOKEN_MULTIPLICACION_ROMANO,	//	wö*
		TOKEN_DIVISION_ROMANO,			//	wö/
		TOKEN_MODULO_ROMANO,			//	wö%

		TOKEN_PUNTO,					//	.
		TOKEN_COMA,						//	,
		TOKEN_COMPARADOR,				//	=, >, >=, <, <=, ><
		TOKEN_CHAR_POS,					//	&#

		TOKEN_ABRIR_ROMANO,				//	w|
		TOKEN_CERRAR_ROMANO,			//	|w

		TOKEN_WOGAWG,
		TOKEN_KARA,
		TOKEN_KLO,
		TOKEN_KASKA,
		TOKEN_WAKIRI,
		TOKEN_PRA,
		TOKEN_JARREE,
		TOKEN_NAKJEE,
		TOKEN_SHTAWA,
		TOKEN_EKLA,
		TOKEN_SHEJ,
		TOKEN_YINAKULIWA,
		TOKEN_JARJA,
		TOKEN_KICHANA,
		TOKEN_BIKO,
		TOKEN_BOK,
		TOKEN_SIWA,
		TOKEN_KOYUWE,
		TOKEN_WAKA,
		TOKEN_DAKA,
		TOKEN_DAWA,
		TOKEN_TOLO,
		TOKEN_JELA,
		TOKEN_JURU,
		TOKEN_KARI,
		TOKEN_IRA,
		TOKEN_IRALE,
		TOKEN_KAI,
		TOKEN_JEBA,
		TOKEN_BAWA,
		TOKEN_MATSII,
		TOKEN_TIA,
		TOKEN_KJATIA,
		TOKEN_KUA,
		TOKEN_WOBOGO,
		TOKEN_SHENAWA,
		TOKEN_NAWOTKO,
		TOKEN_PAKAKIREI,
		TOKEN_DODWA,
		TOKEN_JAREBO,
		TOKEN_KA,
		TOKEN_TUINA,
		TOKEN_DALEIKA,
		TOKEN_SHA,
		TOKEN_CHAGO,
		TOKEN_JENA,
		TOKEN_SHIRINA,
		TOKEN_MANEWA,
		TOKEN_NUI,
		TOKEN_TSU,
		TOKEN_SHANI,
		TOKEN_KANA,
		TOKEN_TUNA,
		TOKEN_KATSO,
		TOKEN_NAI,
		TOKEN_TA,
		TOKEN_TULUWO,
		TOKEN_TSERE,
		TOKEN_SHANA,
		TOKEN_KIANA,
		TOKEN_CHANI,
		TOKEN_WAYUA,
		TOKEN_IAWA,
		TOKEN_TOLI,
		TOKEN_KUKLE,
		TOKEN_AWESHTAWA,
		TOKEN_AWEEKLA,
		TOKEN_AWESHEJ,
		TOKEN_AWEYINAKULIWA,
		TOKEN_AWEJARJA,
		TOKEN_AWEWOMELE,
		TOKEN_BAKSHTAWA,
		TOKEN_BAKEKLA,
		TOKEN_BAKSHEJ,
		TOKEN_BAKYINAKULIWA,
		TOKEN_BAKJARJA,
		TOKEN_BAKWOMELE,
		TOKEN_WEIKA,
		TOKEN_WOMELE,
		TOKEN_KALWA,
		TOKEN_SAKA,
		TOKEN_TOKA,
		TOKEN_NITSOSI,

		TOKEN_EOF,
		TOKEN_UNKNOWN,

		TOKEN_ERR_INT_B,
		TOKEN_ERR_MENOS_B,
		TOKEN_ERR_TOKEN_STRING,
		TOKEN_ERR_CHAR_PIPE_B,
		TOKEN_ERR_TOKEN_ID_B,
		TOKEN_ERR_2PUNTO_B,
		TOKEN_ERR_TIPO_B,
		TOKEN_ERR_INICIA_B,
		TOKEN_ERR_CERRCONJ_B,
		TOKEN_ERR_LLAVE_B,
		TOKEN_ERR_OPENCONJ_B,
		TOKEN_ERR_OPENREG_B,
		TOKEN_ERR_TOKEN_CHAR_B,
		TOKEN_ERR_CHAR_AMPER_B,
		CANT_TOKEN
	} TipoToken;

	class Token
	{
	private:
		TipoToken _tipoToken;
		string _lexema;
		unsigned long _globalPosic = 0;
		unsigned long _currentLine = 0;
		unsigned long _currentPosInLine = 0;
	public:
		TipoToken getTipoToken();
		void setTipoToken(TipoToken tipoToken);
		string getLexema();
		void setLexema(string);
		string toString();
		void setGlobalPosic(unsigned long globalPosic);
		unsigned long getGlobalPosic();
		bool isTokenKeyword();
		void setCurrentLine(unsigned long globalPosic);
		void setCurrentPosInLine(unsigned long globalPosic);
		unsigned long getCurrentLine();
		unsigned long getCurrentPosInLine();
	};

}
#endif