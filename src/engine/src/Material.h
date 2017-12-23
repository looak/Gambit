#pragma once

#include "PieceDef.h"
#include "typedef.h"
#include <vector>
#include <array>
////////////////////////////////////////////////////////////////

namespace GambitEngine
{
////////////////////////////////////////////////////////////////

struct Piece
{
	byte	Type;
	byte	Square10x12;
	byte	Square8x8;
};

class Material
{
private:
	typedef std::array<std::vector<Piece>,NR_OF_PIECES> MaterialGrid;
	MaterialGrid m_materialGrid;
	MaterialGrid m_capturedMaterial;

	Piece* m_king;

	Piece* m_board[64];

public:
	Material();
	~Material();
	Material (const Material& _src);

	Piece* GetPiece(byte square);
	Piece* GetPiece(PIECE pType, byte square);
	Piece* GetKing() { return m_king; };

	std::vector<Piece> GetMaterial() const;

	bool AddPiece(Piece piece);
	bool CapturePiece(Piece* piece);
	bool MakeMove(byte sSqr, PIECE pType, byte tSqr, byte tSqr120);
};
}
