#pragma once
#include "typedef.h"

namespace GambitEngine
{

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
	const MoveNode* m_parent;
	std::vector<MoveNode> m_children;

public:
	MoveNode(Move move, const MoveNode* parent, byte set, byte piece)
		: m_parent(parent)
		, m_move(move)
		, m_setByte(set)
		, m_pieceByte(piece)
	{
	}

	MoveNode* AddMoveNode(Move move, byte set, byte piece)
	{
		MoveNode node(move, this, set, piece);
		m_children.push_back(node);

		return &m_children[m_children.size() - 1];
	}

	const Move* getMove() { return &m_move; }
	const SET getSet() { return (SET)m_setByte; }
	const PIECE getPiece() { return (PIECE)m_pieceByte; }
};

}; // namespace GambitEngine