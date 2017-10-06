#include "stdafx.h"
#include <ctype.h>
#include "Board.h"

using namespace GambitEngine;

Board::Board() :
	m_castleState(0),
	m_enPassant64(0)
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
	m_castleState = _src.m_castleState;
	m_enPassant64 = _src.m_enPassant64;

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
	
	m_castleState = 15; // all castling available
}

bool Board::Occupied(byte indx)
{
	return m_board[indx] & 0x00;
}

byte Board::GetBoard120Index(byte file, byte rank) const
{
	byte corrFile = tolower(file) - 'a';
	byte corrRank = rank - 1;

	// validate placement is inside the board.
	if (corrFile < 0 || corrFile > 7 || corrRank < 0 || corrRank > 7)
		return -1;

	byte index = corrFile + (corrRank << 3);
	return m_boardLookup[index];
}

byte Board::GetBoard64Index(byte file, byte rank) const
{
	byte corrFile = tolower(file) - 'a';
	byte corrRank = rank - 1;

	// validate placement is inside the board.
	if (corrFile < 0 || corrFile > 7 || corrRank < 0 || corrRank > 7)
		return -1;

	byte index = corrFile + (corrRank << 3);
	return index;
}

byte Board::EnPassant(byte sSqr, SET set, PIECE piece, byte tSqr)
{
	if (piece != PAWN)
	{
		// reset en passant since our move didn't add one.
		m_enPassant64 = 0;
		return m_enPassant64;
	}
	short enPassant = (short)tSqr - (short)sSqr;
	short absPass = abs(enPassant);

	if (m_enPassant64 != 0)
	{
		if (tSqr == m_enPassant64)
		{
			int captSet = !int(set);
			CapturePiece((SET)captSet, PAWN, m_enPassantTargetSqr64);
			m_enPassantTargetSqr64 = 0;
			m_enPassant64 = 0;
		}
	}
	
	if (absPass == 16)
	{
		enPassant /= 2;
		m_enPassant64 = sSqr + enPassant;
		
		m_enPassantTargetSqr64 = tSqr;
	}

	return m_enPassant64;
}

bool 
Board::Castling(byte sSqr, SET set, PIECE piece, byte tSqr)
{
	if (piece == ROOK)
	{
		// fetch rook square;
		byte rookByte = m_board[m_boardLookup[tSqr]];
		if (!(rookByte & 64))
		{
			rookByte |= 64; // set has moved;
			byte sFile = sSqr % 8;
			if(sFile == 7) // king side;
				m_castleState ^= 1 + (set * 3);
			else // assume we're moving queen side rook
				m_castleState ^= 2 + (set * 6);

			m_board[m_boardLookup[tSqr]] = rookByte;
		}
	}
	// was our piece a king?
	if (piece != KING)
		return false;
	
	// are we castling?
	short diff = sSqr - tSqr;
	if (diff != -2 && diff != 2)
	{
		/*if (m_castleState & (1 + (set * 2)))
			return false;*/

		// set castle state to always make sure we're unsetting it in the toggle step.
		m_castleState |= 1 + (set * 3);
		m_castleState |= 2 + (set * 6);

		m_castleState ^= 1 + (set * 3);
		m_castleState ^= 2 + (set * 6);
		return false;
	}

	byte sRank = sSqr >> 3;
	byte rookFile = 0;
	byte tFile = 3;
	if (diff < 0) // king side, get rook on file h.
	{
		rookFile = 7;
		tFile = 5;
	}	

	// move rook
	byte sRookSqr = rookFile + (sRank * 8);
	byte tRookSqr = tFile + (sRank * 8);
	// get rook on rookSqr
	Pieces::Piece* pRook = m_material[set].GetPiece(ROOK, sRookSqr);
	// make moves
	m_bitboard.MakeMove(pRook->Square8x8, set, (PIECE)pRook->Type, tRookSqr);
	pRook->Square8x8 = tRookSqr;
	pRook->Square10x12 = m_boardLookup[tRookSqr];
	m_material[set].MakeMove(pRook, sRookSqr);	
	
	m_board[m_boardLookup[tRookSqr]] = m_board[m_boardLookup[sRookSqr]];
	m_board[m_boardLookup[sRookSqr]] = 0x00;
	return true;
}

bool 
Board::PlacePiece(SET set, PIECE piece, byte file, byte rank)
{
	byte bIndx64 = GetBoard64Index(file, rank);
	byte bIndx = m_boardLookup[bIndx64];

	Pieces::Piece p;
	p.Type = piece;
	p.Square10x12 = bIndx;
	p.Square8x8 = bIndx64;

	
	byte comp = 0x07;
	if ((m_board[bIndx] & comp) != 0x00)
		return false;

	byte pieceVal = 0x00;
	pieceVal |= set << 7;
	pieceVal |= piece;

	m_board[bIndx] = pieceVal;

	m_bitboard.PlacePiece(set, piece, file, rank);
	m_material[set].AddPiece(p);
	return true;
}

bool 
Board::CapturePiece(SET set, PIECE piece, byte tSqr)
{	
	Pieces::Piece *p = m_material[set].GetPiece(piece, tSqr);

	if (p == nullptr)
		return false;
		
	m_board[m_boardLookup[tSqr]] = 0x00;
	m_material[set].CapturePiece(p);
	m_bitboard.CapturePiece(set, piece, tSqr);
	return true;
}

bool 
Board::MakeMove(byte sFile, byte sRank, byte tFile, byte tRank)
{		
	byte sInd64 = GetBoard64Index(sFile, sRank);
	byte sInd	= m_boardLookup[sInd64];

	byte tInd64 = GetBoard64Index(tFile, tRank);
	byte tInd	= m_boardLookup[tInd64];
	
	byte pieceByte = m_board[sInd] & 0x7;
	byte pieceSet = m_board[sInd] >> 7;

	u64 avaMoves = m_bitboard.AvailableMoves((SET)pieceSet, (PIECE)pieceByte, sInd64, m_enPassant64, m_castleState);
	u64 moveMsk = 1i64 << tInd64;

	if (avaMoves & moveMsk)
	{
		m_board[tInd] = m_board[sInd];
		m_board[sInd] = 0x00;

		Pieces::Piece *p = m_material[pieceSet].GetPiece((PIECE)pieceByte, sInd64);
		p->Square10x12 = tInd;
		p->Square8x8 = tInd64;
		m_bitboard.MakeMove(sInd64, (SET)pieceSet, (PIECE)pieceByte, tInd64);
		m_material[pieceSet].MakeMove(p, sInd64);

		EnPassant(sInd64, (SET)pieceSet, (PIECE)pieceByte, tInd64);
		Castling(sInd64, (SET)pieceSet, (PIECE)pieceByte, tInd64);
		return true;
	}

	return false;
}

byte 
Board::GetValue(byte file, byte rank) const
{
	byte bIndx = GetBoard120Index(file, rank);
	return  m_board[bIndx];	
}

u64 
Board::GetAttacked(SET set)
{
	return 1;
}
