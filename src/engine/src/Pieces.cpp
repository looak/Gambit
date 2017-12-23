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

Material::Material(const Material & _src)
{
	for (int i = 0; i < NR_OF_PIECES; i++)
	{
		m_materialGrid[i] = _src.m_materialGrid[i];
		m_capturedMaterial[i] = _src.m_capturedMaterial[i];
	}


	for (int i = 0; i < NR_OF_PIECES; i++)
	{
		for (Pieces::Piece mat : m_materialGrid[i])
		{
			m_board[mat.Square8x8] = &mat;
		}
	}
	
}

Pieces::Piece* 
Material::GetPiece(PIECE pType, byte square)
{
	if (pType == KING)
		return m_king;

	auto ret = m_board[square];
	if (ret == nullptr)
		return ret;
	if (ret->Type != pType)
		return nullptr;
	
	return ret;
}

Pieces::Piece*
Material::GetPiece(byte square)
{
	auto ret = m_board[square];
	if (ret == nullptr)
		return ret;

	return ret;
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
Material::MakeMove(byte sSqr, PIECE pType, byte tSqr, byte tSqr120)
{
	auto pP = GetPiece(pType, sSqr);
	if (pP == nullptr)
		return false;

	m_board[sSqr] = nullptr;
	m_board[tSqr] = pP;
	pP->Square8x8 = tSqr;
	pP->Square10x12 = tSqr120;

	return true;
}
