#include "CBuffer.h"
#include <string.h>

namespace ScannerLib
{

	CBuffer::CBuffer(int size) {
		_bufferSize = size;
		_buffer = new char[_bufferSize + 1];
		_buffer[_bufferSize / 2] = 32;//que no tenga el sentinela porque sino va a detenerse cuando finalice de procesar la 2da mitad del buffer
		_buffer[_bufferSize] = 32;//que no tenga el sentinela porque sino va a detenerse cuando finalice de procesar la 2da mitad del buffer
	}

	CBuffer::CBuffer() {
		_bufferSize = 1024; //si no se especifica el tamaño del buffer, se crea de 1024 bytes.
		_buffer = new char[_bufferSize + 1];
		_buffer[_bufferSize / 2] = 32;//que no tenga el sentinela porque sino va a detenerse cuando finalice de procesar la 2da mitad del buffer
		_buffer[_bufferSize] = 32;//que no tenga el sentinela porque sino va a detenerse cuando finalice de procesar la 2da mitad del buffer
	}

	int CBuffer::openFile(string filename) {
		_filename = filename;
		errno_t err = fopen_s(&_file, _filename.c_str(), "r");
		if (err != 0) {
			return 1;
		}
		else {
			return 0;
		}
	}

	void CBuffer::finish() {
		fclose(_file);
	}

	void CBuffer::cargarBuffer() {
		int inic = 0;
		if (_cualParte == 0) {
			_currentBasePosic = _posRead;
		}
		else {
			inic = _bufferSize / 2;
		}
		int cantRead = fread(&_buffer[inic], sizeof(char), _bufferSize / 2, _file);
		_posRead += cantRead;
		if (cantRead < (_bufferSize / 2)) {
			_buffer[inic + cantRead] = CHAR_FIN;
		}
	}

	void CBuffer::iniciandoToken() {
		_posLexemaStart = _posForward;
	}

	unsigned long CBuffer::getGlobalLexemaStart() {
		return _currentBasePosic + (unsigned long)_posForward;
	}

	string CBuffer::finalizandoToken(int cantForward) {
		char * bufferTmp = NULL;
		int posFinal = _posForward - cantForward;
		if (_posLexemaStart <= posFinal) {
			//cuando el token inició y terminó en la misma parte, o inicio en parte=0 y terminó en parte 1.
			bufferTmp = new char[(posFinal - _posLexemaStart + 1) + 1];
			strncpy_s(bufferTmp, (posFinal - _posLexemaStart + 1) + 1, &_buffer[_posLexemaStart], (posFinal - _posLexemaStart + 1));
			bufferTmp[(posFinal - _posLexemaStart + 1)] = 0;
		}
		else {
			//cuando el token inició en la parte = 1 y terminó en la parte = 0.
			bufferTmp = new char[(_bufferSize - _posLexemaStart) + (posFinal + 1) + 1];
			strncpy_s(bufferTmp, (_bufferSize - _posLexemaStart) + 1, &_buffer[_posLexemaStart], (_bufferSize - _posLexemaStart));
			strncpy_s(&bufferTmp[(_bufferSize - _posLexemaStart)], (posFinal + 1) + 1, &_buffer[0], (posFinal + 1));
			bufferTmp[(_bufferSize - _posLexemaStart) + (posFinal + 1)] = 0;
		}
		return string(bufferTmp);
	}

	void CBuffer::backChar() {
		if (_posForward == 0) {
			_posForward = _bufferSize - 1;
		}
		else {
			_posForward--;
		}
		if (_buffer[_posForward] == CHAR_LF) {
			_currentLine--;
			_currentPosInLine[0] = _currentPosInLine[1];
		}
		else {
			_currentPosInLine[0] = _currentPosInLine[1];
		}
	}

	char CBuffer::demeChar() {
		char result;
		if (_buffer[_posForward] == CHAR_FIN) {
			result = CHAR_FIN;//fin del archivo
			_posForward++;
		}
		else {
			if (_cualParte == 0 && _posForward == _bufferSize / 2) {
				//se acabó la primera mitad, hay que cargar la segunda mitad.
				_cualParte = 1;
				cargarBuffer();
			}
			else if (_cualParte == 1 && _posForward == _bufferSize) {
				//se acabó la segunda mitad, hay que cargar la primera mitad.
				_cualParte = 0;
				cargarBuffer();
				_posForward = 0;
			}
			result = _buffer[_posForward];
			if (result != CHAR_FIN) {
				_posForward++;
			}
		}
		if (result == CHAR_LF) {
			_currentLine++;
			_currentPosInLine[1] = _currentPosInLine[0];
			_currentPosInLine[0] = 1;
		}
		else  {
			_currentPosInLine[1] = _currentPosInLine[0];
			_currentPosInLine[0] += 1;
		}
		return result;
	}

	unsigned long  CBuffer::getCurrentLine() {
		return _currentLine;
	}
	unsigned long  CBuffer::getCurrentPosInLine() {
		return _currentPosInLine[0];
	}


}