#include "AlgebraicNotation.h"

using namespace GambitEngine;

char valueToPiece(byte value)
{
	switch (value)
	{
	case KNIGHT:
		return 'N';
	case BISHOP:
		return 'B';
	case ROOK:
		return 'R';
	case QUEEN:
		return 'Q';
	case KING:
		return 'K';
	default:
		return 'X';	
	}
}

void
Notation::sqrToNotation(byte sqr, char* out)
{
	byte file = sqr % 8;
	byte rank = sqr / 8;

	out[0] = file + 'a';
	out[1] = rank + '1';	
}

void
Notation::ConvertMove(Move& move)
{
	sqrToNotation(move.fromSqr, &move.str[0]);
	sqrToNotation(move.toSqr, &move.str[2]);
}

std::vector<std::string> 
Notation::ConvertMoves(std::vector<Move> moves, const Board& boardRef)
{
	std::vector<std::string> retValue;
	std::vector<Move>::const_iterator itr = moves.begin();
	while(itr != moves.end())
	{
		std::string notation;
		byte value = boardRef.GetValue(itr->fromSqr);
		
		if(value != PAWN)
			notation += valueToPiece(value);

		char square[2];
		sqrToNotation(itr->toSqr, &square[0]);
		notation.append(&square[0], 2);

		retValue.push_back(notation);
		itr++;
	}

	return retValue;
}
