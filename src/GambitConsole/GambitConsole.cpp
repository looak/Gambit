// GambitConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "..\GambitEngine\GambitEngine.h"
#include "..\GambitEngine\FENParser.h"


byte too(byte toConvert)
{
	byte piece = toConvert & 7;	
	switch (piece)
	{
	case PAWN:
		piece = 'p';
		break;
	case KNIGHT:
		piece = 'n';
		break;
	case BISHOP:
		piece = 'b';
		break;
	case ROOK:
		piece = 'r';
		break;
	case QUEEN:
		piece = 'q';
		break;
	case KING:
		piece = 'k';
		break;
	}

	if ((toConvert & 128) == 0)
	{
		piece = toupper(piece);
	}

	return piece;
}

void writeBoard(const GambitEngine::Board& board)
{
	for (byte rank = 8; rank >= 1; rank--)
	{
		for (byte file = 'a'; file <= 'h'; file++)
		{
			auto tmp = board.GetValue(file, rank);
			auto val = too(tmp);
			std::cout << '[' << val << ']';
		}
		std::cout << std::endl;
	}
}

void writeBitboard(const uint64 board)
{
	for (byte i = 64; i > 0; --i)
	{
		if (i % 8 == 0)
			std::cout << std::endl;

		uint64 compBit = 1i64 << i;
		if (board & compBit)
			std::cout << " 1";
		else
			std::cout << " .";

	}
}

int main()
{
	//char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	char inputFen[] = "8/8/8/8/4N3/8/8/3kKN2 w - - 0 1";
	uint8_t length = sizeof(inputFen);
	
	GambitEngine::Board board;

	GambitEngine::FENParser::Deserialize(inputFen, length, board, nullptr);

	while (true)
	{		
		writeBoard(board);

		std::cout << std::endl;
		writeBitboard(board.bitboard.MaterialCombined(WHITE));
		
		std::cout << std::endl;
		writeBitboard(board.bitboard.Attacked(WHITE));
	
		byte tmp[4];
		std::cin >> tmp;

		board.MakeMove(tmp[0], tmp[1]-'0', tmp[2], tmp[3]-'0');
	}

    return 0;
}

