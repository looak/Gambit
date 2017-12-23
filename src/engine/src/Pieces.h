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

static signed short MoveCount[NR_OF_PIECES] = {
	0, 2, 8, 4, 4, 8, 8
};

static bool Slides[NR_OF_PIECES] = {
	false, false, false, true, true, true, false,
};

static signed short Moves[NR_OF_PIECES][8] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ -21, -19, -12, -8, 8, 12, 19, 21 },
	{ -11, -9, 9, 11, 0, 0, 0, 0 },
	{ -10, -1, 1, 10, 0, 0, 0, 0 },
	{ -11, -10, -9, -1, 1, 9, 10, 11 },
	{ -11, -10, -9, -1, 1, 9, 10, 11 }
};

static signed short Moves0x88[NR_OF_PIECES][8] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ -16, -32, 0, 0, 0, 0, 0, 0 },
	{ -33, -31, -18, -14, 14, 18, 31, 33 },
	{ -17, -15, 15, 17, 0, 0, 0, 0 },
	{ -16, -1, 1, 16, 0, 0, 0, 0 },
	{ -17, -16, -15, -1, 1, 15, 16, 17 },
	{ -17, -16, -15, -1, 1, 15, 16, 17 }
};

static signed short Attacks0x88[NR_OF_PIECES][8] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ -15, -17, 0, 0, 0, 0, 0, 0 },
	{ -33, -31, -18, -14, 14, 18, 31, 33 },
	{ -17, -15, 15, 17, 0, 0, 0, 0 },
	{ -16, -1, 1, 16, 0, 0, 0, 0 },
	{ -17, -16, -15, -1, 1, 15, 16, 17 },
	{ -17, -16, -15, -1, 1, 15, 16, 17 }
};

struct Piece
{
	byte	Type;
	byte	Square10x12;
	byte	Square8x8;
};

static PIECE converter(byte toConvert)
{
	switch (toConvert)
	{
	case 'p':
		return PAWN;
	case 'n':
		return KNIGHT;
	case 'b':
		return BISHOP;
	case 'r':
		return ROOK;
	case 'q':
		return QUEEN;
	case 'k':
		return KING;
	default:
		return PAWN;
	}
}

static byte converter(PIECE toConvert)
{
	switch (toConvert)
	{
	case PAWN:
		return 'p';
	case KNIGHT:
		return 'n';
	case BISHOP:
		return 'b';
	case ROOK:
		return 'r';
	case QUEEN:
		return 'q';
	case KING:
		return 'k';
	default:
		return PAWN;
	}
}

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
