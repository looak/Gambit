#include <ctype.h>
#include <cstring>
#include <iostream>
#include <cassert>
#include "Board.h"
#include "Log.h"
#include "PieceDef.h"

using namespace GambitEngine;

Board::Board()
		: m_enPassant64(0)
		, m_enPassantTargetSqr64(0)
		, m_castleState(0)
		, m_rootNode(nullptr)
		, m_lastNode(nullptr)
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

	memcpy(&m_boardLookup[0], lookup, sizeof(lookup));
	
	ResetBoard();
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

	while (m_lastNode != nullptr)
	{
		auto tmpPtr = m_lastNode->getParent();
		delete m_lastNode;
		m_lastNode = tmpPtr;
	} 

	m_enPassant64 = 0;
	m_enPassantTargetSqr64 = 0;
	m_castleState = 0;
}

bool Board::Occupied(byte indx)
{
	return m_board[indx] & 0x00;
}

byte Board::GetBoard120Index(byte file, byte rank) const
{
	byte corrFile = (byte)(tolower(file) - 'a');
	byte corrRank = rank - 1;

	// validate placement is inside the board.
	if (corrFile > 7 || corrRank > 7)
		return (byte)-1;

	byte index = corrFile + (corrRank << 3);
	return m_boardLookup[index];
}

byte Board::GetBoard64Index(byte file, byte rank) const
{
	byte corrFile = (byte)(tolower(file) - 'a');
	byte corrRank = rank - 1;

	// validate placement is inside the board.
	if (corrFile > 7 || corrRank > 7)
		return (byte)-1;

	byte index = corrFile + (corrRank << 3);
	return index;
}

bool Board::EnPassant(byte sSqr, SET set, PIECE piece, byte tSqr, byte& state, byte& enPassantState)
{
	byte prevTarget = m_enPassantTargetSqr64;
	byte prevPassant = m_enPassant64;
	// reset en passant since our move didn't add one.
	m_enPassant64 = 0;
	m_enPassantTargetSqr64 = 0;
	if (piece != PAWN)
		return false;

	short enPassant = (short)tSqr - (short)sSqr;
	short absPass = (short)abs(enPassant);

	if (prevPassant != 0)
	{
		if (tSqr == prevPassant)
		{
			int captSet = !int(set);
			CapturePiece((SET)captSet, PAWN, prevTarget, state);
			m_enPassantTargetSqr64 = 0;
			m_enPassant64 = 0;
			enPassantState |= 128;
			return true;
		}
	}
	
	if (absPass == 16)
	{
		enPassant /= 2;
		assert(enPassant < INT8_MAX || enPassant > INT8_MIN);
		m_enPassant64 = sSqr + (byte)enPassant;
		// to be able to solve capture
		m_enPassantTargetSqr64 = tSqr;
		enPassantState = m_enPassant64;
		return true;
	}

	return false;
}

