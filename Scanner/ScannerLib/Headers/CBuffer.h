#ifndef CBUFFER_H
#define CBUFFER_H

#include <stdio.h>
#include <string>
#include <stdlib.h>

#define CHAR_FIN '\0'
#define CHAR_LF 10
#define CHAR_CR 13

#define SIZE_BUFFER 128

using namespace std;

namespace ScannerLib {

	class CBuffer
	{
	private:
		FILE * _file;
		int _bufferSize = 0;
		char * _buffer;
		int _posLexemaStart = 0;
		int _posForward = 0;
		int _cualParte = 0;
		string _filename;
		unsigned long _posRead = 0;
		unsigned long _currentBasePosic = 0;
		unsigned long _currentLine = 1;
		unsigned long _currentPosInLine[2] = { 1, 1 };
	public:
		CBuffer();
		CBuffer(int size);
		void cargarBuffer();
		char demeChar();
		void backChar();
		int openFile(string filename);
		void finish();
		void iniciandoToken();
		string finalizandoToken(int cantForward);
		unsigned long getGlobalLexemaStart();

		unsigned long  getCurrentLine();
		unsigned long  getCurrentPosInLine();
	};

}

#endif 