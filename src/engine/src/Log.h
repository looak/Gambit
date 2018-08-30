#pragma once
#include <iostream>
#include <string>
#include "typedef.h"

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
			piece = toupper(piece);
		}

		return piece;
	}
};



}