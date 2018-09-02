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
		m_capturedMaterialGrid[i].reserve(8);

		if (i > 0)
		{
			for (int mat = 0; mat < 8; mat++)
			{
				Piece p;
				p.Type = i;
				p.Square8x8 = 0;
				p.Square10x12 = 0;
				m_material.push_back(p);
			}
		}
	}

	for (int i = 0; i < 64; i++)
		m_board[i] = nullptr;

	m_king = nullptr;
}

Material::~Material()
{
	for (int i = 0; i < NR_OF_PIECES; i++)
	{
		m_materialGrid[i].clear();
		m_capturedMaterialGrid[i].clear();
		m_material.clear();
	}

	for (int i = 0; i < 64; i++)
		m_board[i] = nullptr;

	m_king = nullptr;
}

Material::Material(const Material & _src)
{
	for (int i = 0; i < NR_OF_PIECES; i++)
	{
		m_materialGrid[i] = _src.m_materialGrid[i];
		m_capturedMaterialGrid[i] = _src.m_capturedMaterialGrid[i];
	}

	for (int i = 0; i < _src.m_material.size(); i++)
		m_material.push_back(_src.m_material.at(i));

	m_king = nullptr;
	for (int i = 0; i < 64; i++)
		m_board[i] = nullptr;

	std::vector<Piece>::iterator itr = m_material.begin();
	while (itr != m_material.end())
	{
		m_board[itr->Square8x8] = &(*itr);
		itr++;
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



const Piece*
Material::GetPiece(PIECE pType, byte square) const
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
		if (m_materialGrid[piece->Type].at(ind)->Square8x8 == piece->Square8x8)
		{
			foundPiece = true;
			break;
		}
		ind++;
	}

	if (!foundPiece)
		return false;

	m_materialGrid[piece->Type].erase(m_materialGrid[piece->Type].begin() + ind);
	
	m_board[piece->Square8x8]->Square8x8 = 0;
	m_board[piece->Square8x8]->Square10x12 = 0;
	m_board[piece->Square8x8] = nullptr;
	return true;
}
const std::vector<const Piece*> 
GambitEngine::Material::GetMaterial() const
{
	std::vector<const Piece*> retVal;
	for (int i = 0; i < NR_OF_PIECES; i++)
		retVal.insert(retVal.end(), m_materialGrid[i].begin(), m_materialGrid[i].end());
	
	return retVal;
}

const std::vector<const Piece*> 
GambitEngine::Material::GetCaptured() const
{
	std::vector<const Piece*> retVal;
	for (int i = 0; i < NR_OF_PIECES; i++)
		retVal.insert(retVal.end(), m_capturedMaterialGrid[i].begin(), m_capturedMaterialGrid[i].end());

	return retVal;
}


const std::vector<const Piece*> 
GambitEngine::Material::GetMaterial(byte type) const
{
	std::vector<const Piece*> retVal;
	retVal.insert(retVal.end(), m_materialGrid[type].begin(), m_materialGrid[type].end());
	return retVal;
}

bool 
Material::AddPiece(Piece piece)
{
	std::vector<Piece>::iterator itr = m_material.begin();
	while (itr != m_material.end())
	{
		if (itr->Type == piece.Type && ((itr->Square8x8 == 0 && itr->Square10x12 == 0) || itr->Captured == true))
			break;
		itr++;
	}

	if (itr == m_material.end())
		return false;

	itr->Captured = false;
	itr->Square8x8 = piece.Square8x8;
	itr->Square10x12 = piece.Square10x12;
	
	m_materialGrid[itr->Type].push_back(&(*itr));

	m_board[piece.Square8x8] = &(*itr);

	if (piece.Type == KING)
		m_king = m_board[piece.Square8x8];
	return true;
}

bool
Material::CapturePiece(PIECE pType, byte tSqr)
{
	bool foundPiece = false;
	unsigned int ind = 0;
	while (m_materialGrid[pType].size() > ind)
	{
		if (m_materialGrid[pType].at(ind)->Square8x8 == tSqr)
		{
			foundPiece = true;
			break;
		}
		ind++;
	}

	if (!foundPiece)
		return false;

	m_board[tSqr]->Captured = true;

	m_materialGrid[pType].erase(m_materialGrid[pType].begin() + ind);
	m_capturedMaterialGrid[pType].push_back(m_board[tSqr]);
	m_board[tSqr] = nullptr;
	return true;
}

bool GambitEngine::Material::UncapturePiece(PIECE pType, byte tSqr)
{
	int i = 0;
	for (i; i < m_capturedMaterialGrid.at(pType).size(); i++)
	{
		if (m_capturedMaterialGrid.at(pType).at(i)->Square8x8 == tSqr)
			break;
	}

	if (i == m_capturedMaterialGrid.at(pType).size())
		return false;

	m_capturedMaterialGrid.at(pType).erase(m_capturedMaterialGrid.at(pType).begin() + i);
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

bool 
Material::DemotePiece(PIECE pType, byte tSqr)
{
	return ChangeTypeOfPiece(pType, PAWN, tSqr);
}

bool GambitEngine::Material::PromotePiece(PIECE pType, byte tSqr)
{
	return ChangeTypeOfPiece(PAWN, pType, tSqr);
}

bool GambitEngine::Material::ChangeTypeOfPiece(PIECE sType, PIECE tType, byte sqr)
{
	std::vector<Piece*>::iterator itr = m_materialGrid[sType].begin();
	u32 index = 0;
	while (itr != m_materialGrid[sType].end())
	{
		if (m_materialGrid[sType].at(index)->Square8x8 == sqr)
			break;

		itr++;	
		index++;
	}

	if (itr == m_materialGrid[sType].end())
		return false;

	(*itr)->Type = tType;
	m_materialGrid[sType].erase(itr);
	m_materialGrid[tType].push_back(m_board[sqr]);

	return true;
}
