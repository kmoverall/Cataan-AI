#pragma once

#include "stdafx.h"
#include "Pieces.h"
#include "PlayerState.h"

class BoardState
{
public:
	BoardState();
	BoardState(const int radius, const int numPlayers);
	BoardState(const BoardState& other);
	~BoardState();
	std::shared_ptr<Hex> GetHex(const HexCoord position) const;

	BoardState ProduceResources() const;
	BoardState PlayerTrade(std::vector<Resource> offer1, int player1, std::vector<Resource> offer2, int player2) const;
	BoardState BankTrade(std::vector<Resource> in, Resource out) const;
	BoardState BuildRoad(Edge location, int player) const;
	BoardState BuildSettlement(Corner location, int player) const;
	BoardState BuildCity(Corner location, int player) const;
	BoardState BuyDevelopment(int player) const;
	BoardState MoveRobber(Hex location) const;
	BoardState PlayDevelopmentCard(DevelopmentCard card) const;

private:
	static const int STARTING_RESOURCES = 19;

	int m_radius = 0;
	int m_numPlayers = 0;
	std::unordered_map<HexCoord, std::shared_ptr<Hex>> m_map;
	std::vector<std::shared_ptr<Edge>> m_edgeList;
	std::vector<std::shared_ptr<Corner>> m_cornerList;
	HexCoord m_robberLocation;

	std::vector<DevelopmentCard> m_developmentDeck;
	int m_remainingResources[5];

	std::vector<PlayerState> m_players;

	void GenerateNew();
	void UpdateLongestRoad();
	void UpdateLargestArmy();

};

