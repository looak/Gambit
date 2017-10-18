#include "GameState.h"
#include "FENParser.h"

using namespace GambitEngine;

GameState::GameState()
{
}


GameState::~GameState()
{

}

void 
GameState::Setup(char* fen, int length)
{
	FENParser::Deserialize(fen, length, m_currentBoard, this);
}
