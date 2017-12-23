#pragma once
#include "typedef.h"
#include "Pieces.h"

namespace GambitEngine
{

enum MoveFlags
{
	NONE = 0x0,
	CASTLING = 0x80,
	PROMOTION = 0x40,
};
struct Move
{
	byte fromSqr = 0x00;
	byte toSqr = 0x00;

	byte promotion = 0x00;
};

class MoveNode
{
private:
	Move m_move;
	byte m_pieceByte;
	byte m_setByte;
	byte m_state;
	MoveNode* m_parent;
	MoveNode* m_child;

public:
	// TODO() Maybe we could change this to some sort of undo stack?
	// Generic undo units, but might be that this is something for the GUI.

	MoveNode(Move move, MoveNode* parent, byte set, byte piece, byte state)
			: m_move(move)
			  , m_parent(parent)
			  , m_child(nullptr)
	{
		m_pieceByte = piece;
		m_setByte = set;
		m_state = state;
	}

	MoveNode* AddMoveNode(Move move, byte set, byte piece, byte state)
	{
		if(m_child != nullptr) {
			delete (m_child);
			m_child = nullptr;
		}
		m_child = new MoveNode(move, this, set, piece, state);
		return m_child;
	}

	const Move* getMove() { return &m_move; }
	const SET getSet() { return (SET)m_setByte; }
	const PIECE getPiece() { return (PIECE)m_pieceByte; }
	MoveNode* getParent() { return m_parent; }
	const byte getState() { return m_state; }
};

} // namespace GambitEngine