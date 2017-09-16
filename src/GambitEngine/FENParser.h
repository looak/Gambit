#pragma once
#include "Board.h"

namespace GambitEngine
{

class FENParser
{
public:
	FENParser();
	~FENParser();

	static Board Deserialize(char* fen, byte length);

};

}