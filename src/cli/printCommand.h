#pragma once

#include <iostream>
#include <map>
#include <functional>
#include "Board.h"

typedef std::map<std::string, std::pair<std::function<void(const GambitEngine::Board*, const std::string)>, std::function<void(int,const std::string)>>> PrintCommandsMap;

namespace PrintCommands
{

    void Board(const GambitEngine::Board* board, const std::string input);
    void BoardHelp(int option, const std::string command);

    void Bitboard(const GambitEngine::Board* board, const std::string input);
    void BitboardHelp(int option, const std::string command);

    void Attack(const GambitEngine::Board* board, const std::string input);
    void AttackHelp(int option, const std::string command);
    
    void HelpCommand(const GambitEngine::Board* board, const std::string input);
    void HelpHelpCommand(int option, const std::string command);

    static PrintCommandsMap printOptions = {
        {"board", { Board, BoardHelp } },
        {"attack", { Attack, AttackHelp } },
        {"help", { HelpCommand, HelpHelpCommand } }
    };
}
