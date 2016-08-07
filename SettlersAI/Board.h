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
	std::shared_ptr<Hex> GetHex(const HexCoord position) const;

private:
	int m_radius = 0;
	std::unordered_map<HexCoord, std::shared_ptr<Hex>> m_map;
	std::vector<std::shared_ptr<Edge>> m_edge_list;
	std::vector<std::shared_ptr<Corner>> m_corner_list;
	HexCoord m_robberLocation;

	void GenerateNew();
};

