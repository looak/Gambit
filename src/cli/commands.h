#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "FENParser.h"

using namespace GambitEngine;

typedef std::map<std::string, std::pair<std::function<bool(std::string, Board&)>, std::function<void(int,const std::string)>>> CommandsMap;

bool FenCommand(std::string input, Board& board);
void FenHelpCommand(int option, const std::string command);

bool ClearCommand(std::string input, Board& board);
void ClearHelpCommand(int option, const std::string command);

bool HelpCommand(std::string input, Board& board);
void HelpHelpCommand(int option, const std::string command);

bool PrintCommand(std::string input, Board& board);
void PrintHelpCommand(int option, const std::string command);

bool MoveCommand(std::string input, Board& board);
void MoveHelpCommand(int option, const std::string command);

bool ExitCommand(std::string input, Board& board);
void ExitHelpCommand(int option, const std::string command);

static CommandsMap aliases = {
    {"h", { HelpCommand, HelpHelpCommand } },
};

static CommandsMap options = {
    {"fen", { FenCommand, FenHelpCommand } },
    {"clear", { ClearCommand, ClearHelpCommand } },
    {"help", { HelpCommand, HelpHelpCommand } },
    {"print", { PrintCommand, PrintHelpCommand } },
    {"move", { MoveCommand, MoveHelpCommand } },
    {"exit", { ExitCommand, ExitHelpCommand } },

};