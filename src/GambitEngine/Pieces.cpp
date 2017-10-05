#include "stdafx.h"
#include "Pieces.h"
#include <algorithm>

using namespace GambitEngine;

Material::Material()
{
	for (int i = 0; i < NR_OF_PIECES; i++)
	{
		// will probably never have more than eight of each piece.
		m_materialGrid[i].reserve(8); 
		m_capturedMaterial[i].reserve(8);
	}

	for (int i = 0; i < 64; i++)
		m_board[i] = nullptr;
}

Material::~Material()
{
	for (int i = 0; i < NR_OF_PIECES; i++)
	{
		m_materialGrid[i].clear();
		m_capturedMaterial[i].clear();
	}

	for (int i = 0; i < 64; i++)
		m_board[i] = nullptr;
}

Pieces::Piece* 
Material::GetPiece(PIECE pType, byte square)
{
	if (pType == KING)
		return m_king;

	return m_board[square];
}

std::vector<Pieces::Piece> GambitEngine::Material::GetMaterial() const
{
	std::vector<Pieces::Piece> retVal;
	for (int i = 0; i < NR_OF_PIECES; i++)
		retVal.insert(retVal.end(), m_materialGrid[i].begin(), m_materialGrid[i].end());
	
	return retVal;
}

bool 
Material::AddPiece(Pieces::Piece piece)
{
	m_materialGrid[piece.Type].push_back(piece);
	m_board[piece.Square8x8] = &m_materialGrid[piece.Type].at(m_materialGrid[piece.Type].size() - 1);

	if (piece.Type == KING)
		m_king = m_board[piece.Square8x8];
	return true;
}

bool 
Material::CapturePiece(Pieces::Piece* piece)
{
	bool foundPiece = false;
	int ind = 0;
	while (m_materialGrid[piece->Type].size() > ind)
	{
		if (m_materialGrid[piece->Type].at(ind).Square8x8 == piece->Square8x8)
		{
			foundPiece = true;
			break;
		}
		ind++;
	}
	
	if (!foundPiece)
		return false;

	m_materialGrid[piece->Type].erase(m_materialGrid[piece->Type].begin() + ind);
	m_board[piece->Square8x8] = nullptr;
	m_capturedMaterial[piece->Type].push_back(*piece);
	return true;
}

bool 
Material::MakeMove(Pieces::Piece* piece, byte sSquare)
{
	m_board[sSquare] = nullptr;
	m_board[piece->Square8x8] = piece;
	return true;
}