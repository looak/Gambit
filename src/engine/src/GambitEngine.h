#pragma once

#include "Board.h"

namespace GambitEngine
{
	// Universal Chess Interface
	// http://wbec-ridderkerk.nl/html/UCIProtocol.html
	class Chess
	{
	public:
		static void Initialize();
	};

	class UCI
	{
	public:
		static bool HelloWorld();
	};

	class FEN
	{
	public:
		static bool InputFen(char* fen, uint8_t length, Board& outputBoard);

	};
}