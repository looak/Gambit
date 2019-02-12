#include "commands.h"
#include <string>
#include <sstream>
#include "commandsUtils.h"

#include "printCommand.h"


bool MoveCommand(std::string input, Board& board)
{
    if(input.size() == 0)
        MoveHelpCommand(1, "move");
    else if(input.length() >= 4)
    {
        byte promotion = input[4] == 0 ? 0 : input[4];
        return board.MakeMove(input[0], input[1]-'0', input[2], input[3]-'0', promotion);    
    }

    return false;
}
void MoveHelpCommand(int option, const std::string command)
{
    std::ostringstream ssCommand;
    ssCommand << command << ":<AN>";

    std::string helpText("Move piece on board according to Algebraic Notation");
    if(option == 0)
    {
        std::cout << AddLineDivider(ssCommand.str(), helpText);
    }
    else
    {        
        std::cout << AddLineDivider(ssCommand.str(), helpText) << std::endl;
        std::cout << whitespace << "By default any input will be attempted to be executed as a Move." << std::endl;
        std::cout << whitespace << "Algebraic Notation(AN) standard definition can be found https://en.wikipedia.org/wiki/Algebraic_notation_(chess)" << std::endl;
        std::cout << whitespace << "Example: e2e4 or  Nf3 (if non ambiguous)" << std::endl;
    }
}

bool PrintCommand(std::string input, Board& board)
{		
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss;
    iss.str(input);

    while(std::getline(iss, token, ' '))
    {
        tokens.push_back(token);
    }
    if(tokens.size() == 0)
    {
        PrintCommands::printOptions.at("board").first(&board, "");
    }
    else
    if(PrintCommands::printOptions.find(tokens.front()) == PrintCommands::printOptions.end())
    {
        std::string invalidInput = tokens.size() > 0 ? tokens.front() : "Not a Value!";
        std::cout << " > Invalid command: " << invalidInput << ", help for all commands!" <<std::endl;
    }
    else
    {        
        PrintCommands::printOptions.at(tokens.front()).first(&board, tokens.back());
    }

    return true;
}

void PrintHelpCommand(int option, const std::string command)
{
    if(option == 0)
    {
        std::ostringstream ssCommand;
        ssCommand << command << ":<command> or print";
        std::string helpText("Default prints board or Prints a command.");
        std::cout << AddLineDivider(ssCommand.str(), helpText);
    }
    else
    {        
        PrintCommands::printOptions.at("help").first(nullptr, "");
    }
}

void FenHelpCommand(int option, const std::string command)
{
    std::ostringstream ssCommand;
    ssCommand << command << ":<FEN>";

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

bool FenCommand(std::string input, Board& board)
{
    return FENParser::Deserialize(input.c_str(), input.length(), board, nullptr);         
}

void ClearHelpCommand(int option, const std::string command)
{        
    std::string helpText("Resets the GameBoard");
    std::cout << AddLineDivider(command, helpText);    
}

bool ClearCommand(std::string input, Board& board)
{
    board.ResetBoard();
    return true;
}

void HelpHelpCommand(int option, const std::string command)
{    
    std::ostringstream ssCommand;
    ssCommand << command << ":<command> or help";

    std::string helpText("Outputs this help message or more detailed message about command.");

    std::cout << AddLineDivider(ssCommand.str(), helpText);
}

bool HelpCommand(std::string input, Board& board)
{
    if(input.empty() == false)
    {        
	    if(input.size() > 0 && options.find(input) != options.end())
        {
            options.at(input).second(1, input);
            std::cout << std::endl;
        }
        else
        {
            std::string invalidInput = input.length() > 0 ? input : "Not a Value!";
            std::cout << " > Invalid command: " << invalidInput << ", help for all commands!" <<std::endl;
            HelpCommand("", board);
        }
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

    return true;
}

bool ExitCommand(std::string input, Board& board)
{
    std::exit(0);
    return true;
}
void ExitHelpCommand(int option, const std::string command)
{
    std::string helpText("Shutsdown Gambit.");
    std::cout << AddLineDivider(command, helpText);    
}