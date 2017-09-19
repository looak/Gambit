#include "stdafx.h"
#include "FENParser.h"
#include <stdio.h>

using namespace GambitEngine;


PieceStruct
FENPieceConverter::Convert(byte aNotation)
{
	PieceStruct returnPiece;
	returnPiece.m_set = islower(aNotation) ?  BLACK : WHITE;
	byte aNotLow = tolower(aNotation);
	
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

FENBoardWriter::FENBoardWriter()
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
FENBoardWriter::Write(SET set, PIECE piece, Board& board)
{
	return board.PlacePiece(set, piece, m_curFile, m_curRank);
}


FENParser::FENParser()
{
}

FENParser::~FENParser()
{
}

bool FENParser::Deserialize(char* fen, byte length, Board& outputBoard)
{
	FENBoardWriter boardWriter;
	byte counter = 0;

	for (byte i = 0; i < length; i++)
	{
		if (counter < 64)
		{
			auto curr = fen[i];
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
				printf("Syntax error in FEN string");
				return false;
			}
			else
			{
				auto piece = FENPieceConverter::Convert(curr);
				boardWriter.Write(piece.m_set, piece.m_piece, outputBoard);
				counter++;

				boardWriter.NextFile();
			}
		}
	}

	return true;
}
