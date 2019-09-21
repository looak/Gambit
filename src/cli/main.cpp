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

#include "version.h"
#include <iostream>
#include <string>
#include <sstream>

#include "GambitEngine.h"
#include "FENParser.h"
#include "MoveGenerator.h"

#include "commands.h"

using namespace GambitEngine;


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

int main()
{	
	std::cout << " Gambit Chess Engine 2017-2019" << std::endl			  
			  << " Version: " << getVersion();
			  
	GambitEngine::Board board;
	GambitEngine::MoveGenerator mvGen;
	GambitEngine::GameState state(board);

	while (1)
	{	
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
		
		bool madeMove = false;
		if(tokens.size() == 1)
		{
			// attempt to make move;
			madeMove = options.at("move").first(tokens[0], state);
		}
		
		if(madeMove == false)
		{
			if(tokens.size() > 0 && options.find(tokens.front()) != options.end())
			{
				if(tokens.back() == tokens.front())
					options.at(tokens.front()).first("", state);
				else
					options.at(tokens.front()).first(tokens.back(), state);
			}
			else
			{
				std::string invalidInput = tokens.size() > 0 ? tokens.front() : "Not a Value!";
				std::cout << " > Invalid command: " << invalidInput << ", help for all commands!" <<std::endl;
			}
		}

	}

    return 0;
}