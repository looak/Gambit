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

#include "FENParser.h"
#include "Log.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

using namespace GambitEngine;

PieceStruct
FENPieceConverter::Convert(byte aNotation)
{
	PieceStruct returnPiece;
	returnPiece.m_set = islower(aNotation) ?  BLACK : WHITE;
	byte aNotLow = (byte)tolower(aNotation);
	
	switch (aNotLow)
	{
	case 'p':
		returnPiece.m_piece = PAWN;
		return returnPiece;
	case 'n':
		returnPiece.m_piece = KNIGHT;
		return returnPiece;
	case 'b':
		returnPiece.m_piece = BISHOP;
		return returnPiece;
	case 'r':
		returnPiece.m_piece = ROOK;
		return returnPiece;
	case 'q':
		returnPiece.m_piece = QUEEN;
		return returnPiece;
	case 'k':
		returnPiece.m_piece = KING;
		return returnPiece;
	default:
		returnPiece.m_piece = NR_OF_PIECES;
		return returnPiece;
	}

}

void
FENBoardIterator::Reset()
{
	// fen strings starts in a8 corner of black.
	m_curRank = 8;
	m_curFile = 'a';
}

void
FENBoardIterator::NextFile()
{
	m_curFile++;
}

void
FENBoardIterator::NextRank()
{
	m_curRank--;
	m_curFile = 'a';
}

FENBoardIterator&
FENBoardIterator::operator++()
{
	NextFile();
	if (m_curFile > 'h')
		NextRank();

	return *this;
}

bool
FENBoardIterator::end()
{
	return m_curRank <= 0;
}

FENBoardWriter::FENBoardWriter(Board& board) :
	m_board(board)
	
{
	Reset();
}

void
FENBoardWriter::Reset()
{
	// fen strings starts in a8 corner of black.
	m_curRank = 8;
	m_curFile = 'a';
}

void
FENBoardWriter::NextFile()
{
	m_curFile++;
}

void
FENBoardWriter::DownRank()
{
	m_curRank--;
	m_curFile = 'a';
}

bool 
FENBoardWriter::Write(SET set, PIECE piece)
{
	return m_board.PlacePiece(set, piece, m_curFile, m_curRank);
}

bool 
FENBoardWriter::WriteCastlingState(char* states, int length)
{
	int ind = 0;
	while (ind < length)
	{
		switch (states[ind])
		{
		case 'K':
			m_board.m_castleState |= 0x01;
			break;

		case 'Q':
			m_board.m_castleState |= 0x02;
			break;

		case 'k':
			m_board.m_castleState |= 0x04;
			break;

		case 'q':
			m_board.m_castleState |= 0x08;
			break;

		case '-':
			m_board.m_castleState = 0x00;
			break;

		default:
			return false;
		}

		ind++;
	}
	return true;
}

bool 
FENBoardWriter::WriteEnPassant(byte file, byte rank)
{
	if (file > 7 || rank > 7) // file < 0 || rank < 0 don't need to check these since unsigned char can't be below 0
	{
		std::ostringstream strStream;
		strStream << "File(" << file + 'a' << ") and/or rank(" << rank + '0' << ") are out of range.";
		LOG_ERROR(strStream.str().c_str());
		return false;
	}

	byte r = rank;
	if (r == 5) // black
	{
		r--;
	}
	else
	{
		r++;
	}
	// target square 
	byte tSqr = file + (r * 8);
	m_board.m_enPassantTargetSqr64 = tSqr;
	m_board.m_enPassant64 = file + (rank * 8);
	return true;
}

BoardReader::BoardReader(const GameState& state) :
	m_state(state)
{}

bool
BoardReader::end()
{
	return m_itr.end();
}

std::optional<char>
BoardReader::Read()
{
	std::optional<char> ret;
	byte value = m_state.getBoard().GetValue(m_itr.file(), m_itr.rank());
	if (value > 0)
		ret = PieceDef::printable(value);
	++m_itr;
	return ret;
}

std::optional<char> 
BoardReader::ReadToPlay() const
{
	return std::optional<char>(PieceDef::printable(m_state.getActiveSet()));
}

std::optional<std::string> 
BoardReader::ReadCastlingState() const
{
	std::optional<std::string> ret;
	byte state = m_state.getBoard().GetCastlingState();
	std::stringstream output;
	
	if (state & 1)
		output << 'K';
	if (state & 2)
		output << 'Q';
	if (state & 4)
		output << 'k';
	if (state & 8)
		output << 'q';

	ret = output.str();
	return ret;
}

std::optional<std::string> 
BoardReader::ReadEnPassant() const
{
	std::optional<std::string> ret;
	byte state = m_state.getBoard().GetEnPassantState();
	if(state > 0) {
		std::stringstream output;
		byte file = state % 8;
		byte rank = state / 8;
		output << (file + 'a');
		output << (rank + '1');
		ret = output.str();
	}
	else {
		ret = "-";
	}

	return ret;
}

