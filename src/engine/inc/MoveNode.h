#pragma once
#include "typedef.h"
#include "Material.h"

namespace GambitEngine
{

enum MoveFlags
{
	NONE = 0x0,
	CASTLING = 0x80,
	PROMOTION = 0x40,
	CAPTURE = 0x20,
};

struct Move
{
	byte fromSqr = 0x00;
	byte toSqr = 0x00;

	byte promotion = 0x00;
	
	/* capture = 1,
	*  promotion = 2,
	*  en passant 4,
	*  castle = 8,
	*  check = 64,	
	*  checmate = 128
	*/
	byte flags = 0x00;
	char str[4];

	bool const operator == (const Move &rhs) const
	{
        return fromSqr == rhs.fromSqr 
			&& toSqr == rhs.toSqr
			&& promotion == rhs.promotion
			&& flags == rhs.flags;
    }

    bool const operator < (const Move &rhs) const
	{
        return fromSqr < rhs.fromSqr || (fromSqr == rhs.fromSqr && toSqr < rhs.toSqr);
    }

	std::string toString()
	{
		byte sFile = fromSqr % 8;
		byte sRank = fromSqr / 8;
		str[0] = sFile + 'a';
		str[1] = sRank + '1';
		byte tFile = toSqr % 8;
		byte tRank = toSqr / 8;
		str[2] = tFile + 'a';
		str[3] = tRank + '1';
		return std::string(str);
	}
};

class MoveNode
{
private:
	Move m_move;
	byte m_pieceByte;
	byte m_setByte;
	byte m_state;
	byte m_capturedPiece;
	byte m_enPassantState;
	MoveNode* m_parent;
	MoveNode* m_child;

public:
	// TODO() Maybe we could change this to some sort of undo stack?
	// Generic undo units, but might be that this is something for the GUI.

	MoveNode(Move move, MoveNode* parent, byte set, byte piece, byte state, byte enPassant, byte capturedPiece)
			: m_move(move)
			  , m_parent(parent)
			  , m_child(nullptr)
	{
		m_pieceByte = piece;
		m_setByte = set;
		m_state = state;
		m_enPassantState = enPassant;
		m_capturedPiece = capturedPiece;
	}

	MoveNode* AddMoveNode(Move move, byte set, byte piece, byte state, byte enPassant, byte capturedPiece)
	{
		if(m_child != nullptr) {
			delete (m_child);
			m_child = nullptr;
		}
		m_child = new MoveNode(move, this, set, piece, state, enPassant, capturedPiece);
		return m_child;
	}

	const Move* getMove() const		{ return &m_move; }
	const SET getSet() 				{ return (SET)m_setByte; }
	const PIECE getPiece() const	{ return (PIECE)m_pieceByte; }
	MoveNode* getParent() const 	{ return m_parent; }
	const byte getState() const 	{ return m_state; }
	const byte getEnPassantState() const { return m_enPassantState; }
	const byte getCapturedPiece()  const { return m_capturedPiece; }
};

} // namespace GambitEngine