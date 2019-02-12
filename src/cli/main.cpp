#include <iostream>
#include <string>
#include <sstream>

#include "GambitEngine.h"
#include "FENParser.h"
#include "MoveGenerator.h"

#include "commands.h"

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
		std::cout << " " << (int)rank << "  ";
		for (byte file = 'a'; file <= 'h'; file++)
		{
			auto tmp = board.GetValue(file, rank);
			auto val = too(tmp);
			std::cout << '[' << val << ']';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "     A  B  C  D  E  F  G  H";
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
	GambitEngine::Board board;
	GambitEngine::MoveGenerator mvGen;


	while (1)
	{	
		writeBoard(board);

		std::cout << std::endl << " Gambit:" << std::endl << " ";
		std::string buffer = "";
		std::getline(std::cin, buffer);
		std::istringstream command(buffer);
		std::vector<std::string> tokens;
		std::string token;

		while(std::getline(command, token, ':'))
		{
			tokens.push_back(token);
		}
		
		if(options.find(tokens.front()) != options.end())
		{
			if(tokens.back() == tokens.front())
				options.at(tokens.front()).first("", board);
			else
				options.at(tokens.front()).first(tokens.back(), board);
		}
		else
		{
			std::cout << " > Invalid command: " << tokens.front() << std::endl;
			options.at("help").first("", board);
		}
		

	}

    return 0;
}

/*
auto pieces = board.GetPieces(BLACK);
		//u64 avaMoves = ~universe;
		byte promotion = 0x00;
		/*for (int i = pieces.size() - 1; i >= 0; --i)
		{
			auto pP = pieces.at(i);
			avaMoves = board.AvailableMoves(BLACK, (PIECE)pP.Type, pP.Square8x8, promotion);

			std::cout << std::endl;
		//	writeBitboard(avaMoves);
		}* /

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
		else if (buffer == "divide")
		{

		}
		else
		{
			promotion = buffer[4] == 0 ? 0 : buffer[4];
			board.MakeMove(buffer[0], buffer[1]-'0', buffer[2], buffer[3]-'0', promotion);
		}
*/