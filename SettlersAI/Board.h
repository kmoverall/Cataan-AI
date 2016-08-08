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

	int RollDice() const;
	BoardState ProduceResources(const int roll) const;
	BoardState PlayerTrade(const std::vector<Resource> offer1, const int player1, const std::vector<Resource> offer2, const int player2) const;
	BoardState BankTrade(const std::vector<Resource> in, const Resource out, const int player) const;
	BoardState BuildRoad(const std::shared_ptr<Edge> location, const int player) const;
	BoardState BuildSettlement(const std::shared_ptr<Corner> location, const int player) const;
	BoardState BuildCity(const std::shared_ptr<Corner> location, const int player) const;
	BoardState BuyDevelopment(const int player) const;
	BoardState MoveRobber(const HexCoord location) const;
	BoardState Monopoly(const Resource resource, const int player) const;
	BoardState YearOfPlenty(const Resource resource1, const Resource resource2, const int player) const;

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

