#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "CToken.h"
#include "CScanner.h"

namespace ScannerLib
{

	// Transition table: state transitions based on current state and input familia
	EstadoAutomata tablaTransicion[CANT_ESTADOS_AUTOMATA][CANT_CHAR_CATEGORY] =
	{
		// DIGITO			DIGITO				LETRA				LETRA				LETRA				LETRA				LETRA				LETRA					LETRA				LETRA					
		// DIGITO_CERO,		DIGITO_GENERAL,		LETRA_GENERAL,		LETRA_ROMANA		LETRA_X,			LETRA_W,			LETRA_HEX,			LETRA_C					LETRA_D				LETRA_O_DIERESIS,		CHAR_PUNTO,			CHAR_2PUNTOS,		CHAR_COMA,			CHAR_COMILLA,		CHAR_2COMILLA,		CHAR_PIPE,			CHAR_PARENTESIS_ABIERTO,	CHAR_PARENTESIS_CERRADO,	CHAR_PARENTESIS_CUADRADO_ABIERTO,	CHAR_PARENTESIS_CUADRADO_CERRADO,	CHAR_LLAVE_ABIERTA,		CHAR_LLAVE_CERRADA,		CHAR_AMPERSON,		CHAR_ARROBA,		CHAR_ASTERISCO,		CHAR_MENOS,			CHAR_OPERADOR_GENERAL,	CHAR_MENOR,			CHAR_MAYOR,			CHAR_IGUAL,			CHAR_DOLAR,			CHAR_NUMERAL,		ENTER,				ESPACIO_SEPARADOR,		CHAR_EOF,			OTRO
		{ S_2,				S_1,				S_5,				S_5,				S_5, 				S_6,				S_5,				S_5,					S_5,				S_5,					F_TOKEN_CHAR_INDIV,	S_10,				F_TOKEN_CHAR_INDIV,	S_17,				S_19,				ERR_CHAR_PIPE_B,	F_TOKEN_CHAR_INDIV,			F_TOKEN_CHAR_INDIV,			F_TOKEN_CHAR_INDIV,					F_TOKEN_CHAR_INDIV,					S_14,					ERROR,					S_23,				F_TOKEN_CHAR_INDIV,	S_24,				S_4,				F_TOKEN_CHAR_INDIV,		S_21,				S_22,				F_TOKEN_CHAR_INDIV,	S_25,				ERROR,				START,				START,					F_TOKEN_EOF,		ERROR			},//START,
		{ S_1,				S_1,				ERR_INT_B,			ERR_INT_B,			ERR_INT_B, 			ERR_INT_B,			ERR_INT_B,			ERR_INT_B,				ERR_INT_B,			ERR_INT_B,				F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		ERR_INT_B,			F_TOKEN_INT_B,				F_TOKEN_INT_B,				F_TOKEN_INT_B,						F_TOKEN_INT_B,						F_TOKEN_INT_B,			F_TOKEN_INT_B,			F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,			F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		ERR_INT_B,			F_TOKEN_INT_B,		F_TOKEN_INT_B,			F_TOKEN_INT_B,		ERR_INT_B		},//S_1,
		{ S_1,				S_1,				ERR_INT_B,			ERR_INT_B,			S_3,	 			ERR_INT_B,			ERR_INT_B,			ERR_INT_B,				ERR_INT_B,			ERR_INT_B,				F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		ERR_INT_B,			F_TOKEN_INT_B,				F_TOKEN_INT_B,				F_TOKEN_INT_B,						F_TOKEN_INT_B,						F_TOKEN_INT_B,			F_TOKEN_INT_B,			F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,			F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		ERR_INT_B,			F_TOKEN_INT_B,		F_TOKEN_INT_B,			F_TOKEN_INT_B,		ERR_INT_B		},//S_2,
		{ S_3,				S_3,				ERR_INT_B,			ERR_INT_B,			ERR_INT_B,			ERR_INT_B,			S_3,				S_3,					S_3,				ERR_INT_B,				F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		ERR_INT_B,			F_TOKEN_INT_B,				F_TOKEN_INT_B,				F_TOKEN_INT_B,						F_TOKEN_INT_B,						F_TOKEN_INT_B,			F_TOKEN_INT_B,			F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,			F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		F_TOKEN_INT_B,		ERR_INT_B,			F_TOKEN_INT_B,		F_TOKEN_INT_B,			F_TOKEN_INT_B,		ERR_INT_B		},//S_3,
		{ S_2,				S_1,				F_TOKEN_MENOS_B,	F_TOKEN_MENOS_B,	F_TOKEN_MENOS_B,	F_TOKEN_MENOS_B,	F_TOKEN_MENOS_B,	F_TOKEN_MENOS_B,		F_TOKEN_MENOS_B,	F_TOKEN_MENOS_B,		ERR_MENOS_B,		ERR_MENOS_B,		ERR_MENOS_B,		ERR_MENOS_B,		ERR_MENOS_B,		ERR_MENOS_B,		F_TOKEN_MENOS_B,			ERR_MENOS_B,				F_TOKEN_MENOS_B,					ERR_MENOS_B,						ERR_MENOS_B,			ERR_MENOS_B,			ERR_MENOS_B,		ERR_MENOS_B,		ERR_MENOS_B,		ERR_MENOS_B,		ERR_MENOS_B,			ERR_MENOS_B,		ERR_MENOS_B,		ERR_MENOS_B,		F_TOKEN_MENOS_B,	ERR_MENOS_B,		F_TOKEN_MENOS,		F_TOKEN_MENOS,			F_TOKEN_MENOS,		ERROR			},//S_4,
		{ S_5,				S_5,				S_5,				S_5,				S_5, 				S_5,				S_5,				S_5,					S_5,				S_5,					F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,				F_TOKEN_ID_B,				F_TOKEN_ID_B,						F_TOKEN_ID_B,						F_TOKEN_ID_B,			F_TOKEN_ID_B,			F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,			F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		ERR_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,			F_TOKEN_ID_B,		ERROR			},//S_5,
		{ S_5,				S_5,				S_5,				S_5,				S_5, 				S_5,				S_5,				S_5,					S_5,				S_7,					F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		S_8,				F_TOKEN_ID_B,				F_TOKEN_ID_B,				F_TOKEN_ID_B,						F_TOKEN_ID_B,						F_TOKEN_ID_B,			F_TOKEN_ID_B,			F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,			F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		ERR_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,			F_TOKEN_ID_B,		ERROR			},//S_6,
		{ S_5,				S_5,				S_5,				S_5,				S_5, 				S_5,				S_5,				S_5,					S_5,				S_5,					F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,				F_TOKEN_ID_B,				F_TOKEN_ID_B,						F_TOKEN_ID_B,						F_TOKEN_ID_B,			F_TOKEN_ID_B,			F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_WO_OPER,	F_TOKEN_WO_OPER,	F_TOKEN_WO_OPER,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,		ERR_TOKEN_ID_B,		F_TOKEN_ID_B,		F_TOKEN_ID_B,			F_TOKEN_ID_B,		ERROR			},//S_7,
		{ ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		S_8,				S_8, 				ERR_ROMANO_B,		ERR_ROMANO_B,		S_8,					S_8,				ERR_ROMANO_B,			ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		S_9,				ERR_ROMANO_B,				ERR_ROMANO_B,				ERR_ROMANO_B,						ERR_ROMANO_B,						ERR_ROMANO_B,			ERR_ROMANO_B,			ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,			ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		S_8,					ERR_ROMANO_B,		ERR_ROMANO_B	},//S_8,	
		{ ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B, 		F_TOKEN_ROMANO,		ERR_ROMANO_B,		ERR_ROMANO_B,			ERR_ROMANO_B,		ERR_ROMANO_B,			ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,				ERR_ROMANO_B,				ERR_ROMANO_B,						ERR_ROMANO_B,						ERR_ROMANO_B,			ERR_ROMANO_B,			ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,			ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,		ERR_ROMANO_B,			ERR_ROMANO_B,		ERR_ROMANO_B	},//S_9,	
		{ ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,			ERR_2PUNTO_B,		ERR_2PUNTO_B,			ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,				ERR_2PUNTO_B,				ERR_2PUNTO_B,						ERR_2PUNTO_B,						ERR_2PUNTO_B,			S_13,					ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,			ERR_2PUNTO_B,		S_11,				S_12,				ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,		ERR_2PUNTO_B,			ERR_2PUNTO_B,		ERROR			},//S_10,
		{ ERR_TIPO_B,		ERR_TIPO_B,			F_TOKEN_TIPO_B,		F_TOKEN_TIPO_B,		F_TOKEN_TIPO_B, 	F_TOKEN_TIPO_B,		F_TOKEN_TIPO_B,		F_TOKEN_TIPO_B,			F_TOKEN_TIPO_B,		F_TOKEN_TIPO_B,			ERR_TIPO_B,			ERR_TIPO_B,			ERR_TIPO_B,			ERR_TIPO_B,			ERR_TIPO_B,			ERR_TIPO_B,			ERR_TIPO_B,					ERR_TIPO_B,					ERR_TIPO_B,							ERR_TIPO_B,							ERR_TIPO_B,				ERR_TIPO_B,				ERR_TIPO_B,			ERR_TIPO_B,			ERR_TIPO_B,			ERR_TIPO_B,			ERR_TIPO_B,				ERR_TIPO_B,			ERR_TIPO_B,			ERR_TIPO_B,			F_TOKEN_TIPO_B,		ERR_TIPO_B,			F_TOKEN_TIPO_B,		F_TOKEN_TIPO_B,			F_TOKEN_TIPO_B,		ERR_TIPO_B		},//S_11,
		{ F_TOKEN_INICIA_B,	F_TOKEN_INICIA_B,	F_TOKEN_INICIA_B,	F_TOKEN_INICIA_B,	F_TOKEN_INICIA_B,	F_TOKEN_INICIA_B,	F_TOKEN_INICIA_B,	F_TOKEN_INICIA_B,		F_TOKEN_INICIA_B,	F_TOKEN_INICIA_B,		F_TOKEN_INICIA_B,	ERR_INICIA_B,		F_TOKEN_INICIA_B,	F_TOKEN_INICIA_B,	F_TOKEN_INICIA_B,	ERR_INICIA_B,		F_TOKEN_INICIA_B,			F_TOKEN_INICIA_B,			F_TOKEN_INICIA_B,					ERR_INICIA_B,						F_TOKEN_INICIA_B,		ERR_INICIA_B,			ERR_INICIA_B,		ERR_INICIA_B,		ERR_INICIA_B,		F_TOKEN_INICIA_B,	ERR_INICIA_B,			ERR_INICIA_B,		ERR_INICIA_B,		ERR_INICIA_B,		F_TOKEN_INICIA_B,	ERR_INICIA_B,		F_TOKEN_INICIA_B,	F_TOKEN_INICIA_B,		F_TOKEN_INICIA_B,	ERROR			},//S_12,
		{ F_TOK_CERRCONJ_B,	F_TOK_CERRCONJ_B,	F_TOK_CERRCONJ_B,	F_TOK_CERRCONJ_B,	F_TOK_CERRCONJ_B,	F_TOK_CERRCONJ_B,	F_TOK_CERRCONJ_B,	F_TOK_CERRCONJ_B,		F_TOK_CERRCONJ_B,	F_TOK_CERRCONJ_B,		F_TOK_CERRCONJ_B,	F_TOK_CERRCONJ_B,	F_TOK_CERRCONJ_B,	ERR_CERRCONJ_B,		ERR_CERRCONJ_B,		ERR_CERRCONJ_B,		F_TOK_CERRCONJ_B,			F_TOK_CERRCONJ_B,			F_TOK_CERRCONJ_B,					F_TOK_CERRCONJ_B,					F_TOK_CERRCONJ_B,		F_TOK_CERRCONJ_B,		ERR_CERRCONJ_B,		ERR_CERRCONJ_B,		F_TOK_CERRCONJ_B,	ERR_CERRCONJ_B,		ERR_CERRCONJ_B,			ERR_CERRCONJ_B,		ERR_CERRCONJ_B,		ERR_CERRCONJ_B,		F_TOK_CERRCONJ_B,	ERR_CERRCONJ_B,		F_TOK_CERRCONJ_B,	F_TOK_CERRCONJ_B,		F_TOK_CERRCONJ_B,	ERR_CERRCONJ_B	},//S_13,
		{ ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B, 		ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B,			ERR_LLAVE_B,		ERR_LLAVE_B,			ERR_LLAVE_B,		S_15,				ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B,				ERR_LLAVE_B,				ERR_LLAVE_B,						ERR_LLAVE_B,						ERR_LLAVE_B,			ERR_LLAVE_B,			ERR_LLAVE_B,		ERR_LLAVE_B,		S_16,				ERR_LLAVE_B,		ERR_LLAVE_B,			ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B,		ERR_LLAVE_B,			ERR_LLAVE_B,		ERR_LLAVE_B		},//S_14,
		{ ERR_OPENCONJ_B,	ERR_OPENCONJ_B,		ERR_OPENCONJ_B,		ERR_OPENCONJ_B,		ERR_OPENCONJ_B, 	ERR_OPENCONJ_B,		ERR_OPENCONJ_B,		ERR_OPENCONJ_B,			ERR_OPENCONJ_B,		ERR_OPENCONJ_B,			F_TOKEN_OPENCONJ_B,	F_TOKEN_OPENCONJ_B,	F_TOKEN_OPENCONJ_B,	ERR_OPENCONJ_B,		ERR_OPENCONJ_B,		F_TOKEN_OPENCONJ_B,	F_TOKEN_OPENCONJ_B,			F_TOKEN_OPENCONJ_B,			ERR_OPENCONJ_B,						F_TOKEN_OPENCONJ_B,					ERR_OPENCONJ_B,			F_TOKEN_OPENCONJ_B,		F_TOKEN_OPENCONJ_B,	F_TOKEN_OPENCONJ_B,	F_TOKEN_OPENCONJ_B,	ERR_OPENCONJ_B,		ERR_OPENCONJ_B,			F_TOKEN_OPENCONJ_B,	F_TOKEN_OPENCONJ_B,	F_TOKEN_OPENCONJ_B,	F_TOKEN_OPENCONJ_B,	ERR_OPENCONJ_B,		F_TOKEN_OPENCONJ_B,	F_TOKEN_OPENCONJ_B,		F_TOKEN_OPENCONJ_B,	ERR_OPENCONJ_B	},//S_15,
		{ F_TOKEN_OPENREG_B,F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,		F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,		F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,			F_TOKEN_OPENREG_B,			F_TOKEN_OPENREG_B,					F_TOKEN_OPENREG_B,					F_TOKEN_OPENREG_B,		F_TOKEN_OPENREG_B,		F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,		F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B,		F_TOKEN_OPENREG_B,	F_TOKEN_OPENREG_B},//S_16,
		{ S_18,				S_18,				S_18,				S_18,				S_18, 				S_18,				S_18,				S_18,					S_18,				S_18,					S_18,				S_18,				S_18,				ERR_TOKEN_CHAR_B,	S_18,				S_18,				S_18,						S_18,						S_18,								S_18,								S_18,					S_18,					S_18,				S_18,				S_18,				S_18,				S_18,					S_18,				S_18,				S_18,				S_18,				S_18,				ERR_TOKEN_CHAR_B,	S_18,					ERR_TOKEN_CHAR_B,	S_18			},//S_17,
		{ ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B, 	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,		ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,		ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	F_TOKEN_CHAR,		ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,			ERR_TOKEN_CHAR_B,			ERR_TOKEN_CHAR_B,					ERR_TOKEN_CHAR_B,					ERR_TOKEN_CHAR_B,		ERR_TOKEN_CHAR_B,		ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,		ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B,		ERR_TOKEN_CHAR_B,	ERR_TOKEN_CHAR_B},//S_18,
		{ S_19,				S_19,				S_19,				S_19,				S_19, 				S_19,				S_19,				S_19,					S_19,				S_19,					S_19,				S_19,				S_19,				S_19,				S_20,				S_19,				S_19,						S_19,						S_19,								S_19,								S_19,					S_19,					S_19,				S_19,				S_19,				S_19,				S_19,					S_19,				S_19,				S_19,				S_19,				S_19,				ERR_TOKEN_STRING,	S_19,					ERR_TOKEN_STRING,	S_19			},//S_19,
		{ F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B, 	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,		F_TOKEN_STRING_B,	F_TOKEN_STRING_B,		F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	S_19,				F_TOKEN_STRING_B,	F_TOKEN_STRING_B,			F_TOKEN_STRING_B,			F_TOKEN_STRING_B,					F_TOKEN_STRING_B,					F_TOKEN_STRING_B,		F_TOKEN_STRING_B,		F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,		F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,	F_TOKEN_STRING_B,		F_TOKEN_STRING_B,	F_TOKEN_STRING_B},//S_20,
		{ F_TOKEN_COMP_B,	F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B, 	F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,			F_TOKEN_COMP_B,		F_TOKEN_COMP_B,			F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,				F_TOKEN_COMP_B,				F_TOKEN_COMP_B,						F_TOKEN_COMP_B,						F_TOKEN_COMP_B,			F_TOKEN_COMP_B,			F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,			F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,			F_TOKEN_COMP_B,		F_TOKEN_COMP_B	},//S_21,
		{ F_TOKEN_COMP_B,	F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B, 	F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,			F_TOKEN_COMP_B,		F_TOKEN_COMP_B,			F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,				F_TOKEN_COMP_B,				F_TOKEN_COMP_B,						F_TOKEN_COMP_B,						F_TOKEN_COMP_B,			F_TOKEN_COMP_B,			F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,			F_TOKEN_COMP,		F_TOKEN_COMP_B,		F_TOKEN_COMP,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,		F_TOKEN_COMP_B,			F_TOKEN_COMP_B,		F_TOKEN_COMP_B	},//S_22,
		{ ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B, 	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,		ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,		ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,			ERR_CHAR_AMPER_B,			ERR_CHAR_AMPER_B,					ERR_CHAR_AMPER_B,					ERR_CHAR_AMPER_B,		ERR_CHAR_AMPER_B,		ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,		ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,	F_TOKEN_CHAR_POS,	ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B,		ERR_CHAR_AMPER_B,	ERR_CHAR_AMPER_B},//S_23,
		{ F_TOKEN_MULT_B,	F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B, 	F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,			F_TOKEN_MULT_B,		F_TOKEN_MULT_B,			F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,				F_TOKEN_MULT_B,				F_TOKEN_MULT_B,						F_TOKEN_MULT_B,						F_TOKEN_MULT_B,			F_TOK_CERR_REG,			F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,			F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,		F_TOKEN_MULT_B,			F_TOKEN_MULT_B,		F_TOKEN_MULT_B	},//S_24,
		{ S_28,				S_28,				S_28,				S_28,				S_28, 				S_28,				S_28,				S_28,					S_28,				S_28,					S_28,				S_28,				S_28,				S_28,				S_28,				S_28,				S_28,						S_28,						S_28,								S_28,								S_28,					S_28,					S_28,				S_28,				S_26,				S_28,				S_28,					S_28,				S_28,				S_28,				S_28,				S_28,				START,				S_28,					S_28,				S_28			},//S_25,
		{ S_26,				S_26,				S_26,				S_26,				S_26, 				S_26,				S_26,				S_26,					S_26,				S_26,					S_26,				S_26,				S_26,				S_26,				S_26,				S_26,				S_26,						S_26,						S_26,								S_26,								S_26,					S_26,					S_26,				S_26,				S_27,				S_26,				S_26,					S_26,				S_26,				S_26,				S_26,				S_26,				S_26,				S_26,					S_26,				S_26			},//S_26,
		{ S_26,				S_26,				S_26,				S_26,				S_26, 				S_26,				S_26,				S_26,					S_26,				S_26,					S_26,				S_26,				S_26,				S_26,				S_26,				S_26,				S_26,						S_26,						S_26,								S_26,								S_26,					S_26,					S_26,				S_26,				S_27,				S_26,				S_26,					S_26,				S_26,				S_26,				START,				S_26,				S_26,				S_26,					S_26,				S_26			},//S_27,
		{ S_28,				S_28,				S_28,				S_28,				S_28, 				S_28,				S_28,				S_28,					S_28,				S_28,					S_28,				S_28,				S_28,				S_28,				S_28,				S_28,				S_28,						S_28,						S_28,								S_28,								S_28,					S_28,					S_28,				S_28,				S_28,				S_28,				S_28,					S_28,				S_28,				S_28,				S_28,				S_28,				START,				S_28,					S_28,				S_28			},//S_28,
		{ ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B, 	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,		ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,		ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,			ERR_CHAR_PIPE_B,			ERR_CHAR_PIPE_B,					ERR_CHAR_PIPE_B,					ERR_CHAR_PIPE_B,		ERR_CHAR_PIPE_B,		ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,		ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B,		ERR_CHAR_PIPE_B,	ERR_CHAR_PIPE_B	},//S_29,


		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_INT,				//F1
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_INT_B,				//F2
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_INT_B,					//E1
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_MENOS,				//F3
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_MENOS_B,			//F4
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_MENOS_B,				//E2
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_ID,						//F5
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_ID_B,					//F6
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_TOKEN_ID_B,  				//E3
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_ROMANO,					//F25    //antes F_TOK_OPEN_ROM
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_WO_OPER,				//F24
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_2PUNTO_B,					//E4
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_TIPO_B,					//F7
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_TIPO_B,						//E5
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_INICIA_B,				//F8
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_INICIA_B,					//E6
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOK_CERRCONJ_B,			//F9
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_CERRCONJ_B,				//E7
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_LLAVE_B,					//E8
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_OPENCONJ_B,				//F10
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_OPENCONJ_B,					//E9
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_OPENREG_B,				//F11
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_OPENREG_B,					//E10
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_CHAR,					//F12
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_TOKEN_CHAR_B,				//E11
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_STRING_B,				//F13
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_TOKEN_STRING,				//E12
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_CHAR_INDIV,				//F14
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_COMP,					//LE, GE, NE	//F15, F17, F18
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_COMP_B,					//L, G, E		//F16, F19
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_CHAR_POS,				//F20
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_CHAR_AMPER_B,				//E13
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOK_CERR_REG,					//F22
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_MULT_B,					//F21  equivocado ERR_ASTERISCO_B
//		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOK_CERR_ROM,					//F26
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_CHAR_PIPE_B,
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//ERR_ROMANO_B,					//E15
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			},//F_TOKEN_EOF,	
		{ ERROR,			ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,						ERROR,						ERROR,								ERROR,								ERROR,					ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,				ERROR,					ERROR,				ERROR			} //ERROR
		//			F_COMENTARIO,					//F23
		//			ERR_COMENTARIO,					//E???
	};


	int CScanner::inicializarScanner(string filename) {
		_buffer = new CBuffer(SIZE_BUFFER);
		int result = _buffer->openFile(filename);
		if (result == 0) {
			_buffer->cargarBuffer();
		}
		return result;
	}

	void CScanner::finalizarScanner() {
		_buffer->finish();
	}


	bool isalphaCabecar(char c) {
		bool result = false;
		if (c != CHAR_FIN && strchr("·ÈÌÛ˙‰ÎÔˆ¸Ò¡…Õ”⁄ƒÀœ÷‹—", c)) {
			result = true;
		}
		return result;
	}

	// Function to classify input character into categories for the transition table
	FamiliaChar getFamiliaChar(char c) {
		unsigned char uc = (unsigned char)c;

		if (c == CHAR_FIN) {
			return CHAR_EOF;
		}
		else if (isdigit(uc)) {
			if (uc == '0') {
				return DIGITO_CERO;		//0
			}
			else {
				return DIGITO_GENERAL;	// 1-9
			}
		}
		else if (isalpha(uc) || isalphaCabecar(uc)) {
			// a-z , A-Z , ·ÈÌÛ˙ , ‰ÎÔˆ¸ , Ò , ¡…Õ”⁄ , ƒÀœ÷‹ ,  —
			if (uc == 'X' || uc == 'x') {
				return LETRA_X;
			}
			else if (uc == 'W' || uc == 'w') {
				return LETRA_W;
			}
			else if (uc == 'I' || uc == 'i' ||
				uc == 'V' || uc == 'v' ||
				uc == 'L' || uc == 'l' ||
				uc == 'M' || uc == 'm') {
				return LETRA_ROMANA;
			}
			else if (uc == 'C' || uc == 'c') {
				return LETRA_C;
			}
			else if (uc == 'D' || uc == 'd') {
				return LETRA_D;
			}
			else if (uc == 'A' || uc == 'a' ||
				uc == 'B' || uc == 'b' ||
				uc == 'E' || uc == 'e' ||
				uc == 'F' || uc == 'f') {
				return LETRA_HEX;
			}
			else if (uc == ((unsigned char)'÷') || uc == ((unsigned char)'ˆ')) {
				return LETRA_O_DIERESIS;
			}
			else {
				return LETRA_GENERAL;
			}
		}
		else if (c == '.') {
			return CHAR_PUNTO;
		}
		else if (c == ':') {
			return CHAR_2PUNTOS;
		}
		else if (c == ',') {
			return CHAR_COMA;
		}
		else if (c == '\'') {
			return CHAR_COMILLA;
		}
		else if (c == '"') {
			return CHAR_2COMILLA;
		}
		else if (c == '|') {
			return CHAR_PIPE;
		}
		else if (c == '(') {
			return CHAR_PARENTESIS_ABIERTO;
		}
		else if (c == ')') {
			return CHAR_PARENTESIS_CERRADO;
		}
		else if (c == '[') {
			return CHAR_PARENTESIS_CUADRADO_ABIERTO;
		}
		else if (c == ']') {
			return CHAR_PARENTESIS_CUADRADO_CERRADO;
		}
		else if (c == '{') {
			return CHAR_LLAVE_ABIERTA;
		}
		else if (c == '}') {
			return CHAR_LLAVE_CERRADA;
		}
		else if (c == '&') {
			return CHAR_AMPERSON;
		}
		else if (c == '@') {
			return CHAR_ARROBA;
		}
		else if (c == '*') {
			return CHAR_ASTERISCO;
		}
		else if (c == '-') {
			return CHAR_MENOS;
		}
		else if (strchr("+/%", c)) {
			return CHAR_OPERADOR_GENERAL;
		}
		else if (c == '<') {
			return CHAR_MENOR;
		}
		else if (c == '>') {
			return CHAR_MAYOR;
		}
		else if (c == '=') {
			return CHAR_IGUAL;
		}
		else if (c == '$') {
			return CHAR_DOLAR;
		}
		else if (c == '#') {
			return CHAR_NUMERAL;
		}
		else if (c == CHAR_CR || c == CHAR_LF) {
			return ENTER;
		}
		else if (isspace(uc)) {
			return ESPACIO_SEPARADOR; // space, tab, newline
		}
		else {
			return OTRO;      // Anything else
		}
	}

	bool isInternalState(int state) {
		if (state >= START && state <= S_29) {
			return true;
		}
		else {
			return false;
		}
	}

	bool isErrorStateNoBackChar(int state) {
		if (state == ERR_TOKEN_STRING ||
			state == ERR_CHAR_PIPE_B ||
			state == ERROR)
		{
			return true;
		}
		else {
			return false;
		}
	}

	bool isErrorStateWithBackChar(int state) {
		if (state == ERR_INT_B ||
			state == ERR_MENOS_B ||
			state == ERR_TOKEN_ID_B ||
			state == ERR_2PUNTO_B ||
			state == ERR_TIPO_B ||
			state == ERR_INICIA_B ||
			state == ERR_CERRCONJ_B ||
			state == ERR_LLAVE_B ||
			state == ERR_OPENCONJ_B ||
			state == ERR_OPENREG_B ||
			state == ERR_TOKEN_CHAR_B ||
			state == ERR_CHAR_AMPER_B ||
			state == ERR_ROMANO_B)
		{
			return true;
		}
		else {
			return false;
		}
	}

	bool isFinalStateWithBackChar(int state) {
		if (state == F_TOKEN_INT_B ||
			state == F_TOKEN_MENOS_B ||
			state == F_TOKEN_ID_B ||
			state == F_TOKEN_TIPO_B ||
			state == F_TOKEN_INICIA_B ||
			state == F_TOK_CERRCONJ_B ||
			state == F_TOKEN_OPENCONJ_B ||
			state == F_TOKEN_OPENREG_B ||
			state == F_TOKEN_STRING_B ||
			state == F_TOKEN_COMP_B ||
			state == F_TOKEN_MULT_B
			)
		{
			return true;
		}
		else {
			return false;
		}
	}


	bool isFinalStateNoBackChar(int state) {
		if (state == F_TOKEN_INT ||
			state == F_TOKEN_MENOS ||
			state == F_TOKEN_ID ||
			state == F_TOKEN_ROMANO ||
			state == F_TOKEN_WO_OPER ||
			state == F_TOKEN_CHAR ||
			state == F_TOKEN_CHAR_INDIV ||
			state == F_TOKEN_COMP ||
			state == F_TOKEN_CHAR_POS ||
			state == F_TOK_CERR_REG ||
			state == F_TOKEN_EOF
			)
		{
			return true;
		}
		else {
			return false;
		}
	}


	char * upperCabecar(const char * palabra) {
		size_t lenPal = strlen(palabra);
		char * uPal = new char[lenPal + 1];
		strcpy_s(uPal, lenPal + 1, palabra);
		for (unsigned int i = 0; i < lenPal; i++) {
			if (uPal[i] != CHAR_FIN && strchr("·ÈÌÛ˙‰ÎÔˆ¸Ò¡…Õ”⁄ƒÀœ÷‹—", uPal[i])) {
				if (uPal[i] == '·') { uPal[i] = '¡'; }
				else if (uPal[i] == 'È') { uPal[i] = '…'; }
				else if (uPal[i] == 'Ì') { uPal[i] = 'Õ'; }
				else if (uPal[i] == 'Û') { uPal[i] = '”'; }
				else if (uPal[i] == '˙') { uPal[i] = '⁄'; }
				else if (uPal[i] == '‰') { uPal[i] = 'ƒ'; }
				else if (uPal[i] == 'Î') { uPal[i] = 'À'; }
				else if (uPal[i] == 'Ô') { uPal[i] = 'œ'; }
				else if (uPal[i] == 'ˆ') { uPal[i] = '÷'; }
				else if (uPal[i] == '¸') { uPal[i] = '‹'; }
				else if (uPal[i] == 'Ò') { uPal[i] = '—'; }
			}
			else {
				uPal[i] = toupper(uPal[i]);
			}
		}
		return uPal;
	}

	// FunciÛn para reconocer una palabra clave
	bool isKeyword(const char * palabra) {
		char * uPalabra = upperCabecar(palabra);

		if (
			(strcmp(uPalabra, upperCabecar("wˆgawg")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Kar·")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Klo")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Kaska")) == 0) ||
			(strcmp(uPalabra, upperCabecar("W‰kiri")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Pr·")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Jarree")) == 0) ||
			(strcmp(uPalabra, upperCabecar("—·kjÎÎ")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Sht·wa")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Èkla")) == 0) ||
			(strcmp(uPalabra, upperCabecar("ShÈj")) == 0) ||
			(strcmp(uPalabra, upperCabecar("YÌnaKuÅlÌÅwa")) == 0) ||
			(strcmp(uPalabra, upperCabecar("J·rj·")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Kich·na")) == 0) ||
			(strcmp(uPalabra, upperCabecar("bikˆ")) == 0) ||
			(strcmp(uPalabra, upperCabecar("BÛk")) == 0) ||
			(strcmp(uPalabra, upperCabecar("SÌwa")) == 0) ||
			(strcmp(uPalabra, upperCabecar("KÛyuwÈ")) == 0) ||
			(strcmp(uPalabra, upperCabecar("w·ka")) == 0) ||
			(strcmp(uPalabra, upperCabecar("d‰k·")) == 0) ||
			(strcmp(uPalabra, upperCabecar("d‰w·")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Tˆlˆ")) == 0) ||
			(strcmp(uPalabra, upperCabecar("JÈl·")) == 0) ||
			(strcmp(uPalabra, upperCabecar("J˙ru")) == 0) ||
			(strcmp(uPalabra, upperCabecar("K·ri")) == 0) ||
			(strcmp(uPalabra, upperCabecar("ir‰")) == 0) ||
			(strcmp(uPalabra, upperCabecar("ir‰lÈ")) == 0) ||
			(strcmp(uPalabra, upperCabecar("k·i")) == 0) ||
			(strcmp(uPalabra, upperCabecar("jÈb‰")) == 0) ||
			(strcmp(uPalabra, upperCabecar("bawa")) == 0) ||
			(strcmp(uPalabra, upperCabecar("matsÌÌ")) == 0) ||
			(strcmp(uPalabra, upperCabecar("ti‰")) == 0) ||
			(strcmp(uPalabra, upperCabecar("kj‰")) == 0) ||  //"kj‰ti‰"
			(strcmp(uPalabra, upperCabecar("kua")) == 0) ||
			(strcmp(uPalabra, upperCabecar("wÛbogo")) == 0) ||
			(strcmp(uPalabra, upperCabecar("shÈnawa")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Òawˆtkˆ")) == 0) ||
			(strcmp(uPalabra, upperCabecar("pakakirei")) == 0) ||
			(strcmp(uPalabra, upperCabecar("dodwa")) == 0) ||
			(strcmp(uPalabra, upperCabecar("j·rebo")) == 0) ||
			(strcmp(uPalabra, upperCabecar("k·")) == 0) ||
			(strcmp(uPalabra, upperCabecar("tuÌna")) == 0) ||
			(strcmp(uPalabra, upperCabecar("daleika")) == 0) ||
			(strcmp(uPalabra, upperCabecar("sh‰")) == 0) ||
			(strcmp(uPalabra, upperCabecar("chagˆ")) == 0) ||
			(strcmp(uPalabra, upperCabecar("jÈn·")) == 0) ||
			(strcmp(uPalabra, upperCabecar("shirÌna")) == 0) ||
			(strcmp(uPalabra, upperCabecar("manÈwa")) == 0) ||
			(strcmp(uPalabra, upperCabecar("nuÌ")) == 0) ||
			(strcmp(uPalabra, upperCabecar("ts˙")) == 0) ||
			(strcmp(uPalabra, upperCabecar("sh‰ni")) == 0) ||
			(strcmp(uPalabra, upperCabecar("kana")) == 0) ||
			(strcmp(uPalabra, upperCabecar("tuna")) == 0) ||
			(strcmp(uPalabra, upperCabecar("katso")) == 0) ||
			(strcmp(uPalabra, upperCabecar("nai")) == 0) ||
			(strcmp(uPalabra, upperCabecar("ta")) == 0) ||
			(strcmp(uPalabra, upperCabecar("tuluwo")) == 0) ||
			(strcmp(uPalabra, upperCabecar("TsÎrÈ")) == 0) ||
			(strcmp(uPalabra, upperCabecar("sh·na")) == 0) ||
			(strcmp(uPalabra, upperCabecar("kiana")) == 0) ||
			(strcmp(uPalabra, upperCabecar("chani")) == 0) ||
			(strcmp(uPalabra, upperCabecar("wayu‰")) == 0) ||
			(strcmp(uPalabra, upperCabecar("i·wa")) == 0) ||
			(strcmp(uPalabra, upperCabecar("toli")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Bikˆ")) == 0) ||
			(strcmp(uPalabra, upperCabecar("kuklÎ")) == 0) ||
			//todos los ‰wÈTipo y B·kTipo no ser·n palabras reservadas para el lÈxico, porque sino hay que meter gram·tica para todos ellos.  Se opta porque se retornen como TOKEN_ID dado que van a ser el nombre de una funciÛn, y luego en el sem·ntico o generaciÛn de cÛdigo que haga lo que corresponda.
			//(strcmp(uPalabra, upperCabecar("‰wÈSht·wa")) == 0) ||
			//(strcmp(uPalabra, upperCabecar("‰wÈÈkla")) == 0) ||
			//(strcmp(uPalabra, upperCabecar("‰wÈShÈj")) == 0) ||
			//(strcmp(uPalabra, upperCabecar("‰wÈYÌnaKuÅlÌÅwa")) == 0) ||
			//(strcmp(uPalabra, upperCabecar("‰wÈJ·rj·")) == 0) ||
			//(strcmp(uPalabra, upperCabecar("‰wÈwˆmÎle")) == 0) ||
			//(strcmp(uPalabra, upperCabecar("B·kSht·wa")) == 0) ||
			//(strcmp(uPalabra, upperCabecar("B·kÈkla")) == 0) ||
			//(strcmp(uPalabra, upperCabecar("B·kShÈj")) == 0) ||
			//(strcmp(uPalabra, upperCabecar("B·kYÌnaKuÅlÌÅwa")) == 0) ||
			//(strcmp(uPalabra, upperCabecar("B·kJ·rj·")) == 0) ||
			//(strcmp(uPalabra, upperCabecar("B·kwˆmÎle")) == 0) ||
			(strcmp(uPalabra, upperCabecar("wÎik‰")) == 0) ||
			(strcmp(uPalabra, upperCabecar("wˆmÎle")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Kalw·")) == 0) ||
			(strcmp(uPalabra, upperCabecar("Sak·")) == 0) ||
			(strcmp(uPalabra, upperCabecar("tˆka")) == 0) ||
			(strcmp(uPalabra, upperCabecar("NitsÛsi")) == 0) ||
			(strcmp(uPalabra, upperCabecar("nuÌ")) == 0))
		{
			return true;
		}
		else {
			return false;
		}
	}

	TipoToken  keywordToToken(const char * pal) {
		char * palabra = upperCabecar(pal);
		if (strcmp(palabra, upperCabecar("wˆgawg")) == 0) {
			return TOKEN_WOGAWG;
		}
		else if (strcmp(palabra, upperCabecar("Kar·")) == 0) {
			return TOKEN_KARA;
		}
		else if (strcmp(palabra, upperCabecar("Klo")) == 0) {
			return TOKEN_KLO;
		}
		else if (strcmp(palabra, upperCabecar("Kaska")) == 0) {
			return TOKEN_KASKA;
		}
		else if (strcmp(palabra, upperCabecar("W‰kiri")) == 0) {
			return TOKEN_WAKIRI;
		}
		else if (strcmp(palabra, upperCabecar("Pr·")) == 0) {
			return TOKEN_PRA;
		}
		else if (strcmp(palabra, upperCabecar("Jarree")) == 0) {
			return TOKEN_JARREE;
		}
		else if (strcmp(palabra, upperCabecar("—·kjÎÎ")) == 0) {
			return TOKEN_NAKJEE;
		}
		else if (strcmp(palabra, upperCabecar("Sht·wa")) == 0) {
			return TOKEN_SHTAWA;
		}
		else if (strcmp(palabra, upperCabecar("Èkla")) == 0) {
			return TOKEN_EKLA;
		}
		else if (strcmp(palabra, upperCabecar("ShÈj")) == 0) {
			return TOKEN_SHEJ;
		}
		else if (strcmp(palabra, upperCabecar("YÌnaKuÅlÌÅwa")) == 0) {
			return TOKEN_YINAKULIWA;
		}
		else if (strcmp(palabra, upperCabecar("J·rj·")) == 0) {
			return TOKEN_JARJA;
		}
		else if (strcmp(palabra, upperCabecar("Kich·na")) == 0) {
			return TOKEN_KICHANA;
		}
		else if (strcmp(palabra, upperCabecar("bikˆ")) == 0) {
			return TOKEN_BIKO;
		}
		else if (strcmp(palabra, upperCabecar("BÛk")) == 0) {
			return TOKEN_BOK;
		}
		else if (strcmp(palabra, upperCabecar("SÌwa")) == 0) {
			return TOKEN_SIWA;
		}
		else if (strcmp(palabra, upperCabecar("KÛyuwÈ")) == 0) {
			return TOKEN_KOYUWE;
		}
		else if (strcmp(palabra, upperCabecar("w·ka")) == 0) {
			return TOKEN_WAKA;
		}
		else if (strcmp(palabra, upperCabecar("d‰k·")) == 0) {
			return TOKEN_DAKA;
		}
		else if (strcmp(palabra, upperCabecar("d‰w·")) == 0) {
			return TOKEN_DAWA;
		}
		else if (strcmp(palabra, upperCabecar("Tˆlˆ")) == 0) {
			return TOKEN_TOLO;
		}
		else if (strcmp(palabra, upperCabecar("JÈl·")) == 0) {
			return TOKEN_JELA;
		}
		else if (strcmp(palabra, upperCabecar("J˙ru")) == 0) {
			return TOKEN_JURU;
		}
		else if (strcmp(palabra, upperCabecar("K·ri")) == 0) {
			return TOKEN_KARI;
		}
		else if (strcmp(palabra, upperCabecar("ir‰")) == 0) {
			return TOKEN_IRA;
		}
		else if (strcmp(palabra, upperCabecar("ir‰lÈ")) == 0) {
			return TOKEN_IRALE;
		}
		else if (strcmp(palabra, upperCabecar("k·i")) == 0) {
			return TOKEN_KAI;
		}
		else if (strcmp(palabra, upperCabecar("jÈb‰")) == 0) {
			return TOKEN_JEBA;
		}
		else if (strcmp(palabra, upperCabecar("bawa")) == 0) {
			return TOKEN_BAWA;
		}
		else if (strcmp(palabra, upperCabecar("matsÌÌ")) == 0) {
			return TOKEN_MATSII;
		}
		else if (strcmp(palabra, upperCabecar("ti‰")) == 0) {
			return TOKEN_TIA;
		}
		else if (strcmp(palabra, upperCabecar("kj‰")) == 0) { //"kj‰ti‰"
			return TOKEN_KJA;
		}
		else if (strcmp(palabra, upperCabecar("kua")) == 0) {
			return TOKEN_KUA;
		}
		else if (strcmp(palabra, upperCabecar("wÛbogo")) == 0) {
			return TOKEN_WOBOGO;
		}
		else if (strcmp(palabra, upperCabecar("shÈnawa")) == 0) {
			return TOKEN_SHENAWA;
		}
		else if (strcmp(palabra, upperCabecar("Òawˆtkˆ")) == 0) {
			return TOKEN_NAWOTKO;
		}
		else if (strcmp(palabra, upperCabecar("pakakirei")) == 0) {
			return TOKEN_PAKAKIREI;
		}
		else if (strcmp(palabra, upperCabecar("dodwa")) == 0) {
			return TOKEN_DODWA;
		}
		else if (strcmp(palabra, upperCabecar("j·rebo")) == 0) {
			return TOKEN_JAREBO;
		}
		else if (strcmp(palabra, upperCabecar("k·")) == 0) {
			return TOKEN_KA;
		}
		else if (strcmp(palabra, upperCabecar("tuÌna")) == 0) {
			return TOKEN_TUINA;
		}
		else if (strcmp(palabra, upperCabecar("daleika")) == 0) {
			return TOKEN_DALEIKA;
		}
		else if (strcmp(palabra, upperCabecar("sh‰")) == 0) {
			return TOKEN_SHA;
		}
		else if (strcmp(palabra, upperCabecar("chagˆ")) == 0) {
			return TOKEN_CHAGO;
		}
		else if (strcmp(palabra, upperCabecar("jÈn·")) == 0) {
			return TOKEN_JENA;
		}
		else if (strcmp(palabra, upperCabecar("shirÌna")) == 0) {
			return TOKEN_SHIRINA;
		}
		else if (strcmp(palabra, upperCabecar("manÈwa")) == 0) {
			return TOKEN_MANEWA;
		}
		else if (strcmp(palabra, upperCabecar("nuÌ")) == 0) {
			return TOKEN_NUI;
		}
		else if (strcmp(palabra, upperCabecar("ts˙")) == 0) {
			return TOKEN_TSU;
		}
		else if (strcmp(palabra, upperCabecar("sh‰ni")) == 0) {
			return TOKEN_SHANI;
		}
		else if (strcmp(palabra, upperCabecar("kana")) == 0) {
			return TOKEN_KANA;
		}
		else if (strcmp(palabra, upperCabecar("tuna")) == 0) {
			return TOKEN_TUNA;
		}
		else if (strcmp(palabra, upperCabecar("katso")) == 0) {
			return TOKEN_KATSO;
		}
		else if (strcmp(palabra, upperCabecar("nai")) == 0) {
			return TOKEN_NAI;
		}
		else if (strcmp(palabra, upperCabecar("ta")) == 0) {
			return TOKEN_TA;
		}
		else if (strcmp(palabra, upperCabecar("tuluwo")) == 0) {
			return TOKEN_TULUWO;
		}
		else if (strcmp(palabra, upperCabecar("TsÎrÈ")) == 0) {
			return TOKEN_TSERE;
		}
		else if (strcmp(palabra, upperCabecar("sh·na")) == 0) {
			return TOKEN_SHANA;
		}
		else if (strcmp(palabra, upperCabecar("kiana")) == 0) {
			return TOKEN_KIANA;
		}
		else if (strcmp(palabra, upperCabecar("chani")) == 0) {
			return TOKEN_CHANI;
		}
		else if (strcmp(palabra, upperCabecar("wayu‰")) == 0) {
			return TOKEN_WAYUA;
		}
		else if (strcmp(palabra, upperCabecar("i·wa")) == 0) {
			return TOKEN_IAWA;
		}
		else if (strcmp(palabra, upperCabecar("toli")) == 0) {
			return TOKEN_TOLI;
		}
		else if (strcmp(palabra, upperCabecar("kuklÎ")) == 0) {
			return TOKEN_KUKLE;
		}
		/* Todos los ‰wÈTipo y B·kTipo ya no van a ser manejados como tokens separados sino como TOKEN_ID.
		else if (strcmp(palabra, upperCabecar("‰wÈSht·wa")) == 0) {
			return TOKEN_AWESHTAWA;
		}
		else if (strcmp(palabra, upperCabecar("‰wÈÈkla")) == 0) {
			return TOKEN_AWEEKLA;
		}
		else if (strcmp(palabra, upperCabecar("‰wÈShÈj")) == 0) {
			return TOKEN_AWESHEJ;
		}
		else if (strcmp(palabra, upperCabecar("‰wÈYÌnaKuÅlÌÅwa")) == 0) {
			return TOKEN_AWEYINAKULIWA;
		}
		else if (strcmp(palabra, upperCabecar("‰wÈJ·rj·")) == 0) {
			return TOKEN_AWEJARJA;
		}
		else if (strcmp(palabra, upperCabecar("‰wÈwˆmÎle")) == 0) {
			return TOKEN_AWEWOMELE;
		}
		else if (strcmp(palabra, upperCabecar("B·kSht·wa")) == 0) {
			return TOKEN_BAKSHTAWA;
		}
		else if (strcmp(palabra, upperCabecar("B·kÈkla")) == 0) {
			return TOKEN_BAKEKLA;
		}
		else if (strcmp(palabra, upperCabecar("B·kShÈj")) == 0) {
			return TOKEN_BAKSHEJ;
		}
		else if (strcmp(palabra, upperCabecar("B·kYÌnaKuÅlÌÅwa")) == 0) {
			return 	TOKEN_BAKYINAKULIWA;
		}
		else if (strcmp(palabra, upperCabecar("B·kJ·rj·")) == 0) {
			return TOKEN_BAKJARJA;
		}
		else if (strcmp(palabra, upperCabecar("B·kwˆmÎle")) == 0) {
			return TOKEN_BAKWOMELE;
		}
		*/
		else if (strcmp(palabra, upperCabecar("wÎik‰")) == 0) {
			return TOKEN_WEIKA;
		}
		else if (strcmp(palabra, upperCabecar("wˆmÎle")) == 0) {
			return TOKEN_WOMELE;
		}
		else if (strcmp(palabra, upperCabecar("Kalw·")) == 0) {
			return TOKEN_KALWA;
		}
		else if (strcmp(palabra, upperCabecar("Sak·")) == 0) {
			return TOKEN_SAKA;
		}
		else if (strcmp(palabra, upperCabecar("tˆka")) == 0) {
			return TOKEN_TOKA;
		}
		else if (strcmp(palabra, upperCabecar("NitsÛsi")) == 0) {
			return TOKEN_NITSOSI;
		}
		else {
			return TOKEN_UNKNOWN;
		}
	}

	TipoToken getToken(int state, string strPal) {
		const char * palabra = upperCabecar(strPal.c_str());
		if (isErrorStateNoBackChar(state) || isErrorStateWithBackChar(state)) {
			if (state == ERR_INT_B) { return TOKEN_ERR_INT_B; }
			else if (state == ERR_MENOS_B) { return TOKEN_ERR_MENOS_B; }
			else if (state == ERR_TOKEN_STRING) { return TOKEN_ERR_TOKEN_STRING; }
			else if (state == ERR_CHAR_PIPE_B) { return TOKEN_ERR_CHAR_PIPE_B; }
			else if (state == ERROR) { return TOKEN_UNKNOWN; }
			else if (state == ERR_MENOS_B) { return TOKEN_ERR_MENOS_B; }
			else if (state == ERR_TOKEN_ID_B) { return TOKEN_ERR_TOKEN_ID_B; }
			else if (state == ERR_2PUNTO_B) { return TOKEN_ERR_2PUNTO_B; }
			else if (state == ERR_TIPO_B) { return TOKEN_ERR_TIPO_B; }
			else if (state == ERR_INICIA_B) { return TOKEN_ERR_INICIA_B; }
			else if (state == ERR_CERRCONJ_B) { return TOKEN_ERR_CERRCONJ_B; }
			else if (state == ERR_LLAVE_B) { return TOKEN_ERR_LLAVE_B; }
			else if (state == ERR_OPENCONJ_B) { return TOKEN_ERR_OPENCONJ_B; }
			else if (state == ERR_OPENREG_B) { return TOKEN_ERR_OPENREG_B; }
			else if (state == ERR_TOKEN_CHAR_B) { return TOKEN_ERR_TOKEN_CHAR_B; }
			else if (state == ERR_CHAR_AMPER_B) { return TOKEN_ERR_CHAR_AMPER_B; }
			else if (state == ERR_CHAR_PIPE_B) { return TOKEN_ERR_CHAR_PIPE_B; }
			else if (state == ERR_ROMANO_B) { return TOKEN_ERR_ROMANO_B; }
			else { return TOKEN_UNKNOWN; }
		}
		else {
			if (state == F_TOKEN_INT_B) { return TOKEN_INT; }
			else if (state == F_TOKEN_MENOS_B) { return TOKEN_MENOS; }
			else if (state == F_TOKEN_MULT_B) { return TOKEN_MULTIPLICACION; }
			else if (state == F_TOKEN_ID_B || state == F_TOKEN_ID) {
				if (isKeyword(palabra)) {
					return keywordToToken(palabra);
				}
				else {
					return TOKEN_ID;
				}
			}
			else if (state == F_TOKEN_TIPO_B) { return TOKEN_TIPO; }
			else if (state == F_TOKEN_INICIA_B) { return TOKEN_INICIALIZAR; }

			else if (state == F_TOKEN_OPENCONJ_B) { return TOKEN_ABRIR_CONJUNTO; }
			else if (state == F_TOK_CERRCONJ_B) { return TOKEN_CERRAR_CONJUNTO; }

			else if (state == F_TOKEN_OPENREG_B) { return TOKEN_ABRIR_REGISTRO; }
			else if (state == F_TOK_CERR_REG) { return TOKEN_CERRAR_REGISTRO; }

			else if (state == F_TOKEN_ROMANO) { return TOKEN_ROMANO; }

			else if (state == F_TOKEN_STRING_B) { return TOKEN_STRING; }
			else if (state == F_TOKEN_CHAR) { return TOKEN_CHAR; }

			else if (state == F_TOKEN_COMP_B) { return TOKEN_COMPARADOR; }
			else if (state == F_TOKEN_COMP) { return TOKEN_COMPARADOR; }

			else if (state == F_TOKEN_INT) { return TOKEN_INT; }

			else if (state == F_TOKEN_MENOS) { return TOKEN_MENOS; }
			else if (state == F_TOKEN_CHAR_INDIV) {

				//CHAR_ARROBA,		CHAR_OPERADOR_GENERAL,	CHAR_IGUAL,			
				//F_TOKEN_CHAR_INDIV, F_TOKEN_CHAR_INDIV,		F_TOKEN_CHAR_INDIV, 
				if (strcmp(palabra, ".") == 0) { return TOKEN_PUNTO; }
				else if (strcmp(palabra, ",") == 0) { return TOKEN_COMA; }
				else if (strcmp(palabra, "(") == 0) { return TOKEN_PAR_REDONDO_ABRIR; }
				else if (strcmp(palabra, ")") == 0) { return TOKEN_PAR_REDONDO_CERRAR; }
				else if (strcmp(palabra, "[") == 0) { return TOKEN_PAR_CUADRADO_ABRIR; }
				else if (strcmp(palabra, "]") == 0) { return TOKEN_PAR_CUADRADO_CERRAR; }
				else if (strcmp(palabra, "@") == 0) { return TOKEN_ARROBA; }
				else if (strcmp(palabra, "+") == 0) { return TOKEN_MAS; }
				else if (strcmp(palabra, "/") == 0) { return TOKEN_DIVISION; }
				else if (strcmp(palabra, "%") == 0) { return TOKEN_MODULO; }
				else if (strcmp(palabra, "=") == 0) { return TOKEN_COMPARADOR; }
				else { return TOKEN_UNKNOWN; }
			}

			else if (state == F_TOKEN_WO_OPER) { 
				if (strcmp(palabra, "W÷+") == 0) { return TOKEN_MAS_ROMANO; }
				else if (strcmp(palabra, "W÷-") == 0) { return TOKEN_MENOS_ROMANO; }
				else if (strcmp(palabra, "W÷*") == 0) { return TOKEN_MULTIPLICACION_ROMANO; }
				else if (strcmp(palabra, "W÷/") == 0) { return TOKEN_DIVISION_ROMANO; }
				else if (strcmp(palabra, "W÷%") == 0) { return TOKEN_MODULO_ROMANO; }
				else { return TOKEN_UNKNOWN; }
			}
			else if (state == F_TOKEN_CHAR_POS) { return TOKEN_CHAR_POS; }
			else if (state == F_TOKEN_EOF) { return TOKEN_EOF; }
			else { return TOKEN_UNKNOWN; }

		}
	}


	// Function to extract the next token using the DFA and transition table
	Token CScanner::demeToken() {
		Token token;
		char currentChar;
		string formandoToken = "";
		bool firstChar = true;
		EstadoAutomata currentState = START;
		FamiliaChar familia = OTRO;

		token.setTipoToken(TOKEN_UNKNOWN);
		_buffer->iniciandoToken();
		token.setCurrentLine(_buffer->getCurrentLine());
		token.setCurrentPosInLine(_buffer->getCurrentPosInLine());
		token.setGlobalPosic(_buffer->getGlobalLexemaStart());
		while (true) {
			currentChar = _buffer->demeChar();
			_contChars++;
			if (currentChar == CHAR_LF) {
				_contLineas++;
			}
			familia = getFamiliaChar(currentChar);
			EstadoAutomata nextState = tablaTransicion[currentState][familia];

			if ((currentState == S_25 || currentState == S_27 || currentState == S_28) && nextState == START) {
				_contComentarios++;
			}

			if (nextState == F_TOKEN_EOF) {
				token.setTipoToken(TOKEN_EOF);
				break;
			}
			else if (nextState == ERROR) {
				token.setTipoToken(TOKEN_UNKNOWN);
				formandoToken += currentChar;
				//token.setLexema(bufferValue); cambiado por setLexema con finalizandoToken
				token.setLexema(_buffer->finalizandoToken(1));
				if (formandoToken != token.getLexema()) {
					int xyz = 0;
				}
				break;
			}
			else if (isErrorStateNoBackChar(nextState)) {
				string valToken = _buffer->finalizandoToken(1);
				token.setTipoToken(getToken(nextState, valToken));
				token.setLexema(valToken);
				formandoToken += currentChar;
				//token.setLexema(bufferValue); cambiado por setLexema con finalizandoToken
				if (formandoToken != token.getLexema()) {
					int xyz = 0;
				}
				break;
			}
			else if (isErrorStateWithBackChar(nextState)) {
				_buffer->backChar();   // regresa al caracter que adelantÛ
				_contChars--;
				if (currentChar == CHAR_LF) {
					_contLineas--;
				}

				string valToken = _buffer->finalizandoToken(1);
				token.setTipoToken(getToken(nextState, valToken));
				token.setLexema(valToken);
				//token.setLexema(bufferValue); cambiado por setLexema con finalizandoToken
				if (formandoToken != token.getLexema()) {
					int xyz = 0;
				}
				break;
			}
			else if (isInternalState(nextState)) {
				//no son estados de finalizaciÛn, debe continuar hasta cuando encuentre un estado final
				formandoToken += currentChar;

				currentState = nextState;

				if (currentState == START) {
					formandoToken = "";
					_buffer->iniciandoToken();
					token.setCurrentLine(_buffer->getCurrentLine());
					token.setCurrentPosInLine(_buffer->getCurrentPosInLine());
					token.setGlobalPosic(_buffer->getGlobalLexemaStart());
				}
			}
			else if (isFinalStateWithBackChar(nextState)) {
				_buffer->backChar();   // regresa al caracter que adelantÛ
				_contChars--;
				if (currentChar == CHAR_LF) {
					_contLineas--;
				}
				string valToken = _buffer->finalizandoToken(1);
				token.setTipoToken(getToken(nextState, valToken));
				token.setLexema(valToken);
				if (formandoToken != token.getLexema()) {
					int xyz = 0;
				}
				break;
			}
			else {
				//es Final State No Back Char
				string valToken = _buffer->finalizandoToken(1);
				token.setTipoToken(getToken(nextState, valToken));
				token.setLexema(valToken);
				formandoToken += currentChar;
				if (formandoToken != token.getLexema()) {
					int xyz = 0;
				}
				break;
			}
		}//while
		_contTokens++;
		return token;
	}

	int CScanner::getCantidadLineas() 
	{
		return _contLineas;
	}

	int CScanner::getCantidadChars()
	{
		return _contChars;
	}

	int CScanner::getCantidadTokens()
	{
		return _contTokens;
	}

	int CScanner::getCantidadComentarios()
	{
		return _contComentarios;
	}

}