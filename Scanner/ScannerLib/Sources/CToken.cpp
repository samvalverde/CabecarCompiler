#include <string>
#include "CToken.h"

using namespace std;

namespace ScannerLib
{

	TipoToken Token::getTipoToken() {
		return _tipoToken;
	}
	void Token::setTipoToken(TipoToken tipoToken) {
		_tipoToken = tipoToken;
	}

	void Token::setCurrentLine(unsigned long currentLine) {
		_currentLine = currentLine;
	}

	void Token::setCurrentPosInLine(unsigned long currentPosInLine) {
		_currentPosInLine = currentPosInLine;
	}

	unsigned long Token::getCurrentLine() {
		return _currentLine;
	}

	unsigned long Token::getCurrentPosInLine() {
		return _currentPosInLine;
	}

	string Token::getLexema() {
		return _lexema;
	}

	void Token::setLexema(string newValue) {
		_lexema = newValue;
	}

	void Token::setGlobalPosic(unsigned long globalPosic) {
		_globalPosic = globalPosic;
	}

	unsigned long Token::getGlobalPosic() {
		return _globalPosic;
	}

	bool Token::isTokenKeyword() {
		if (_tipoToken >= TOKEN_WOGAWG && _tipoToken <= TOKEN_NITSOSI) {
			return true;
		}
		else {
			return false;
		}
	}

