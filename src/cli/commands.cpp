#include "commands.h"
#include <sstream>

static const std::string whitespace = " > ......................... ";
static const int lineLength = 24;
std::string AddLineDivider(std::string command, std::string helpText)
{
    int lengthLeft = lineLength - command.length();
    _ASSERT(lengthLeft >= 0);

    std::stringstream output;
    output << " > " << command << ' ';
    while (lengthLeft > 0)
    {
        output << '.';
        --lengthLeft;
    }
    output << " " << helpText;
    return output.str();            
}

void FenHelpCommand(int option, const std::string command)
{
    std::ostringstream ssCommand;
    ssCommand << command << ":<input FEN>";

    std::string helpText("Sets the board to given FEN");
    if(option == 0)
    {
        std::cout << AddLineDivider(ssCommand.str(), helpText);
    }
    else
    {        
        std::cout << AddLineDivider(ssCommand.str(), helpText) << std::endl;
        std::cout << whitespace << "Fen standard definition can be found https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation" << std::endl;
        std::cout << whitespace << "Starting position Example: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" << std::endl;
    }
}

void FenCommand(std::string input, Board& board)
{
    FENParser::Deserialize(input.c_str(), input.length(), board, nullptr);         
}

void ClearHelpCommand(int option, const std::string command)
{        
    std::string helpText("Resets the GameBoard");
    std::cout << AddLineDivider(command, helpText);    
}

void ClearCommand(std::string input, Board& board)
{
    board.ResetBoard();
}

void HelpHelpCommand(int option, const std::string command)
{    
    std::ostringstream ssCommand;
    ssCommand << command << ":<command> or help";

    std::string helpText("Outputs this help message or more detailed message about command.");

    std::cout << AddLineDivider(ssCommand.str(), helpText);
}

void HelpCommand(std::string input, Board& board)
{
    if(input.empty() == false)
    {
        options.at(input).second(1, input);
        std::cout << std::endl;
    }
    else
    {
        std::cout << " > Gambit CLI Commands:" << std::endl;
        for(CommandsMap::iterator iter = options.begin(); iter != options.end(); ++iter)
        {
            options.at(iter->first).second(0, iter->first);
            std::cout << std::endl;
        }        
    }    
}