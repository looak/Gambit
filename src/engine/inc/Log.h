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
#include "typedef.h"

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG_ERROR(x) GambitEngine::Log::LogErrorImpl(x, __FILENAME__, __FUNCTION__, __LINE__);
#define LOG_INFO(x) GambitEngine::Log::LogInfoImpl(x, __FILENAME__, __LINE__);
#define LOG_WARNING(x) GambitEngine::Log::LogWarningImpl(x, __FILENAME__, __FUNCTION__, __LINE__);
#define FATAL_ASSERT(expr, x) GambitEngine::Log::AssertImpl(expr, x, __FILENAME__, __FUNCTION__, __LINE__);

namespace GambitEngine
{
class Log
{
public:
	static byte tooChar(byte pieceByte)
	{
		byte piece = pieceByte & 7;
		switch (piece)
		{
		case 1:
			piece = 'p';
			break;
		case 2:
			piece = 'n';
			break;
		case 3:
			piece = 'b';
			break;
		case 4:
			piece = 'r';
			break;
		case 5:
			piece = 'q';
			break;
		case 6:
			piece = 'k';
			break;
		}

		if ((pieceByte & 128) == 0)
		{
			piece = (byte)toupper(piece);
		}

		return piece;
	}
	static void LogInfoImpl(const char* message, const char* file, const int line)
	{
		std::cout << "[      INFO] " << file << ":" << line << " > message: " << message << std::endl;
	}

	static void LogWarningImpl(const char* message, const char* file, const char* function, const int line)
	{
		std::cout << "[   WARNING] " << file << ":" << line  << function << " message: " << message << std::endl;
	}

	static void LogErrorImpl(const char* message, const char* file, const char* function, const int line)
	{
		std::cout << "[     ERROR] " << file << ":" << line << " > " << function << " message: " << message << std::endl;
	}

	static void AssertImpl(int expression, const char* message, const char* file, const char* function, const int line)
	{
		if (expression == 0)
		{
			std::cout << "[FATAL ASRT] " << file << ":" << line << " > " << function << " message: " << message << std::endl;
			throw new std::exception(message);
		}
	}
};



}