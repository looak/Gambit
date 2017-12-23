#pragma once
#include "typedef.h"
#include <vector>
#include <array>

enum PIECE
{
	NONE = 0,
	PAWN = 1,
	KNIGHT = 2,
	BISHOP = 3,
	ROOK = 4,
	QUEEN = 5,
	KING = 6,
	NR_OF_PIECES =7,
};

enum SET
{
	WHITE = 0,
	BLACK = 1,
	NR_OF_SETS = 2,
};

namespace Pieces
{

struct Piece
{
	byte	Type;
	byte	Square10x12;
	byte	Square8x8;
};
}

namespace GambitEngine
{
class Material
{
private:
	typedef std::array<std::vector<Pieces::Piece>,NR_OF_PIECES> MaterialGrid;	
	MaterialGrid m_materialGrid;
	MaterialGrid m_capturedMaterial;

	Pieces::Piece* m_king;

	Pieces::Piece* m_board[64];

public:
	Material();
	~Material();
	Material (const Material& _src);

	Pieces::Piece* GetPiece(byte square);
	Pieces::Piece* GetPiece(PIECE pType, byte square);
	Pieces::Piece* GetKing() { return m_king; };

	std::vector<Pieces::Piece> GetMaterial() const;

	bool AddPiece(Pieces::Piece piece);
	bool CapturePiece(Pieces::Piece* piece);
	bool MakeMove(byte sSqr, PIECE pType, byte tSqr, byte tSqr120);
};
}
