#pragma once
#include "Board.h"

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
	FENBoardWriter();
	~FENBoardWriter();

	bool Write(SET set, PIECE piece, Board& board);

	void NextFile();
	void DownRank();

private:
	void Reset();

	byte m_curFile;
	byte m_curRank;
};

class GAMBIT_API FENParser
{
public:
	FENParser();
	~FENParser();

	static Board Deserialize(char* fen, byte length);

};

}