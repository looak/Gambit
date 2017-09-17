#include "stdafx.h"
#include <ctype.h>
#include "Board.h"

using namespace GambitEngine;

Board::Board()
{
	// initialize lookup
	byte lookup[64] = {
		21, 22, 23, 24, 25, 26, 27, 28,
		31, 32, 33, 34, 35, 36, 37, 38,
		41, 42, 43, 44, 45, 46, 47, 48,
		51, 52, 53, 54, 55, 56, 57, 58,
		61, 62, 63, 64, 65, 66, 67, 68,
		71, 72, 73, 74, 75, 76, 77, 78,
		81, 82, 83, 84, 85, 86, 87, 88,
		91, 92, 93, 94, 95, 96, 97, 98};
	
	memcpy_s(&m_boardLookup[0], sizeof(m_boardLookup), lookup, sizeof(lookup));

	ResetBoard();
}

Board::Board(const Board & _src)
{
	memcpy(m_board, _src.m_board, sizeof(_src.m_board));
	memcpy(m_boardLookup, _src.m_boardLookup, sizeof(_src.m_boardLookup));
}


Board::~Board()
{
}

void
Board::ResetBoard()
{
	// actual board, 00 is a empty place.
	memset(&m_board[21], 0, 8);
	memset(&m_board[31], 0, 8);
	memset(&m_board[41], 0, 8);
	memset(&m_board[51], 0, 8);
	memset(&m_board[61], 0, 8);
	memset(&m_board[71], 0, 8);
	memset(&m_board[81], 0, 8);
	memset(&m_board[91], 0, 8);

	// out of board is represented with 0xff.
	memset(&m_board[0], 0xff, 21);
	memset(&m_board[29], 0xff, 2);
	memset(&m_board[39], 0xff, 2);
	memset(&m_board[49], 0xff, 2);
	memset(&m_board[59], 0xff, 2);
	memset(&m_board[69], 0xff, 2);
	memset(&m_board[79], 0xff, 2);
	memset(&m_board[89], 0xff, 2);
	memset(&m_board[99], 0xff, 21);
}

byte GambitEngine::Board::GetBoardIndex(byte file, byte rank)
{
	byte corrFile = tolower(file) - 'a';
	byte corrRank = rank - 1;

	// validate placement is inside the board.
	if (corrFile < 0 || corrFile > 7 || corrRank < 0 || corrRank > 7)
		return -1;

	byte index = corrFile + (corrRank << 3);
	return m_boardLookup[index];
}

bool 
Board::PlacePiece(SET set, PIECE piece, byte file, byte rank)
{
	byte bIndx = GetBoardIndex(file, rank);

	byte comp = 0x07;
	if ((m_board[bIndx] & comp) != 0x00)
		return false;

	byte pieceVal = 0x00;
	pieceVal |= set << 7;
	pieceVal |= piece;

	m_board[bIndx] = pieceVal;

	return true;
}

bool 
Board::MovePiece(byte sFile, byte sRank, byte tFile, byte tRank)
{
	byte sInd = GetBoardIndex(sFile, sRank);

	return false;
}

byte GambitEngine::Board::GetValue(byte file, byte rank)
{
	byte bIndx = GetBoardIndex(file, rank);
	return  m_board[bIndx];	
}
