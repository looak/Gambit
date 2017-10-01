#pragma once

#include "typedef.h"
#include "Pieces.h"

#include "Bitboard.h"
#include <array>
#include <vector>

//	FF FF FF FF FF FF FF FF FF FF
//	FF FF FF FF FF FF FF FF FF FF
//	FF 04 02 03 05 06 03 02 04 FF
//	FF 01 01 01 01 01 01 01 01 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 81 81 81 81 81 81 81 81 FF
//	FF 84 82 83 85 86 83 82 84 FF
//	FF FF FF FF FF FF FF FF FF FF
//	FF FF FF FF FF FF FF FF FF FF

//	FF FF FF FF FF FF FF FF FF FF
//	FF FF FF FF FF FF FF FF FF FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF FF FF FF FF FF FF FF FF FF
//	FF FF FF FF FF FF FF FF FF FF

namespace GambitEngine
{

class GAMBIT_API Board
{
friend class FENBoardWriter;
private:
	// resets board to be empty;
	void ResetBoard();
	bool Occupied(byte indx);

	byte GetBoard120Index(byte file, byte rank) const;
	byte GetBoard64Index(byte file, byte rank) const;

	byte EnPassant();

	Bitboard m_bitboard;
	byte m_board[120];
	byte m_boardLookup[64];

	byte m_enPassant;

	// 0x01 == K, 0x02 == Q, 0x04 == k, 0x08 == q
	byte m_castleState;

	typedef std::array<std::vector<Pieces::Piece>, 2> PieceArray;
	PieceArray m_pieceArray;

	Pieces::Piece m_kings[2];
public:
	Board();
	Board(const Board& _src);

	~Board();

	// attempts to place said piece of said set in the given file & rank.
	// returns false if this failed either because of being outside of board 
	// or square is occupied
	bool PlacePiece(SET set, PIECE piece, byte file, byte rank);
	bool MakeMove(byte sFile, byte sRank, byte tFile, byte tRank);

	const std::vector<Pieces::Piece> GetPieces(SET set) const { return m_pieceArray[set]; };

	byte GetValue(byte file, byte rank) const;
	Bitboard GetBitboard() const { return m_bitboard; }
	u64 GetAttacked(SET set);
	
};
}	
