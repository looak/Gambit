#include "printCommand.h"
#include "commandsUtils.h"
#include "PieceDef.h"
#include <sstream>

using namespace GambitEngine;

byte convert(byte toConvert)
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

namespace PrintCommands 
{
    
void HelpCommand(const GambitEngine::Board* board, const std::string input)
{
    std::cout << " > Gambit CLI print Commands:" << std::endl;
    for(PrintCommandsMap::iterator iter = printOptions.begin(); iter != printOptions.end(); ++iter)
    {
        printOptions.at(iter->first).second(0, iter->first);
        std::cout << std::endl;
    }   
}
void HelpHelpCommand(int option, const std::string command)
{
    std::string helpText("Outputs this help message.");
    std::cout << AddLineDivider(command, helpText);
}

void BoardHelp(int option, const std::string command)
{
    std::string helpText("Prints the board.");
    std::cout << AddLineDivider(command, helpText);    
}

void Board(const GambitEngine::Board* board, const std::string input)
{
    for (byte rank = 8; rank >= 1; rank--)
    {
        std::cout << " > " << (int)rank << "  ";
        for (byte file = 'a'; file <= 'h'; file++)
        {
            auto tmp = board->GetValue(file, rank);
            auto val = convert(tmp);
            std::cout << '[' << val << ']';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << " >     A  B  C  D  E  F  G  H";
}

void Attack(const GambitEngine::Board* board, const std::string input)
{
    u64 attked = ~universe;

    if (input == "black")
        attked = board->GetBitboard().Attacked(BLACK);
    else
        attked = board->GetBitboard().Attacked(WHITE);
 
    writeBitboard(attked);
 
}
void AttackHelp(int option, const std::string command)
{
    std::ostringstream ssCommand;
    ssCommand << command << " <color> or attack";
    std::string helpText("Draws attacked squares by given color, by default white.");
    std::cout << AddLineDivider(command, helpText);
}

}