bool 
Board::Castling(byte sSqr, SET set, PIECE piece, byte tSqr)
{
	// so we later can store it in our MoveNode.
	byte prevCastleState = m_castleState;

	if (piece == ROOK)
	{ 
		// remove castling ability if we move the rook out of origin
		byte sFile = sSqr % 8;
		if(sFile == 7)  // king side;
			m_castleState &= ~(1 + (set * 3));
		else if(sFile == 0) // queen side 
			m_castleState &= ~(2 + (set * 6));
	}

	if (piece != KING)
		return false;
	
	// are we castling?
	short diff = sSqr - tSqr;
	if (diff != -2 && diff != 2)
	{
		// set castle state to always make sure we're unsetting it in the toggle step.
		m_castleState |= 1 + (set * 3);
		m_castleState |= 2 + (set * 6);

		// toggle castle state
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
	
	// make moves
	m_bitboard.MakeMove(sRookSqr, set, ROOK, tRookSqr);
	m_material[set].MakeMove(sRookSqr, ROOK, tRookSqr, m_boardLookup[tRookSqr]);
	
	m_board[m_boardLookup[tRookSqr]] = m_board[m_boardLookup[sRookSqr]];
	m_board[m_boardLookup[sRookSqr]] = 0x00;

	RegisterMove(sRookSqr, set, ROOK, tRookSqr, prevCastleState);
	return true;
}

bool 
Board::Promote(byte sqr, SET set, byte promoteTo)
{
	PIECE pType = PieceDef::converter(promoteTo);
	m_material[set].PromotePiece(pType, sqr);
	m_bitboard.Promote(set, pType, sqr);
	byte newBoardByte = pType;
	newBoardByte |= set << 7;
	m_board[m_boardLookup[sqr]] = newBoardByte;
	return true;
}

bool 
Board::Check(SET set)
{
	// are we in check
	auto pKing = m_material[set].GetKing();
	SET opSet = (SET)!(int)set;

	if(pKing == nullptr)
		return false;

	return m_bitboard.IsSquareAttacked(opSet, pKing->Square8x8);
}

bool 
Board::CheckMate(SET set)
{
	if(!Check(set))
		return false;

	auto pKing = m_material[set].GetKing();
	SET opSet = (SET)!(int)set;

	// can we move king?
	u64 attked = m_bitboard.Attacked(opSet, true);
	u64 moves = m_bitboard.AvailableMovesSimple(set, (PIECE)pKing->Type, pKing->Square8x8);

	// find diagonals from king
	u64 digs = m_bitboard.AvailableMovesSimple(set, QUEEN, pKing->Square8x8);
		
	u64 attkedMask = ~attked;
	u64 avaMoves = attkedMask & moves;
	u64 attkedDiagnoals = attked & digs;

	// can king move
	if (avaMoves != 0)
		return false;

	// can we block or capture us out of check mate?
	auto material = m_material[set].GetMaterial();
	for (u32 i = 0; i < material.size(); i++)
	{
		byte promotion = 0;
		const Piece* pP = material.at(i);
		if(pP->Type == 6)
			continue;

		moves = m_bitboard.AvailableMoves(set, (PIECE)pP->Type, pP->Square8x8, m_enPassant64, m_castleState, promotion);
			
		avaMoves = attkedDiagnoals & moves;
		if (avaMoves > 0)
			return false;
	}
		
	return true;
}

bool 
Board::PlacePiece(SET set, PIECE piece, byte file, byte rank)
{
	byte bIndx64 = GetBoard64Index(file, rank);
	return PlacePiece(set, piece, bIndx64);
}

bool Board::PlacePiece(SET set, PIECE piece, byte square)
{
	byte sqr120 = m_boardLookup[square];

	Piece p;
	p.Type = piece;
	p.Square10x12 = sqr120;
	p.Square8x8 = square;

	byte comp = 0x07;
	if ((m_board[sqr120] & comp) != 0x00)
		return false;

	byte pieceVal = 0x00;
	pieceVal |= set << 7;
	pieceVal |= piece;

	m_board[sqr120] = pieceVal;

	m_bitboard.PlacePiece(set, piece, square);
	m_material[set].AddPiece(p);
	return true;
}

bool 
Board::CapturePiece(SET set, PIECE pType, byte tSqr, byte& state)
{
	state |= CAPTURE;	
	state |= pType;

	m_board[m_boardLookup[tSqr]] = 0x00;
	m_material[set].CapturePiece(pType, tSqr);
	m_bitboard.CapturePiece(set, pType, tSqr);

	if (pType == ROOK) // we need to modify castling
	{
		if (tSqr == 63) // black king side
		{
			m_castleState |= 4;
			m_castleState ^= 4;
		}
		else if (tSqr == 56)
		{
			m_castleState |= 8;
			m_castleState ^= 8;
		}
		else if (tSqr == 7)
		{
			m_castleState |= 1;
			m_castleState ^= 1;
		}
		else if (tSqr == 0)
		{
			m_castleState |= 2;
			m_castleState ^= 2;
		}
	}

	return true;
}

bool 
Board::MakeMove(byte sFile, byte sRank, byte tFile, byte tRank, byte promote)
{		
	byte sInd64 = GetBoard64Index(sFile, sRank);
	byte tInd64 = GetBoard64Index(tFile, tRank);

	if(sInd64 > 64 || tInd64 > 64)
		return false;

	return MakeMove(sInd64, tInd64, promote);
}

bool Board::MakeMove(byte sSqr, byte tSqr, byte promotion)
{
	byte sSqr120 = m_boardLookup[sSqr];

	byte pieceByte = m_board[sSqr120] & (byte)0x7;
	byte pieceSet = m_board[sSqr120] >> 7;

	byte temp = 0x00;
	u64 avaMoves = m_bitboard.AvailableMoves((SET)pieceSet, (PIECE)pieceByte, sSqr, m_enPassant64, m_castleState, temp);
	u64 moveMsk = UINT64_C(1) << tSqr;

	if (avaMoves & moveMsk)
		return MakeLegalMove(sSqr, tSqr, promotion);
	
	return false;
}
bool
Board::MakeLegalMove(byte sSqr, byte tSqr, byte promote)
{
	byte state = 0x0;
	byte capturedPiece = 0x0;
	byte enPassantState = 0x0;
	byte sSqr120 = m_boardLookup[sSqr];
	byte tSqr120 = m_boardLookup[tSqr];

	byte pieceByte = m_board[sSqr120] & 0x7;
	byte pieceSet = m_board[sSqr120] >> 7;

	bool isCapture = false;
	byte targetPiece = m_board[tSqr120] & 0x7;
	if ((targetPiece & 0x7) != 0x0)
		isCapture = true;

	// store previous castling state
	state |= m_castleState;

	EnPassant(sSqr, (SET)pieceSet, (PIECE)pieceByte, tSqr, capturedPiece, enPassantState);

	if (isCapture)
		CapturePiece((SET)!(int)pieceSet, (PIECE)targetPiece, tSqr, capturedPiece);
		
	bool castled = Castling(sSqr, (SET)pieceSet, (PIECE)pieceByte, tSqr);

	m_bitboard.MakeMove(sSqr, (SET)pieceSet, (PIECE)pieceByte, tSqr);
	if (!m_material[pieceSet].MakeMove(sSqr, (PIECE)pieceByte, tSqr, tSqr120))
	{
		std::cout << "[    OUTPUT] Board::MakeLegalMove failed moving " << Log::tooChar(m_board[sSqr120]) << " from:" << (int)sSqr << " to:" << (int)tSqr << std::endl;
		return false;
	}

	if (castled)
	{
		if (pieceSet == 0)
			m_castleState ^= 3;
		else
			m_castleState ^= 12;

		state = 1 << 7;
	}

	// do move
	m_board[tSqr120] = m_board[sSqr120];
	m_board[sSqr120] = 0x00;

	if (promote != 0x00)
	{
		Promote(tSqr, (SET)pieceSet, promote);
		state |= PROMOTION;
	}

	return RegisterMove(sSqr, (SET)pieceSet, (PIECE)pieceByte, tSqr, state, enPassantState, capturedPiece);
}

bool 
Board::MakeLegalMove(byte sFile, byte sRank, byte tFile, byte tRank, byte promote)
{
	byte sInd64 = GetBoard64Index(sFile, sRank);	
	byte tInd64 = GetBoard64Index(tFile, tRank);
	return MakeLegalMove(sInd64, tInd64, promote);
}

byte 
Board::GetValue(byte file, byte rank) const
{
	byte bIndx = GetBoard120Index(file, rank);
	return  m_board[bIndx];	
}

byte
Board::GetValue(byte sqr) const
{
	byte bIndx = m_boardLookup[sqr];
	return  m_board[bIndx];
}

byte
Board::GetCastlingState() const
{
	return m_castleState;
}

byte
Board::GetEnPassantState() const
{
	return m_enPassantTargetSqr64;
}

bool Board::UnmakeMove()
{
	const Move* mv = m_lastNode->getMove();
	byte tSqr120 = m_boardLookup[mv->fromSqr];
	byte sSqr120 = m_boardLookup[mv->toSqr];

	// reset promoted piece
	if(m_lastNode->getState() & PROMOTION)
	{
		byte newBoardByte = 0x01;
		newBoardByte |= (m_lastNode->getSet() << 7);

		m_board[sSqr120] = newBoardByte;
		const Piece* pP = m_material[m_lastNode->getSet()].GetPiece(mv->toSqr);
		byte pieceType = pP->Type;
		m_material[m_lastNode->getSet()].DemotePiece((PIECE)pP->Type, mv->toSqr);
		m_bitboard.Demote(m_lastNode->getSet(), (PIECE)pieceType, mv->toSqr);
	}

	m_bitboard.MakeMove(mv->toSqr, m_lastNode->getSet(), m_lastNode->getPiece(), mv->fromSqr);

	m_material[m_lastNode->getSet()].MakeMove(mv->toSqr, m_lastNode->getPiece(), mv->fromSqr, tSqr120);

	m_board[tSqr120] = m_board[sSqr120];
	m_board[sSqr120] = 0x00;

	if(m_lastNode->getParent() == nullptr)
		m_rootNode = nullptr;

	auto prevLast = m_lastNode;
	m_lastNode = m_lastNode->getParent();

	m_castleState = prevLast->getState() & 15;

	// TODO() change this to something more generic.
	if (prevLast->getState() == 128) // 128 is flag for castling
		UnmakeMove();
	else if (prevLast->getCapturedPiece() & CAPTURE)
	{
		byte sqrToPlace = mv->toSqr;
		if(prevLast->getEnPassantState() & 128)
			sqrToPlace = m_lastNode->getMove()->toSqr;

		byte piece = prevLast->getCapturedPiece() & 7;
		SET opSet = (SET)!prevLast->getSet();
		m_material[opSet].UncapturePiece((PIECE)piece, sqrToPlace);
		PlacePiece(opSet, (PIECE)piece, sqrToPlace);
	}

	// should always set enPassantSqr if there was one
	if(m_lastNode != nullptr)
	{	
		m_enPassantTargetSqr64 = m_lastNode->getMove()->toSqr;
		m_enPassant64 = m_lastNode->getEnPassantState();
	}
	else
	{
		m_enPassant64 = 0;
		m_enPassantTargetSqr64 = 0;
	}
	return true;
}

bool Board::RegisterMove(byte sSqr, SET set, PIECE piece, byte tSqr, byte state, byte enPassantState, byte capturedPiece)
{
	Move move;
	move.fromSqr = sSqr;
	move.toSqr = tSqr;

	if(m_rootNode == nullptr)
	{
		m_rootNode = new MoveNode(move, nullptr, set, piece, state, enPassantState, capturedPiece);
		m_lastNode = m_rootNode;
	}
	else
	{
		m_lastNode = m_lastNode->AddMoveNode(move, set, piece, state, enPassantState, capturedPiece);
	}

	return true;
}

bool Board::Legal()
{
	if(Check(BLACK))
		return false;

	return !Check(WHITE);
}

bool Board::Legal(SET perspective)
{
	if(Check(perspective))
		return false;

	return true;
}

const MoveNode*
Board::GetLastMove()
{
	return m_lastNode;
}