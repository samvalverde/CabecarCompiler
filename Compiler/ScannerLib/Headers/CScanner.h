#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "CBuffer.h"

namespace ScannerLib
{

	// Input categories for the transition table
	typedef enum {
		DIGITO_CERO,					//
		DIGITO_GENERAL,			// 1-9
		LETRA_GENERAL,     // a-z , A-Z , ·ÈÌÛ˙ , ‰ÎÔˆ¸ , Ò , ¡…Õ”⁄ , ƒÀœ÷‹ , — menos: X, W, A, B, C, D, E, F, ÷,     I, V, L, M, C, D
		LETRA_ROMANA,		// I,V,L,M  i,v,l,m???
		LETRA_X,
		LETRA_W,
		LETRA_HEX,
		LETRA_C,			//Puede ser Hex o puede ser Romano
		LETRA_D,			//Puede ser Hex o puede ser Romano
		LETRA_O_DIERESIS,
		CHAR_PUNTO,
		CHAR_2PUNTOS,
		CHAR_COMA,
		CHAR_COMILLA,
		CHAR_2COMILLA,
		CHAR_PIPE,
		CHAR_PARENTESIS_ABIERTO,
		CHAR_PARENTESIS_CERRADO,
		CHAR_PARENTESIS_CUADRADO_ABIERTO,
		CHAR_PARENTESIS_CUADRADO_CERRADO,
		CHAR_LLAVE_ABIERTA,
		CHAR_LLAVE_CERRADA,
		CHAR_AMPERSON,
		CHAR_ARROBA,
		CHAR_ASTERISCO,
		CHAR_MENOS,
		CHAR_OPERADOR_GENERAL,
		CHAR_MENOR,
		CHAR_MAYOR,
		CHAR_IGUAL,
		CHAR_DOLAR,
		CHAR_NUMERAL,
		ENTER,
		ESPACIO_SEPARADOR, // space, tab, newline
		CHAR_EOF,
		OTRO,      // Anything else
		CANT_CHAR_CATEGORY // Number of input categories
	} FamiliaChar;

	// Estados del AutÛmata
	typedef enum {
		START,
		S_1,
		S_2,
		S_3,
		S_4,
		S_5,
		S_6,
		S_7,
		S_8,
		S_9,
		S_10,
		S_11,
		S_12,
		S_13,
		S_14,
		S_15,
		S_16,
		S_17,
		S_18,
		S_19,
		S_20,
		S_21,
		S_22,
		S_23,
		S_24,
		S_25,
		S_26,
		S_27,
		S_28,
		S_29,
		F_TOKEN_INT,				//F1
		F_TOKEN_INT_B,				//F2
		ERR_INT_B,					//E1

		F_TOKEN_MENOS,				//F3
		F_TOKEN_MENOS_B,			//F4
		ERR_MENOS_B,				//E2

		F_TOKEN_ID,						//F5
		F_TOKEN_ID_B,					//F6
		ERR_TOKEN_ID_B,  				//E3
		F_TOKEN_ROMANO,					//F25
		F_TOKEN_WO_OPER,				//F24

		ERR_2PUNTO_B,					//E4
		F_TOKEN_TIPO_B,					//F7
		ERR_TIPO_B,						//E5
		F_TOKEN_INICIA_B,				//F8
		ERR_INICIA_B,					//E6


		F_TOK_CERRCONJ_B,				//F9

		ERR_CERRCONJ_B,					//E7

		ERR_LLAVE_B,					//E8

		F_TOKEN_OPENCONJ_B,				//F10
		ERR_OPENCONJ_B,					//E9


		F_TOKEN_OPENREG_B,				//F11
		ERR_OPENREG_B,					//E10

		F_TOKEN_CHAR,					//F12
		ERR_TOKEN_CHAR_B,				//E11

		F_TOKEN_STRING_B,				//F13
		ERR_TOKEN_STRING,				//E12

		F_TOKEN_CHAR_INDIV,				//F14

		F_TOKEN_COMP,					//LE, GE, NE	//F15, F17, F18
		F_TOKEN_COMP_B,					//L, G, E		//F16, F19

		F_TOKEN_CHAR_POS,				//F20  
		ERR_CHAR_AMPER_B,				//E13

		F_TOK_CERR_REG,					//F22
		F_TOKEN_MULT_B,					//F21

		//F_TOK_CERR_ROM,				//F26  eliminado porque ahora es toda una unidad
		ERR_CHAR_PIPE_B,				//E14
		ERR_ROMANO_B,					//E15
		F_TOKEN_EOF,
		ERROR,

		//			F_COMENTARIO,					//F23
		//			ERR_COMENTARIO,					//E???

		CANT_ESTADOS_AUTOMATA
	} EstadoAutomata;

	class CScanner {
	private:
		int _contLineas = 0;
		int _contChars = 0;
		int _contTokens = 0;
		int _contComentarios = 0;
		CBuffer * _buffer;
	public:
		Token demeToken();

		int inicializarScanner(string filename);
		void finalizarScanner();

		int getCantidadLineas();
		int getCantidadChars();
		int getCantidadTokens();
		int getCantidadComentarios();
	};
}

/*

	m·s tokens de las palabras reservadas

*/


#endif