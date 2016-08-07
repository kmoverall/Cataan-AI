#pragma once

#include "stdafx.h"
#include "Pieces.h"

class BoardState
{
public:
	BoardState();
	BoardState(const int radius);
	BoardState(const BoardState& other);
	~BoardState();
	Hex* GetHex(const HexCoord position) const;

private:
	int m_radius = 0;
	std::unordered_map<HexCoord, Hex*> m_map;
	std::vector<Edge> edge_list;
	std::vector<Corner> corner_list;
	HexCoord m_robberLocation;

	void GenerateNew();
};

