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
	case 1:
		piece = 'p';
		break;
	case 2:
		piece = 'n';
		break;
	case 3:
		piece = 'b';
		break;
	case 4:
		piece = 'r';
		break;
	case 5:
		piece = 'q';
		break;
	case 6:
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

int main()
{
	//char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	char inputFen[] = "8/8/8/8/8/8/8/3kKn2 w - - 0 1";
	uint8_t length = sizeof(inputFen);
	
	GambitEngine::Board board = GambitEngine::FENParser::Deserialize(inputFen, length);

	while (true)
	{		
		writeBoard(board);
	
		byte tmp[4];
		std::cin >> tmp;

		board.MovePiece(tmp[0], tmp[1]-'0', tmp[2], tmp[3]-'0');
	}

    return 0;
}

