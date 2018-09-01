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
	byte	Type = 0;
	byte	Square10x12 = 0;
	byte	Square8x8 = 0;
	bool	Captured = false;
};

class Material
{
private:

	typedef std::array<std::vector<Piece*>,NR_OF_PIECES> MaterialGrid;
	MaterialGrid m_materialGrid;
	MaterialGrid m_capturedMaterialGrid;

	std::vector<Piece> m_material;

	Piece* m_king;
	Piece* m_board[64];

	Piece* GetPiece(PIECE pType, byte square);
	bool ChangeTypeOfPiece(PIECE sType, PIECE tType, byte sqr);

public:
	Material();
	~Material();
	Material (const Material& _src);

	bool RemovePiece(const Piece* piece);
	const Piece* GetPiece(byte square) const;
	const Piece* GetPiece(PIECE pType, byte square) const;
	const Piece* GetKing() const { return m_king; };

	const std::vector<const Piece*> GetMaterial() const;
	const std::vector<const Piece*> GetMaterial(byte type) const;
	const std::vector<const Piece*> GetCaptured() const;

	bool AddPiece(Piece piece);
	bool CapturePiece(PIECE pType, byte tSqr);
	bool UncapturePiece(PIECE pType, byte tSqr);
	bool MakeMove(byte sSqr, PIECE pType, byte tSqr, byte tSqr120);
	bool DemotePiece(PIECE pType, byte tSqr);
	bool PromotePiece(PIECE pType, byte tSqr);
	

};
}
