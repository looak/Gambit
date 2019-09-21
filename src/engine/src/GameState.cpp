#include "GameState.h"
#include "FENParser.h"

using namespace GambitEngine;

GameState::GameState(Board& aBoard) :
	m_currentBoard(aBoard)
{

}


GameState::~GameState()
{

}

bool 
GameState::Setup(const char* fen, int length)
{
	FENParser::Deserialize(fen, length, m_currentBoard, this);
}
