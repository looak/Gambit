#include "Material.h"
#include <ctype.h>
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

	m_king = nullptr;
	for (int i = 0; i < 64; i++)
		m_board[i] = nullptr;

	for (int i = 0; i < NR_OF_PIECES; i++)
	{
		for(int p = 0; p < m_materialGrid[i].size(); p ++)
		{
			m_board[m_materialGrid[i].at(p).Square8x8] = &m_materialGrid[i].at(p);
			
			if (i == KING)
				m_king = &m_materialGrid[i].at(0); // there can only ever be one king.... as of now.
		}		
	}
}

Piece*
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

Piece*
Material::GetPiece(byte square)
{
	auto ret = m_board[square];
	if (ret == nullptr)
		return ret;

	return ret;
}

const Piece*
Material::GetPiece(byte square) const
{
	auto ret = m_board[square];
	if (ret == nullptr)
		return ret;

	return ret;
}

bool
GambitEngine::Material::RemovePiece(const Piece* piece)
{
	bool foundPiece = false;
	unsigned int ind = 0;
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

	return true;
}
std::vector<Piece> 
GambitEngine::Material::GetMaterial() const
{
	std::vector<Piece> retVal;
	for (int i = 0; i < NR_OF_PIECES; i++)
		retVal.insert(retVal.end(), m_materialGrid[i].begin(), m_materialGrid[i].end());
	
	return retVal;
}

std::vector<Piece> 
GambitEngine::Material::GetCaptured() const
{
	std::vector<Piece> retVal;
	for (int i = 0; i < NR_OF_PIECES; i++)
		retVal.insert(retVal.end(), m_capturedMaterial[i].begin(), m_capturedMaterial[i].end());

	return retVal;
}


std::vector<Piece> 
GambitEngine::Material::GetMaterial(byte type) const
{
	return m_materialGrid[type];
}

bool 
Material::AddPiece(Piece piece)
{
	m_materialGrid[piece.Type].push_back(piece);
	m_board[piece.Square8x8] = &m_materialGrid[piece.Type].at(m_materialGrid[piece.Type].size() - 1);

	if (piece.Type == KING)
		m_king = m_board[piece.Square8x8];
	return true;
}

bool 
Material::CapturePiece(Piece piece)
{
	bool foundPiece = false;
	unsigned int ind = 0;
	while (m_materialGrid[piece.Type].size() > ind)
	{
		if (m_materialGrid[piece.Type].at(ind).Square8x8 == piece.Square8x8)
		{
			foundPiece = true;
			break;
		}
		ind++;
	}
	
	if (!foundPiece)
		return false;

	m_board[piece.Square8x8] = nullptr;
	m_materialGrid[piece.Type].erase(m_materialGrid[piece.Type].begin() + ind);
	m_capturedMaterial[piece.Type].push_back(piece);
	return true;
}

bool 
Material::MakeMove(byte sSqr, PIECE pType, byte tSqr, byte tSqr120)
{
	auto pP = GetPiece(pType, sSqr);
	if (pP == nullptr)
		return false;

	m_board[tSqr] = pP;
	m_board[sSqr] = nullptr;
	pP->Square8x8 = tSqr;
	pP->Square10x12 = tSqr120;

	return true;
}
