#include "version.h"
#include "commands.h"
#include <string>
#include <sstream>
#include <algorithm>
#include "commandsUtils.h"
#include "printCommand.h"
#include "MoveGenerator.h"
#include "../engine/src/AlgebraicNotation.cpp"


bool MoveCommand(const std::string& input, GameState& state)
{
    if(input.size() == 0)
        MoveHelpCommand(1, "move");
    else if(input.length() >= 4)
    {
        byte promotion = input[4] == 0 ? 0 : input[4];
        return state.MakeMove(input[0], input[1]-'0', input[2], input[3]-'0', promotion);    
    }

    return false;
}

void MoveHelpCommand(int option, const std::string& command)
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

bool 
AvailableMovesCommand(const std::string&, GameState& state)
{
    MoveGenerator movGen;
    u32 count = 0;
    std::vector<Move> moves = movGen.getMoves(WHITE, &state.getBoard(), count);

    std::cout << " Available moves: \n ";
    auto print = [](Move& mv) { std::cout << mv.toString() << ", "; };
    for_each(moves.begin(), moves.end(), print);

    return true;
}

void 
AvailableMovesHelpCommand(int, const std::string& command)
{        
    std::string helpText("Outputs available move for current ply");
    std::cout << AddLineDivider(command, helpText);    
}

bool PrintCommand(const std::string& input, GameState& state)
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
        PrintCommands::printOptions.at("board").first(&state.getBoard(), "");
    }
    else
    if(PrintCommands::printOptions.find(tokens.front()) == PrintCommands::printOptions.end())
    {
        std::string invalidInput = tokens.size() > 0 ? tokens.front() : "Not a Value!";
        std::cout << " > Invalid command: " << invalidInput << ", help for all commands!" <<std::endl;
    }
    else
    {        
        PrintCommands::printOptions.at(tokens.front()).first(&state.getBoard(), tokens.back());
    }

    return true;
}

void PrintHelpCommand(int option, const std::string& command)
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

void FenHelpCommand(int option, const std::string& command)
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

bool FenCommand(const std::string& input, GameState& state)
{
    if(input == "get")
    {
        std::cout << " > " << FENParser::Serialize(state).value();
        return true;
    }
    else
        return state.Setup(input);
}

void ClearHelpCommand(int, const std::string& command)
{        
    std::string helpText("Resets the Game Board");
    std::cout << AddLineDivider(command, helpText); 
}

bool ClearCommand(const std::string&, GameState& state)
{
    state.getBoard().ResetBoard();
    return true;
}

void HelpHelpCommand(int, const std::string& command)
{    
    std::ostringstream ssCommand;
    ssCommand << command << ":<command> or help";

    std::string helpText("Outputs this help message or more detailed message about command");

    std::cout << AddLineDivider(ssCommand.str(), helpText);
}

bool HelpCommand(const std::string& input, GameState& state)
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
            HelpCommand("", state);
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

bool DivideDepthCommand(const std::string& input, GameState& state)
{
	GambitEngine::MoveGenerator movGen;

	u32 totalCount = 0;
	auto depth = atoi(input.c_str());
	auto divisionResult = movGen.getMovesDivision(state.getActiveSet(), &state.getBoard(), depth);
	MoveGenerator::DivisionResult::iterator it = divisionResult.begin();
	for (it; it != divisionResult.end(); ++it)
	{
		totalCount += it->second;
		std::cout << std::string(&it->first.str[0], 4) << " " << it->second << std::endl;
	}
	totalCount += (u32)divisionResult.size();
	std::cout << "total count: " << totalCount << std::endl;
	std::cout << "nodes: " << divisionResult.size() << std::endl;
	return true;
}

void DivideDepthCommandHelp(int, const std::string& command)
{
    std::ostringstream ssCommand;
    ssCommand << command << ":<depth>";

	std::string helpText("Divide position with given depth");
	std::cout << AddLineDivider(ssCommand.str(), helpText);
}

bool AboutCommand(const std::string&, GameState&)
{
    std::cout << " Gambit CLI Open Source Chess Engine 2017-2019" << std::endl    
			  << " Version: " << getVersion() << std::endl
			  << " Source: https://github.com/looak/Gambit" << std::endl 
			  << " Contributor(s): Alexander Loodin Ek" << std::endl;

    return true;
}
void AboutHelpCommand(int, const std::string& command)
{
    std::string helpText("About Gambit");
    std::cout << AddLineDivider(command, helpText);
}

bool ExitCommand(const std::string&, GameState&)
{
    std::exit(0);
    return true;
}
void ExitHelpCommand(int, const std::string& command)
{
    std::string helpText("Shutsdown Gambit");
    std::cout << AddLineDivider(command, helpText);
}