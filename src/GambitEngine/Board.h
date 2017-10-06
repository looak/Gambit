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

	// handles setting en passant, removing en passant square
	// and also if move is taking an enpassant that's also handled.
	bool EnPassant(byte sSqr, SET set, PIECE piece, byte tSqr);
	bool Castling(byte sSqr, SET set, PIECE piece, byte tSqr);

	Bitboard m_bitboard;
	byte m_board[120];
	byte m_boardLookup[64];

	byte m_enPassant64;
	byte m_enPassantTargetSqr64;

	// 0x01 == K, 0x02 == Q, 0x04 == k, 0x08 == q
	byte m_castleState;

	typedef std::array<Material, 2> MaterialArray;
	MaterialArray m_material;

public:
	Board();
	Board(const Board& _src);

	~Board();

	// attempts to place said piece of said set in the given file & rank.
	// returns false if this failed either because of being outside of board 
	// or square is occupied
	bool PlacePiece(SET set, PIECE piece, byte file, byte rank);
	bool CapturePiece(SET set, PIECE piece, byte tSqr);
	bool MakeMove(byte sFile, byte sRank, byte tFile, byte tRank);
	
	const std::vector<Pieces::Piece> GetPieces(SET set) const { return m_material[set].GetMaterial(); };

	byte GetValue(byte file, byte rank) const;
	Bitboard GetBitboard() const { return m_bitboard; }
	u64 GetAttacked(SET set);


	u64 AvailableMoves(SET set, PIECE piece, u32 square) {
		return m_bitboard.AvailableMoves(set, piece, square, m_enPassant64, m_castleState);
	}
	
};
}	
