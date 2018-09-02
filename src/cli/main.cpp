#include <iostream>
#include <string>

#include "../engine/src/GambitEngine.h"
#include "../engine/src/FENParser.h"
#include "../engine/src/MoveGenerator.h"

using namespace GambitEngine;

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

			u64 compBit = INT64_C(1) << i;
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
//	char inputFen[] = "4k2r/8/8/8/8/8/8/8 w kq - 0 1";
	//char inputFen[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -"; // position five
//	uint8_t length = sizeof(inputFen);
	
	GambitEngine::Board board;
	GambitEngine::MoveGenerator mvGen;

//	GambitEngine::FENParser::Deserialize(inputFen, length, board, nullptr);

	while (1)
	{		
		writeBoard(board);

		std::cout << std::endl;

		auto pieces = board.GetPieces(BLACK);
		//u64 avaMoves = ~universe;
		byte promotion = 0x00;
		/*for (int i = pieces.size() - 1; i >= 0; --i)
		{
			auto pP = pieces.at(i);
			avaMoves = board.AvailableMoves(BLACK, (PIECE)pP.Type, pP.Square8x8, promotion);

			std::cout << std::endl;
		//	writeBitboard(avaMoves);
		}*/

		std::string buffer = "";
		std::getline(std::cin, buffer);

		if(buffer == "UnmakeMove")
			board.UnmakeMove();
		else if(buffer == "clear")
			board.ResetBoard();
		else if (buffer == "attack")
		{
			std::string input = "";
			std::cout << "Input Color:" << std::endl;
			std::getline(std::cin, input);
			u64 attked = ~universe;

			if (input == "black")
				attked = board.GetBitboard().Attacked(BLACK);
			else
				attked = board.GetBitboard().Attacked(WHITE);
			std::cout << std::endl;
			writeBitboard(attked);
		}
		else if(buffer == "fen")
		{
			std::string inputFen = "";
			std::cout << "Input FEN:" << std::endl;
			std::getline(std::cin, buffer);
			FENParser::Deserialize(buffer.c_str(), buffer.length(), board, nullptr);
		}
		else if (buffer == "legal")
		{
			std::cout << "Legal moves for White:" << std::endl;
		}
		else
		{
			promotion = buffer[4] == 0 ? 0 : buffer[4];
			board.MakeMove(buffer[0], buffer[1]-'0', buffer[2], buffer[3]-'0', promotion);
		}
	}

    return 0;
}

