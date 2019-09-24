// Gambit Chess Engine - a Chess AI
// Copyright (C) 2019  Alexander Loodin Ek

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <variant>

#include "FENParser.h"

using namespace GambitEngine;

typedef std::variant<Board, GameState> GameStateBoard;
typedef std::map<std::string, std::pair<std::function<bool(const std::string&, GameState&)>, std::function<void(int,const std::string&)>>> CommandsMap;

bool FenCommand(const std::string& input, GameState& state);
void FenHelpCommand(int option, const std::string& command);

bool ClearCommand(const std::string& input, GameState& state);
void ClearHelpCommand(int option, const std::string& command);

bool HelpCommand(const std::string& input, GameState& state);
void HelpHelpCommand(int option, const std::string& command);

bool PrintCommand(const std::string& input, GameState& state);
void PrintHelpCommand(int option, const std::string& command);

bool MoveCommand(const std::string& input, GameState& state);
void MoveHelpCommand(int option, const std::string& command);

bool AvailableMovesCommand(const std::string& input, GameState& state);
void AvailableMovesHelpCommand(int option, const std::string& command);

bool DivideDepthCommand(const std::string& input, GameState& state);
void DivideDepthCommandHelp(int option, const std::string& command);

bool AboutCommand(const std::string& input, GameState& state);
void AboutHelpCommand(int option, const std::string& command);

bool ExitCommand(const std::string& input, GameState& state);
void ExitHelpCommand(int option, const std::string& command);

static CommandsMap aliases = {
    {"h", { HelpCommand, HelpHelpCommand } },    
    {"m", { MoveCommand, MoveHelpCommand } },
	{"x", { ExitCommand, ExitHelpCommand } },
};

static CommandsMap options = {
    {"fen", { FenCommand, FenHelpCommand } },
    {"clear", { ClearCommand, ClearHelpCommand } },
    {"help", { HelpCommand, HelpHelpCommand } },
    {"print", { PrintCommand, PrintHelpCommand } },
    {"move", { MoveCommand, MoveHelpCommand } },
	{"divide", {DivideDepthCommand, DivideDepthCommandHelp } },
    {"show", { AvailableMovesCommand, AvailableMovesHelpCommand } },
    {"exit", { ExitCommand, ExitHelpCommand } },
    {"about", {AboutCommand, AboutHelpCommand } }

};