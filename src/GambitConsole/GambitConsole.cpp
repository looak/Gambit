// GambitConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "..\GambitEngine\GambitEngine.h"
#include "..\GambitEngine\FENParser.h"
#include "..\GambitEngine\MoveGenerator.h"


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

void writeMoves(std::vector<GambitEngine::Move> moves)
{
	for (u32 i = 0; i < moves.size(); i++)
	{
		auto mv = moves.at(i);
		byte sRank = mv.fromSqr >> 3;
		byte sFile = mv.fromSqr & 7; 
		
		byte tRank = mv.toSqr >> 3;
		byte tFile = mv.toSqr & 7; 

		byte promote = mv.promotion;

		char sF = sFile + 'a';
		char sR = sRank + '1';
		char tF = tFile + 'a';
		char tR = tRank + '1';

		if(promote != 0x00)
			std::cout << sF << sR << tF << tR << promote << std::endl;
		else
			std::cout << sF << sR << tF << tR << std::endl;
	}
}

void writeBitboard(const u64 board)
{
	for (int r = 7; r >= 0; r--)
	{
		for (int f = 0; f < 8; f++)
		{
			int i = r * 8 + f;

			u64 compBit = 1i64 << i;
			if (board & compBit)
				std::cout << " 1";
			else
				std::cout << " .";
		}
		std::cout << std::endl;
	}
}

int main()
{
	//char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	//char inputFen[] = "8/8/8/8/4N3/8/8/3kKN2 w - - 0 1";
	//char inputFen[] = "Q7/3R4/4B3/8/6N1/8/2K5/8 w - - 0 1";
	//char inputFen[] = "8/8/8/8/8/8/8/4K3 w - - 0 1";
	//char inputFen[] = "8/8/8/8/1R3r2/8/8/8 w - - 0 1";
	//char inputFen[] = "8/8/8/8/1Q6/8/8/8 w - - 0 1";
	//char inputFen[] = "8/8/8/8/8/8/8/R3K2R w KQ -";
	//char inputFen[] = "8/8/8/4Pp2/8/8/8/8 w KQ f6 0 1";
	//char inputFen[] = "8/1P6/8/8/8/8/8/8 w KQ f6 0 1";
	//char inputFen[] = "8/8/8/1R6/8/8/r7/1r2K3 w KQ f6 0 1";
	char inputFen[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
	uint8_t length = sizeof(inputFen);
	
	GambitEngine::Board board;
	GambitEngine::MoveGenerator mvGen;

	GambitEngine::FENParser::Deserialize(inputFen, length, board, nullptr);

	while (true)
	{		
		writeBoard(board);

		std::cout << std::endl;
		writeBitboard(board.GetBitboard().Attacked(BLACK, true));
		
		auto pieces = board.GetPieces(WHITE);
		u64 avaMoves = ~universe;
		byte promotion = 0x00;
		for (int i = pieces.size() - 1; i >= 0; --i)
		{
			auto pP = pieces.at(i);
			avaMoves |= board.AvailableMoves(WHITE, (PIECE)pP.Type, pP.Square8x8, promotion);
		}
		std::cout << std::endl;
		writeBitboard(avaMoves);

		u32 moveCount = 0;
		mvGen.FindBestMove(WHITE, &board, moveCount, 2);
	//	writeMoves(mvs);

		std::cout << moveCount << std::endl;
	
		byte tmp[5];
		std::cin >> tmp;

		promotion = tmp[4] == 0 ? 0 : tmp[4];
		board.MakeMove(tmp[0], tmp[1]-'0', tmp[2], tmp[3]-'0', promotion);
	}

    return 0;
}

