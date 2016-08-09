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
	BoardState PlayerTrade(const std::map<Resource, int> ownOffer, const std::map<Resource, int> otherOffer, const int otherPlayer) const;
	BoardState BankTrade(const std::map<Resource, int> toBank, const std::map<Resource, int> fromBank) const;
	BoardState BuildRoad(const std::shared_ptr<Edge> location) const;
	BoardState BuildSettlement(const std::shared_ptr<Corner> location) const;
	BoardState BuildCity(const std::shared_ptr<Corner> location) const;
	BoardState BuyDevelopment() const;
	BoardState DiscardCards(const std::map<Resource, int> amount) const;
	BoardState MoveRobber(const HexCoord location) const;
	BoardState StealCard(const int otherPlayer) const;
	BoardState Knight() const;
	BoardState Monopoly(const Resource resource) const;
	BoardState YearOfPlenty(const Resource resource1, const Resource resource2) const;
	BoardState RoadBuilding() const;
	BoardState EndPhase() const;
	BoardState EndTurn() const;

	std::vector<std::function<void>> GetAvailableActions();

private:
	static const int STARTING_RESOURCES = 19;

	int m_radius = 0;
	int m_numPlayers = 0;
	int m_playerTurn = 0;
	int m_robberPlayer = -1;
	int winner = -1;
	TurnPhase m_turnPhase = TurnPhase::Produce;
	RobberPhase m_robberPhase = RobberPhase::Inactive;
	SetupPhase m_setupPhase = SetupPhase::Settlement1;
	int m_roadBuildingPhase = 0;
	std::unordered_map<HexCoord, std::shared_ptr<Hex>> m_map;
	std::vector<std::shared_ptr<Edge>> m_edgeList;
	std::vector<std::shared_ptr<Corner>> m_cornerList;
	HexCoord m_robberLocation;

	std::vector<DevelopmentCard> m_developmentDeck;

	std::vector<PlayerState> m_players;

	void GenerateNew();
	void UpdateLongestRoad();
	void UpdateLargestArmy();
	void CheckGameEnd();
	std::vector<std::shared_ptr<Edge>> GetValidRoadEdges() const;
	std::vector<std::shared_ptr<Corner>> GetValidSettlmentCorner() const;
	bool HasValidBankTrade() const;
};

