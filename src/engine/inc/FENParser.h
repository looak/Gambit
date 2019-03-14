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
#include "Board.h"
#include "GameState.h"

namespace GambitEngine
{

struct PieceStruct
{
	SET m_set;
	PIECE m_piece;
};

class FENPieceConverter
{
public:
	static PieceStruct Convert(byte aNotation);

};

class FENBoardWriter
{
public:
	FENBoardWriter(Board& board);
	~FENBoardWriter();

	bool Write(SET set, PIECE piece);
	bool WriteCastlingState(char* states, int count);
	bool WriteEnPassant(byte file, byte rank);

	void NextFile();
	void DownRank();

private:
	void Reset();

	byte m_curFile;
	byte m_curRank;

	Board& m_board;
};

class GAMBIT_API FENParser
{
public:
	FENParser();
	~FENParser();

	static bool Deserialize(const char* fen, unsigned int length, Board& outputBoard, GameState* state);
};

}