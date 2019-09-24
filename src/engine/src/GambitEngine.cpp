#include "GambitEngine.h"
#include "GameState.h"
#include "FENParser.h"
#include <optional>
#include <Log.h>

namespace GambitEngine
{
	bool UCI::HelloWorld()
	{
		return true;
	}

	bool FEN::InputFen(char* fen, uint8_t length, GameState& state)
	{
		// if we fail to parse fen, reset board.
		if (!FENParser::Deserialize(fen, length, state.getBoard(), &state))
		{
			state.getBoard().ResetBoard();
			return false;
		}
		return true;
	}

	bool FEN::InputFen(char* fen, uint8_t length, Board& outputBoard)
	{	
		// if we fail to parse fen, reset board.
		if (!FENParser::Deserialize(fen, length, outputBoard, nullptr))
		{
			outputBoard.ResetBoard();
			return false;
		}
		return true;
	}

	std::string FEN::OutputFEN(const GameState& state)
	{
		auto result = FENParser::Serialize(state);
		if(!result.has_value())
			LOG_ERROR("Something went wrong when serializing game state into FEN.");

		return result.value();
	}

	void Chess::Initialize()
	{

	}
}