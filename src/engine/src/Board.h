#pragma once

#include "typedef.h"
#include "MoveNode.h"
#include "Bitboard.h"
#include <array>
#include <vector>

// 0xX1 - PAWN
// 0xX2 - KNIGHT
// 0xX3 - BISHOP
// 0xX4 - ROOK
// 0xX4 - QUEEN
// 0xX4 - KING

// 0x0X - WHITE
// 0x8X - BLACK

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

// Piece Byte
//[set][moved flag][not used][not used][not used][piece t][piece t][piece t]

namespace GambitEngine
{

class GAMBIT_API Board
{
friend class FENBoardWriter;
private:
	bool Occupied(byte indx);

	byte GetBoard120Index(const byte file, const byte rank) const;
	byte GetBoard64Index(byte file, byte rank) const;

	// handles setting en passant, removing en passant square
	// and also if move is taking an enpassant that's also handled.
	bool EnPassant(byte sSqr, SET set, PIECE piece, byte tSqr, byte& state, byte& enPassant);
	bool Castling(byte sSqr, SET set, PIECE piece, byte tSqr);
	bool Promote(byte sqr, SET set, byte promoteTo);

	bool RegisterMove(byte sSqr, SET set, PIECE piece, byte tSqr, byte state = 0x0, byte enPassantState = 0x0, byte capturedPiece = 0x0);

	Bitboard m_bitboard;

	byte m_board[120];
	byte m_boardLookup[64];

	byte m_enPassant64;
	byte m_enPassantTargetSqr64;

	// 0x01 == K, 0x02 == Q, 0x04 == k, 0x08 == q
	byte m_castleState;

	typedef std::array<Material, 2> MaterialArray;
	MaterialArray m_material;

	MoveNode* m_rootNode;
	MoveNode* m_lastNode;

public:
	Board();
	Board(const Board& _src);

	~Board();

	// resets board to be empty;
	void ResetBoard();

	// attempts to place said piece of said set in the given file & rank.
	// returns false if this failed either because of being outside of board 
	// or square is occupied
	bool PlacePiece(SET set, PIECE piece, byte file, byte rank);
	bool PlacePiece(SET set, PIECE piece, byte square);

	bool CapturePiece(SET set, PIECE piece, byte tSqr, byte& state);
	bool MakeMove(byte sFile, byte sRank, byte tFile, byte tRank, byte promotion = 0x00);
	bool MakeMove(byte sSqr, byte tSqr, byte promotion = 0x00);
	// Unmakes last move.
	bool UnmakeMove();

	bool MakeLegalMove(byte sFile, byte sRank, byte tFile, byte tRank, byte promotion = 0x00);
	bool MakeLegalMove(byte sSqr, byte tSqr, byte promote = 0x00);
	
	bool Check(SET set);
	bool CheckMate(SET set);

	u64 AvailableMoves(SET set, PIECE piece, u32 square, byte& promotion) { return m_bitboard.AvailableMoves(set, piece, square, m_enPassant64, m_castleState, promotion); };

	byte GetValue(const byte file, const byte rank) const;
	Bitboard GetBitboard() const { return m_bitboard; }
	const std::vector<Piece> GetPieces(SET set) const { return m_material[set].GetMaterial(); };
	void SetCastlingRights(byte castlingByte) { m_castleState = castlingByte; };

	bool Legal();

};
}	
