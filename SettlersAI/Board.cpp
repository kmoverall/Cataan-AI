#include "stdafx.h"
#include "Board.h"

BoardState::BoardState() : 
	m_radius(2),
	m_map(std::unordered_map<HexCoord, Hex*>())
{
	GenerateNew();
}

BoardState::BoardState(const int radius) :
	m_radius(radius),
	m_map(std::unordered_map<HexCoord, Hex*>())
{
	GenerateNew();
}

BoardState::BoardState(const BoardState& other)
{

}

BoardState::~BoardState()
{
}

void BoardState::GenerateNew()
{
	//Generate Hexes for map
	for (int q = -m_radius; q <= m_radius; q++) {
		int r1 = std::max(-m_radius, -q - m_radius);
		int r2 = std::min(m_radius, -q + m_radius);
		for (int r = r1; r <= r2; r++) {
			m_map.emplace(HexCoord(q, r, -q - r), new Hex());
		}
	}

	//Add edges and corners to the map
	for (auto& it : m_map) {
		for (int i = 0; i < 6; i++)
		{
			if (it.second->edges[i] == NULL) 
			{
				edge_list.push_back( Edge(edge_list.size()) );
				it.second->edges[i] = &(edge_list.back());

				//Add a reference to the edge to hex that shares that edge
				if (GetHex(it.first + HexDirections::DIRECTIONS[i]) != NULL) 
				{
					GetHex(it.first + HexDirections::DIRECTIONS[i])->edges[(i + 3) % 6] = it.second->edges[i];
				}
			}

			if (it.second->corners[i] == NULL)
			{
				corner_list.push_back( Corner(corner_list.size()) );
				it.second->corners[i] = &(corner_list.back());

				//Add a reference to the corner to hexes that share it
				if (GetHex(it.first + HexDirections::DIRECTIONS[i]) != NULL)
				{
					GetHex(it.first + HexDirections::DIRECTIONS[i])->corners[(i + 2) % 6] = it.second->corners[i];
				}

				if (GetHex(it.first + HexDirections::DIRECTIONS[(i + 1) % 6]) != NULL)
				{
					GetHex(it.first + HexDirections::DIRECTIONS[(i + 1) % 6])->corners[(i + 3) % 6] = it.second->corners[i];
				}
			}
		}
	}

	//Hard Code Default Terrain and Values
	GetHex(HexCoord(0, 0, 0))->terrain = Terrain::Desert;
	GetHex(HexCoord(0, 0, 0))->value = 7;

	GetHex(HexCoord(0, 1, -1))->terrain = Terrain::Hills;
	GetHex(HexCoord(0, 1, -1))->value = 6;

	GetHex(HexCoord(1, 0, -1))->terrain = Terrain::Pasture;
	GetHex(HexCoord(1, 0, -1))->value = 4;

	GetHex(HexCoord(1, -1, 0))->terrain = Terrain::Forest;
	GetHex(HexCoord(1, -1, 0))->value = 3;

	GetHex(HexCoord(0, -1, 1))->terrain = Terrain::Fields;
	GetHex(HexCoord(0, -1, 1))->value = 4;

	GetHex(HexCoord(-1, 0, 1))->terrain = Terrain::Mountains;
	GetHex(HexCoord(-1, 0, 1))->value = 3;

	GetHex(HexCoord(-1, 1, 0))->terrain = Terrain::Forest;
	GetHex(HexCoord(-1, 1, 0))->value = 11;

	GetHex(HexCoord(0, 2, -2))->terrain = Terrain::Mountains;
	GetHex(HexCoord(0, 2, -2))->value = 10;

	GetHex(HexCoord(1, 1, -2))->terrain = Terrain::Pasture;
	GetHex(HexCoord(1, 1, -2))->value = 2;

	GetHex(HexCoord(2, 0, -2))->terrain = Terrain::Forest;
	GetHex(HexCoord(2, 0, -2))->value = 9;

	GetHex(HexCoord(2, -1, -1))->terrain = Terrain::Hills;
	GetHex(HexCoord(2, -1, -1))->value = 10;

	GetHex(HexCoord(2, -2, 0))->terrain = Terrain::Mountains;
	GetHex(HexCoord(2, -2, 0))->value = 8;

	GetHex(HexCoord(1, -2, 1))->terrain = Terrain::Pasture;
	GetHex(HexCoord(1, -2, 1))->value = 5;

	GetHex(HexCoord(0, -2, 2))->terrain = Terrain::Pasture;
	GetHex(HexCoord(0, -2, 2))->value = 11;

	GetHex(HexCoord(-1, -1, 2))->terrain = Terrain::Fields;
	GetHex(HexCoord(-1, -1, 2))->value = 6;

	GetHex(HexCoord(-2, 0, 2))->terrain = Terrain::Hills;
	GetHex(HexCoord(-2, 0, 2))->value = 5;

	GetHex(HexCoord(-2, 1, 1))->terrain = Terrain::Forest;
	GetHex(HexCoord(-2, 1, 1))->value = 8;

	GetHex(HexCoord(-2, 2, 0))->terrain = Terrain::Fields;
	GetHex(HexCoord(-2, 2, 0))->value = 9;

	GetHex(HexCoord(-1, 2, -1))->terrain = Terrain::Fields;
	GetHex(HexCoord(-1, 2, -1))->value = 12;
}

Hex* BoardState::GetHex(const HexCoord position) const
{
	if (m_map.find(position) == m_map.end())
	{
		return NULL;
	}
	else
	{
		return m_map.at(position);
	}
}