std::optional<std::string>
BoardReader::ReadInt(int value) const
{
	std::optional<std::string> ret;
	std::stringstream output;
	output << value;
	ret = output.str();
	return ret;
}

std::optional<std::string>
BoardReader::ReadPly() const
{
	return ReadInt(m_state.getPly());
}

std::optional<std::string>
BoardReader::ReadCounter() const
{
	return ReadInt(m_state.getMoveCounter());
}

FENParser::FENParser()
{
}

std::optional<std::string>
FENParser::Serialize(const GameState& gameState)
{
	std::optional<std::string> returnValue;
	BoardReader reader(gameState);
	std::stringstream outputFEN;

	u32 rowCounter = 0;
	do
	{
		byte rank = reader.rank();
		auto result = reader.Read();
		byte newRank = reader.rank();

		if (result.has_value()) {
			if (rowCounter > 0)
				outputFEN << rowCounter;
			outputFEN << result.value();
			rowCounter = 0;
			if (rank != newRank && reader.rank() != 0) 
				outputFEN << '/';
			continue;
		}

		rowCounter++;
		if (rank != newRank) {
			if (rowCounter > 0)
				outputFEN << rowCounter;
			outputFEN << '/';
			rowCounter = 0;
			continue;
		}

	} while (!reader.end());

	// add white space;
	outputFEN << ' ';

	auto colorToPlay = reader.ReadToPlay();
	if (colorToPlay.has_value())
		outputFEN << colorToPlay.value() << ' ';
	
	auto castling = reader.ReadCastlingState();
	if (castling.has_value())
		outputFEN << castling.value() << ' ';

	auto enPassant = reader.ReadEnPassant();
	if (enPassant.has_value())
		outputFEN << enPassant.value() << ' ';

	auto ply = reader.ReadPly();
	if (ply.has_value())
		outputFEN << ply.value() << ' ';
	
	auto counter = reader.ReadCounter();
	if (counter.has_value())
		outputFEN << counter.value();


	outputFEN << '\0';
	if (outputFEN.str().size() > 0)
		returnValue = outputFEN.str();
	return returnValue;
}

bool FENParser::Deserialize(const char* fen, unsigned int length, Board& outputBoard, GameState* state)
{
	outputBoard.ResetBoard();
	FENBoardWriter boardWriter(outputBoard);
	byte counter = 0;
	byte index = 0;

	for (byte i = 0; i < length; ++i)
	{
		auto curr = fen[i];
		if (counter < 64)
		{
			if (curr == '/')
			{
				boardWriter.DownRank();
			}
			else if (isdigit(curr))
			{
				byte steps = curr - '0';
				counter += steps;
				while (steps > 0)
				{
					boardWriter.NextFile();
					steps--;
				}
			}
			else if (curr == ' ')
			{
				LOG_ERROR("Syntax error in FEN string");
				return false;
			}
			else
			{
				auto piece = FENPieceConverter::Convert(curr);
				boardWriter.Write(piece.m_set, piece.m_piece);
				counter++;

				boardWriter.NextFile();
			}
		}

		if (counter == 64)
		{
			index = i;
			break;
		}
	}

	// expecting white space
	do
	{
		++index;
	} while (fen[index] == ' ');

	// write defaults to board since we're just reading a short fen.
	if (fen[index] == '\0')
	{
		LOG_INFO("Short FEN detected, writing defaults.");
		char castlingState[4]{ 'K','Q','k','q' };
		boardWriter.WriteCastlingState(&castlingState[0], 4);
		return true;
	}

	// we should be on side to move char.
	if (state != nullptr)
	{
		switch (fen[index])
		{
		case 'w':
			state->m_activeSet = WHITE;
			break;
		case 'b':
			state->m_activeSet = BLACK;
			break;
		default:
			LOG_ERROR("Syntax error in FEN string");
			return false;
		}
	}
	// expecting white space
	// move forward, should now be on Castling;
	do
	{
		++index;
	} while (fen[index] == ' ');

	char castlState[4];
	int castlingCounter = 0;
	while (fen[index] != ' ' && castlingCounter < 4)
	{
		castlState[castlingCounter] = fen[index];
		castlingCounter++;
		index++;
	}
	if (!boardWriter.WriteCastlingState(castlState, castlingCounter))
	{
		LOG_ERROR("Syntax error in FEN string");
		return false;
	}
	
	index++;
	// should be on en passant now.
	if (fen[index] != '-')
	{
		byte file = fen[index] - 'a';
		index++;
		byte rank = fen[index] - '1';

		if (!boardWriter.WriteEnPassant(file, rank))
		{
			LOG_ERROR("Syntax error in FEN string");
			return false;
		}
	}

	index+=2;
	// now on ply moves or half moves.
	if (state != nullptr)
	{		
		char a = fen[index];
		state->m_plyCounter = a - '0';
		index += 2;
		
		a = fen[index];
		state->m_moveCounter = a - '0';
	}

	return true;
}
