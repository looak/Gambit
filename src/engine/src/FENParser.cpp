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

FENBoardWriter::FENBoardWriter(Board& board) :
	m_board(board)
	
{
	Reset();
}

FENBoardWriter::~FENBoardWriter()
{

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

		case'-':
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
	if (file < 0 || file > 7 || rank < 0 || rank > 7)
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


FENParser::FENParser()
{
}

FENParser::~FENParser()
{
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
		boardWriter.WriteCastlingState("KQkq", 4);
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

	index++;
	// now on ply moves or half moves.
	if (state != nullptr)
	{		
		char a = fen[index];
		state->m_plyCounter = a - 'a';
		index += 2;
		
		a = fen[index];
		state->m_moveCounter = a - 'a';
	}

	return true;
}
