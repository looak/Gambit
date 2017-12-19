#pragma once

#include <stdint.h>
#include "Board.h"



namespace GambitEngine
{
	// Universal Chess Interface
	// http://wbec-ridderkerk.nl/html/UCIProtocol.html
	class Chess
	{
	public:
		static GAMBIT_API void Initialize();
	};

	class UCI
	{
	public:
		static GAMBIT_API bool HelloWorld();
	};

	class FEN
	{
	public:
		static GAMBIT_API bool InputFen(char* fen, uint8_t length, Board& outputBoard);

	};
}