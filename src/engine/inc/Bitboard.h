#pragma once
#include "typedef.h"
#include "Material.h"
#include "MoveNode.h"

namespace GambitEngine
{

class GAMBIT_API Bitboard
{
private:
	void CalculateAttacked(SET set, bool ignoreKing = false);
	void CalculatePotentialPinns(SET set);
	PIECE GetPieceOnSquare(SET set, int square);
	void AddAttackedFrom(SET set, PIECE piece, int square, u64 matCombedOp);
	
	u64 AvailableCastling(SET set, byte castling);

	void MarkDirty(SET set);

	bool m_combMaterialDirty[NR_OF_SETS];
	bool m_attackedDirty[NR_OF_SETS];

	u64 m_material[NR_OF_SETS][NR_OF_PIECES];

	u64 m_materialCombined[NR_OF_SETS];
	u64 m_attacked[NR_OF_SETS];
	u64 m_pinned[NR_OF_SETS];
	bool m_pinnedDirty[NR_OF_SETS];
	u64 m_perPieceAttack[NR_OF_SETS][NR_OF_PIECES];

	byte m_board0x88[128];
	

public:
	Bitboard();
	Bitboard(const Bitboard& _src);

	~Bitboard();
		
	bool PlacePiece(SET set, PIECE piece, byte file, byte rank);
	bool PlacePiece(SET set, PIECE piece, byte tSqr);
	bool CapturePiece(SET set, PIECE piece, byte tSqr);
	bool MakeMove(byte sSqr, SET set, PIECE piece, byte tSqr);
	bool MakeMove(Move move, SET set, PIECE piece);
	bool Promote(SET set, PIECE toPiece, byte sqr);
	bool Demote(SET set, PIECE fromPiece, byte sqr);

	u64 AvailableMoves(SET set, PIECE piece, u32 square, byte enPassant, byte castling, byte& promotion);
	u64 AvailableMovesSimple(SET set, PIECE piece, byte square, byte mvMod = 1, byte enPassant = 0x0);

	u64 MaterialCombined(SET set);
	u64 MatCombinedNoKing(SET set);
	u64 Attacked(SET set, bool ignoreKing = false);

	bool IsSquareAttacked(SET opSet, byte sqr);

	void Clear();

};
}