	string Token::toString() {
		// Function to print token types as strings
		switch (this->getTipoToken()) {
		case TOKEN_INT: return "TOKEN_INT";
		case TOKEN_ID: return "TOKEN_ID";
		case TOKEN_CHAR: return "TOKEN_CHAR";
		case TOKEN_STRING: return "TOKEN_STRING";
		case TOKEN_TIPO: return "TOKEN_TIPO";
		case TOKEN_INICIALIZAR: return "TOKEN_INICIALIZAR";
		case TOKEN_ABRIR_CONJUNTO: return "TOKEN_ABRIR_CONJUNTO";
		case TOKEN_CERRAR_CONJUNTO: return "TOKEN_CERRAR_CONJUNTO";
		case TOKEN_ABRIR_REGISTRO: return "TOKEN_ABRIR_REGISTRO";
		case TOKEN_CERRAR_REGISTRO: return "TOKEN_CERRAR_REGISTRO";
		case TOKEN_PAR_REDONDO_ABRIR: return "TOKEN_PAR_REDONDO_ABRIR";
		case TOKEN_PAR_REDONDO_CERRAR: return "TOKEN_PAR_REDONDO_CERRAR";
		case TOKEN_PAR_CUADRADO_ABRIR: return "TOKEN_PAR_CUADRADO_ABRIR";
		case TOKEN_PAR_CUADRADO_CERRAR: return "TOKEN_PAR_CUADRADO_CERRAR";
		case TOKEN_ARROBA: return "TOKEN_ARROBA";
		case TOKEN_MAS: return "TOKEN_MAS";
		case TOKEN_MENOS: return "TOKEN_MENOS";
		case TOKEN_MULTIPLICACION: return "TOKEN_MULTIPLICACION";
		case TOKEN_DIVISION: return "TOKEN_DIVISION";
		case TOKEN_MODULO: return "TOKEN_MODULO";
		//case TOKEN_OPERADOR: return "TOKEN_OPERADOR";
		//case TOKEN_OPERADOR_ROMANO: return "TOKEN_OPERADOR_ROMANO";
		case TOKEN_MAS_ROMANO: return "TOKEN_MAS_ROMANO";
		case TOKEN_MENOS_ROMANO: return "TOKEN_MENOS_ROMANO";
		case TOKEN_MULTIPLICACION_ROMANO: return "TOKEN_MULTIPLICACION_ROMANO";
		case TOKEN_DIVISION_ROMANO: return "TOKEN_DIVISION_ROMANO";
		case TOKEN_MODULO_ROMANO: return "TOKEN_MODULO_ROMANO";
		case TOKEN_PUNTO: return "TOKEN_PUNTO";
		case TOKEN_COMA: return "TOKEN_COMA";
		case TOKEN_COMPARADOR: return "TOKEN_COMPARADOR";
		case TOKEN_CHAR_POS: return "TOKEN_CHAR_POS";
		case TOKEN_ABRIR_ROMANO: return "TOKEN_ABRIR_ROMANO";
		case TOKEN_CERRAR_ROMANO: return "TOKEN_CERRAR_ROMANO";
		case TOKEN_WOGAWG: return "TOKEN_WOGAWG";
		case TOKEN_KARA: return "TOKEN_KARA";
		case TOKEN_KLO: return "TOKEN_KLO";
		case TOKEN_KASKA: return "TOKEN_KASKA";
		case TOKEN_WAKIRI: return "TOKEN_WAKIRI";
		case TOKEN_PRA: return "TOKEN_PRA";
		case TOKEN_JARREE: return "TOKEN_JARREE";
		case TOKEN_NAKJEE: return "TOKEN_NAKJEE";
		case TOKEN_SHTAWA: return "TOKEN_SHTAWA";
		case TOKEN_EKLA: return "TOKEN_EKLA";
		case TOKEN_SHEJ: return "TOKEN_SHEJ";
		case TOKEN_YINAKULIWA: return "TOKEN_YINAKULIWA";
		case TOKEN_JARJA: return "TOKEN_JARJA";
		case TOKEN_KICHANA: return "TOKEN_KICHANA";
		case TOKEN_BIKO: return "TOKEN_BIKO";
		case TOKEN_BOK: return "TOKEN_BOK";
		case TOKEN_SIWA: return "TOKEN_SIWA";
		case TOKEN_KOYUWE: return "TOKEN_KOYUWE";
		case TOKEN_WAKA: return "TOKEN_WAKA";
		case TOKEN_DAKA: return "TOKEN_DAKA";
		case TOKEN_DAWA: return "TOKEN_DAWA";
		case TOKEN_TOLO: return "TOKEN_TOLO";
		case TOKEN_JELA: return "TOKEN_JELA";
		case TOKEN_JURU: return "TOKEN_JURU";
		case TOKEN_KARI: return "TOKEN_KARI";
		case TOKEN_IRA: return "TOKEN_IRA";
		case TOKEN_IRALE: return "TOKEN_IRALE";
		case TOKEN_KAI: return "TOKEN_KAI";
		case TOKEN_JEBA: return "TOKEN_JEBA";
		case TOKEN_BAWA: return "TOKEN_BAWA";
		case TOKEN_MATSII: return "TOKEN_MATSII";
		case TOKEN_TIA: return "TOKEN_TIA";
		case TOKEN_KJATIA: return "TOKEN_KJATIA";
		case TOKEN_KUA: return "TOKEN_KUA";
		case TOKEN_WOBOGO: return "TOKEN_WOBOGO";
		case TOKEN_SHENAWA: return "TOKEN_SHENAWA";
		case TOKEN_NAWOTKO: return "TOKEN_NAWOTKO";
		case TOKEN_PAKAKIREI: return "TOKEN_PAKAKIREI";
		case TOKEN_DODWA: return "TOKEN_DODWA";
		case TOKEN_JAREBO: return "TOKEN_JAREBO";
		case TOKEN_KA: return "TOKEN_KA";
		case TOKEN_TUINA: return "TOKEN_TUINA";
		case TOKEN_DALEIKA: return "TOKEN_DALEIKA";
		case TOKEN_SHA: return "TOKEN_SHA";
		case TOKEN_CHAGO: return "TOKEN_CHAGO";
		case TOKEN_JENA: return "TOKEN_JENA";
		case TOKEN_SHIRINA: return "TOKEN_SHIRINA";
		case TOKEN_MANEWA: return "TOKEN_MANEWA";
		case TOKEN_NUI: return "TOKEN_NUI";
		case TOKEN_TSU: return "TOKEN_TSU";
		case TOKEN_SHANI: return "TOKEN_SHANI";
		case TOKEN_KANA: return "TOKEN_KANA";
		case TOKEN_TUNA: return "TOKEN_TUNA";
		case TOKEN_KATSO: return "TOKEN_KATSO";
		case TOKEN_NAI: return "TOKEN_NAI";
		case TOKEN_TA: return "TOKEN_TA";
		case TOKEN_TULUWO: return "TOKEN_TULUWO";
		case TOKEN_TSERE: return "TOKEN_TSERE";
		case TOKEN_SHANA: return "TOKEN_SHANA";
		case TOKEN_KIANA: return "TOKEN_KIANA";
		case TOKEN_CHANI: return "TOKEN_CHANI";
		case TOKEN_WAYUA: return "TOKEN_WAYUA";
		case TOKEN_IAWA: return "TOKEN_IAWA";
		case TOKEN_TOLI: return "TOKEN_TOLI";
		case TOKEN_KUKLE: return "TOKEN_KUKLE";
		case TOKEN_AWESHTAWA: return "TOKEN_AWESHTAWA";
		case TOKEN_AWEEKLA: return "TOKEN_AWEEKLA";
		case TOKEN_AWESHEJ: return "TOKEN_AWESHEJ";
		case TOKEN_AWEYINAKULIWA: return "TOKEN_AWEYINAKULIWA";
		case TOKEN_AWEJARJA: return "TOKEN_AWEJARJA";
		case TOKEN_AWEWOMELE: return "TOKEN_AWEWOMELE";
		case TOKEN_BAKSHTAWA: return "TOKEN_BAKSHTAWA";
		case TOKEN_BAKEKLA: return "TOKEN_BAKEKLA";
		case TOKEN_BAKSHEJ: return "TOKEN_BAKSHEJ";
		case TOKEN_BAKYINAKULIWA: return "TOKEN_BAKYINAKULIWA";
		case TOKEN_BAKJARJA: return "TOKEN_BAKJARJA";
		case TOKEN_BAKWOMELE: return "TOKEN_BAKWOMELE";
		case TOKEN_WEIKA: return "TOKEN_WEIKA";
		case TOKEN_WOMELE: return "TOKEN_WOMELE";
		case TOKEN_KALWA: return "TOKEN_KALWA";
		case TOKEN_SAKA: return "TOKEN_SAKA";
		case TOKEN_TOKA: return "TOKEN_TOKA";
		case TOKEN_NITSOSI: return "TOKEN_NITSOSI";
		case TOKEN_EOF: return "TOKEN_EOF";
		case TOKEN_UNKNOWN: return "TOKEN_UNKNOWN";
		case TOKEN_ERR_INT_B: return "ERROR_POSIBLE_INT";
		case TOKEN_ERR_MENOS_B: return "ERROR_POSIBLE_MENOS";
		case TOKEN_ERR_TOKEN_STRING: return "ERROR_POSIBLE_STRING";
		case TOKEN_ERR_CHAR_PIPE_B: return "ERROR PIPE";
		case TOKEN_ERR_TOKEN_ID_B: return "ERROR_POSIBLE_ID";
		case TOKEN_ERR_2PUNTO_B: return "ERROR_2PUNTO";
		case TOKEN_ERR_TIPO_B: return "ERROR_POSIBLE_TIPO";
		case TOKEN_ERR_INICIA_B: return "ERROR_POSIBLE_INICIALIZACION";
		case TOKEN_ERR_CERRCONJ_B: return "ERROR_CERRAR_CONJUNTO";
		case TOKEN_ERR_LLAVE_B: return "ERROR_LLAVE";
		case TOKEN_ERR_OPENCONJ_B: return "ERROR_ABRIR_CONJUNTO";
		case TOKEN_ERR_OPENREG_B: return "ERROR_ABRIR_REGISTRO";
		case TOKEN_ERR_TOKEN_CHAR_B: return "ERROR_POSIBLE_CHAR";
		case TOKEN_ERR_CHAR_AMPER_B: return "ERROR AMPERSON";

		default: return "UNKNOWN";
		}
	}

}

