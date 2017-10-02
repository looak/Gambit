#include "stdafx.h"
#include "FENParser.h"
#include <stdio.h>
#include <stdlib.h>

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

bool 
FENBoardWriter::WriteCastlingState(char* states, int length, Board & board)
{
	int ind = 0;
	while (ind < length)
	{
		switch (states[ind])
		{
		case 'K':
			board.m_castleState |= 0x01;
			break;

		case 'Q':
			board.m_castleState |= 0x02;
			break;

		case 'k':
			board.m_castleState |= 0x02;
			break;

		case 'q':
			board.m_castleState |= 0x02;
			break;

		case'-':
			board.m_castleState = 0x00;
			break;

		default:
			return false;
		}

		ind++;
	}
	return true;
}

bool 
FENBoardWriter::WriteEnPassant(byte file, byte rank, Board& board)
{
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
	board.m_enPassantTargetSqr64 = tSqr;
	board.m_enPassant64 = file + (rank * 8);
	return true;
}


FENParser::FENParser()
{
}

FENParser::~FENParser()
{
}

bool FENParser::Deserialize(const char* fen, byte length, Board& outputBoard, GameState* state)
{
	FENBoardWriter boardWriter;
	byte counter = 0;
	byte index = 0;

	for (byte i = 0; i < length; i++)
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

		if (counter == 64)
		{
			index = i;
			break;
		}
	}

	index+=2;
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
			printf("Syntax error in FEN string");
			return false;
		}
	}
	// move forward two, should now be on Castling;
	index += 2;
	char castlState[4];
	int castlingCounter = 0;
	while (fen[index] != ' ')
	{
		castlState[castlingCounter] = fen[index];
		castlingCounter++;
		index++;
	}
	if (!boardWriter.WriteCastlingState(castlState, castlingCounter, outputBoard))
	{
		printf("Syntax error in FEN string");
		return false;
	}
	
	index++;
	// should be on en passant now.
	if (fen[index] != '-')
	{
		byte file = fen[index] - 'a';
		index++;
		byte rank = fen[index] - '1';

		boardWriter.WriteEnPassant(file, rank, outputBoard);
	}

	index++;
	// now on ply moves or half moves.
	if (state != nullptr)
	{		
		const char* a = &fen[index];
		state->m_plyCounter = int(a - 'a');
		index += 2;
		
		a = &fen[index];
		state->m_moveCounter = int(a - 'a');
	}

	return true;
}
