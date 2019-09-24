#pragma once

#include "Board.h"

namespace GambitEngine
{
	class GameState;
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
		/* 
		* Will return true or false if we succeed to parse fen.
		* if false board will be empty. */
		static bool InputFen(char* fen, uint8_t length, Board& outputBoard);

		static bool InputFen(char* fen, uint8_t length, GameState& state);


		/*
		* serializes current given state into FEN string */
		static std::string FEN::OutputFEN(const GameState& state);
	};
}