#include "stdafx.h"
#include "Board.h"

BoardState::BoardState() : 
	m_radius(2),
	m_map(std::unordered_map<HexCoord, std::shared_ptr<Hex>>())
{
	GenerateNew();
}

BoardState::BoardState(const int radius) :
	m_radius(radius),
	m_map(std::unordered_map<HexCoord, std::shared_ptr<Hex>>())
{
	GenerateNew();
}

BoardState::BoardState(const BoardState& other) :
	m_radius(other.m_radius),
	m_robberLocation(other.m_robberLocation)
{
	//Create copy of the edge and corner list
	for (auto& it : other.m_edge_list) {
		m_edge_list.push_back( std::make_shared<Edge>(*it) );
	}
	for (auto& it : other.m_corner_list) {
		m_corner_list.push_back( std::make_shared<Corner>(*it) );
	}

	//Create copies of all of the hexes
	for (auto& it : other.m_map) {
		m_map.emplace(it.first, std::make_shared<Hex>(*(it.second)));
	}

	//Change hexes to reference edges and corners of the copied edge and corner list
	for (auto& it : m_map) 
	{
		for (int i = 0; i < 6; i++) 
		{
			it.second->edges[i] = m_edge_list[it.second->edges[i]->index];
			it.second->corners[i] = m_corner_list[it.second->corners[i]->index];
		}
	}
}

BoardState::~BoardState()
{
}

void BoardState::GenerateNew()
{
	//Forces Radius of 2. TODO: Remove once variable board sizes are implemented
	m_radius = 2;

	//Generate Hexes for map
	for (int q = -m_radius; q <= m_radius; q++) {
		int r1 = std::max(-m_radius, -q - m_radius);
		int r2 = std::min(m_radius, -q + m_radius);
		for (int r = r1; r <= r2; r++) {
			m_map.emplace(HexCoord(q, r, -q - r), std::make_shared<Hex>());
		}
	}

	//Add edges and corners to the map
	for (auto& it : m_map) {
		for (int i = 0; i < 6; i++)
		{
			if (it.second->edges[i] == nullptr)
			{
				m_edge_list.push_back( std::make_shared<Edge>(m_edge_list.size()) );
				it.second->edges[i] = m_edge_list.back();

				//Add a reference to the edge to hex that shares that edge
				if (GetHex(it.first + HexDirections::VECTORS[i]) != nullptr)
				{
					GetHex(it.first + HexDirections::VECTORS[i])->edges[(i + 3) % 6] = it.second->edges[i];
				}
			}

			if (it.second->corners[i] == nullptr)
			{
				m_corner_list.push_back( std::make_shared<Corner>(m_corner_list.size()) );
				it.second->corners[i] = m_corner_list.back();

				//Add a reference to the corner to hexes that share it
				if (GetHex(it.first + HexDirections::VECTORS[i]) != nullptr)
				{
					GetHex(it.first + HexDirections::VECTORS[i])->corners[(i + 2) % 6] = it.second->corners[i];
				}

				if (GetHex(it.first + HexDirections::VECTORS[(i + 1) % 6]) != nullptr)
				{
					GetHex(it.first + HexDirections::VECTORS[(i + 1) % 6])->corners[(i + 3) % 6] = it.second->corners[i];
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

	//Add preset Ports
	GetHex(HexCoord(0, 2, -2))->edges[(int)(HexDirections::Direction::North)]->port = Port::Any;
	GetHex(HexCoord(1, 1, -2))->edges[(int)(HexDirections::Direction::Northeast)]->port = Port::Grain;
	GetHex(HexCoord(2, -1, -1))->edges[(int)(HexDirections::Direction::Northeast)]->port = Port::Ore;
	GetHex(HexCoord(2, -2, 0))->edges[(int)(HexDirections::Direction::Southeast)]->port = Port::Any;
	GetHex(HexCoord(1, -2, 1))->edges[(int)(HexDirections::Direction::South)]->port = Port::Wool;
	GetHex(HexCoord(-1, -1, 2))->edges[(int)(HexDirections::Direction::South)]->port = Port::Grain;
	GetHex(HexCoord(-2, 0, 2))->edges[(int)(HexDirections::Direction::Southwest)]->port = Port::Any;
	GetHex(HexCoord(-2, 1, 1))->edges[(int)(HexDirections::Direction::Northwest)]->port = Port::Brick;
	GetHex(HexCoord(-1, 2, -1))->edges[(int)(HexDirections::Direction::Northwest)]->port = Port::Lumber;

	//Position Robber
	m_robberLocation = HexCoord(0, 0, 0);
}

std::shared_ptr<Hex> BoardState::GetHex(const HexCoord position) const
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
