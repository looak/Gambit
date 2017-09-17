#pragma once

enum PIECE
{
	NONE = 0,
	PAWN = 1,
	BISHOP = 2,
	KNIGHT = 3,
	ROOK = 4,
	QUEEN = 5,
	KING = 6,
	NR_OF_PIECES = 7
};

enum SET
{
	WHITE = 0,
	BLACK = 1,
	NR_OF_COLORS = 2,
};

class Pieces
{
public:
	static signed short* GetMoves(PIECE piece, byte& count)
	{
		switch (piece)
		{
		case KING: {
			signed short moves[8] = { -11, -10, -9, -1, 1, 9, 10, 11 };
			auto mvsCpy = new signed short[8];
			count = sizeof(moves);
			memcpy(mvsCpy, moves, count);
			return mvsCpy;
		}
		default:
			return nullptr;
		}

	}
};