#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "FENParser.h"

using namespace GambitEngine;

typedef std::map<std::string, std::pair<std::function<void(std::string, Board&)>, std::function<void(int,const std::string)>>> CommandsMap;

void FenCommand(std::string input, Board& board);
void FenHelpCommand(int option, const std::string command);

void ClearCommand(std::string input, Board& board);
void ClearHelpCommand(int option, const std::string command);

void HelpCommand(std::string input, Board& board);
void HelpHelpCommand(int option, const std::string command);

static CommandsMap aliases = {
    {"h", { HelpCommand, HelpHelpCommand } },
};

static CommandsMap options = {
    {"fen", { FenCommand, FenHelpCommand } },
    {"clear", { ClearCommand, ClearHelpCommand } },
    {"help", { HelpCommand, HelpHelpCommand } },
};