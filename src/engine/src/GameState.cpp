#include "GameState.h"
#include "FENParser.h"

using namespace GambitEngine;

GameState::GameState(Board& aBoard) :
	m_currentBoard(aBoard)
{

}

bool 
GameState::Setup(const std::string& fen)
{
	return FENParser::Deserialize(fen.c_str(), (u32)fen.size(), m_currentBoard, this);
}

bool 
GameState::MakeMove(byte sFile, byte sRank, byte tFile, byte tRank, byte promotion)
{
	auto ret = m_currentBoard.MakeMove(sFile, sRank, tFile, tRank, promotion);

	if (ret)
	{
		m_plyCounter++;
		// flip set.
		m_activeSet = (SET)!(int)m_activeSet;
	}
	return ret;
}