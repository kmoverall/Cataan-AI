#include "stdafx.h"
#include "Board.h"

BoardState::BoardState() : 
	m_radius(2),
	m_numPlayers(4),
	m_map(std::unordered_map<HexCoord, std::shared_ptr<Hex>>())
{
	GenerateNew();
}

BoardState::BoardState(const int radius, const int numPlayers) :
	m_radius(radius),
	m_numPlayers(numPlayers),
	m_map(std::unordered_map<HexCoord, std::shared_ptr<Hex>>())
{
	GenerateNew();
}

BoardState::BoardState(const BoardState& other) :
	m_radius(other.m_radius),
	m_robberLocation(other.m_robberLocation)
{
	//Create copy of the edge and corner list
	for (auto& it : other.m_edgeList) {
		m_edgeList.push_back( std::make_shared<Edge>(*it) );
	}
	for (auto& it : other.m_cornerList) {
		m_cornerList.push_back( std::make_shared<Corner>(*it) );
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
			it.second->edges[i] = m_edgeList[it.second->edges[i]->index];
			it.second->corners[i] = m_cornerList[it.second->corners[i]->index];
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
				m_edgeList.push_back( std::make_shared<Edge>(m_edgeList.size()) );
				it.second->edges[i] = m_edgeList.back();

				//Add a reference to the edge to hex that shares that edge
				if (GetHex(it.first + HexDirections::VECTORS[i]) != nullptr)
				{
					GetHex(it.first + HexDirections::VECTORS[i])->edges[(i + 3) % 6] = it.second->edges[i];
				}
			}

			if (it.second->corners[i] == nullptr)
			{
				m_cornerList.push_back( std::make_shared<Corner>(m_cornerList.size()) );
				it.second->corners[i] = m_cornerList.back();

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

	//Fill development deck
	for(int i = 0; i < 14; i ++)
		m_developmentDeck.push_back(DevelopmentCard::Knight);
	for (int i = 0; i < 2; i++)
		m_developmentDeck.push_back(DevelopmentCard::Monopoly);
	for (int i = 0; i < 2; i++)
		m_developmentDeck.push_back(DevelopmentCard::RoadBuilding);
	for (int i = 0; i < 2; i++)
		m_developmentDeck.push_back(DevelopmentCard::YearOfPlenty);
	for (int i = 0; i < 5; i++)
		m_developmentDeck.push_back(DevelopmentCard::VictoryPoint);

	//Set up players
	for (int i = 0; i < m_numPlayers; i++)
		m_players.push_back(PlayerState(i));
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

BoardState BoardState::ProduceResources() const
{
	BoardState result = *this;

	int roll = rand() % 6 + rand() % 6 + 2;

	result.m_turnPhase = TurnPhase::EndProduce;

	if (roll == 7) {
		result.m_robberPhase = RobberPhase::Discard;
		result.m_robberPlayer = result.m_playerTurn;
		result.m_playerTurn = (result.m_playerTurn + 1) % m_numPlayers;
		return result;
	}

	for (auto& it : m_map) {
		//Number was rolled, robber is not there, and terrain is not desert
		if (it.second->value == roll && it.first != m_robberLocation && it.second->terrain != Terrain::Desert) {
			for (int i = 0; i < 6; i++) {
				if (it.second->corners[i]->owner != -1) {
					result.m_players[i].resources[(Resource)(it.second->terrain)]++;
					if (it.second->corners[i]->building == CornerBuilding::City)
						result.m_players[i].resources[(Resource)(it.second->terrain)]++;
				}
			}
		}
	}

	return result;
}

BoardState BoardState::PlayerTrade(const std::map<Resource, int> ownOffer, const std::map<Resource, int> otherOffer, const int otherPlayer) const
{
	BoardState result = *this;

	//Move player 1's offer from its hand to player 2's
	for (auto& offer : ownOffer) {
		result.m_players[m_playerTurn].resources[offer.first] -= offer.second;
		result.m_players[otherPlayer].resources[offer.first] += offer.second;
	}

	//Move player 2's offer from its hand to player 1's
	for (auto& offer : otherOffer) {
		result.m_players[otherPlayer].resources[offer.first] -= offer.second;
		result.m_players[m_playerTurn].resources[offer.first] += offer.second;
	}

	return result;
}

BoardState BoardState::BankTrade(const std::map<Resource, int> toBank, const std::map<Resource, int> fromBank) const
{
	BoardState result = *this;

	//Remove resources from player's hand
	for (auto& offer : toBank) {
		result.m_players[m_playerTurn].resources[offer.first] -= offer.second;
	}

	//Add resource traded for into player's hand
	for (auto& offer : fromBank) {
		result.m_players[m_playerTurn].resources[offer.first] += offer.second;
	}

	return result;
}

BoardState BoardState::BuildRoad(const std::shared_ptr<Edge> location) const
{
	BoardState result = *this;

	result.m_edgeList[location->index]->building = EdgeBuilding::Road;
	result.m_edgeList[location->index]->owner = m_playerTurn;

	if(result.m_roadBuildingPhase != 0 && result.m_setupPhase == SetupPhase::Done) {
		result.m_players[m_playerTurn].resources[Resource::Brick]--;
		result.m_players[m_playerTurn].resources[Resource::Lumber]--;
	}

	if (result.m_roadBuildingPhase > 0)
		result.m_roadBuildingPhase++;
	if (result.m_roadBuildingPhase > 2)
		result.m_roadBuildingPhase = 0;

	result.UpdateLongestRoad();

	if (result.m_setupPhase != SetupPhase::Done && result.m_setupPhase == SetupPhase::Road1) {
		if (result.m_playerTurn != result.m_numPlayers - 1) {
			result.m_setupPhase = SetupPhase::Settlement1;
			result.m_playerTurn++;
		}
		else {
			result.m_setupPhase = SetupPhase::Settlement2;
		}
	}
	else if (result.m_setupPhase != SetupPhase::Done && result.m_setupPhase == SetupPhase::Road2) {
		if (result.m_playerTurn != 0) {
			result.m_setupPhase = SetupPhase::Settlement2;
			result.m_playerTurn--;
		}
		else {
			result.m_setupPhase = SetupPhase::Done;
		}
	}

	return result;
}

BoardState BoardState::BuildSettlement(const std::shared_ptr<Corner> location) const
{
	BoardState result = *this;

	result.m_cornerList[location->index]->building = CornerBuilding::Settlement;
	result.m_cornerList[location->index]->owner = m_playerTurn;

	if (result.m_setupPhase == SetupPhase::Done) {
		result.m_players[m_playerTurn].resources[Resource::Brick]--;
		result.m_players[m_playerTurn].resources[Resource::Lumber]--;
		result.m_players[m_playerTurn].resources[Resource::Wool]--;
		result.m_players[m_playerTurn].resources[Resource::Ore]--;
	}

	result.m_players[m_playerTurn].score++;
	result.CheckGameEnd();

	if (result.m_setupPhase != SetupPhase::Done && result.m_setupPhase == SetupPhase::Settlement1) {
		result.m_setupPhase = SetupPhase::Road1;
	}
	else if (result.m_setupPhase != SetupPhase::Done && result.m_setupPhase == SetupPhase::Settlement2) {
		result.m_setupPhase = SetupPhase::Road2;
	}

	return result;
}

BoardState BoardState::BuildCity(const std::shared_ptr<Corner> location) const
{
	BoardState result = *this;

	result.m_cornerList[location->index]->building = CornerBuilding::City;

	result.m_players[m_playerTurn].resources[Resource::Ore] -= 3;
	result.m_players[m_playerTurn].resources[Resource::Grain] -= 2;

	result.m_players[m_playerTurn].score++;
	result.CheckGameEnd();

	return result;
}

BoardState BoardState::BuyDevelopment() const
{
	BoardState result = *this;

	int cardIndex = rand() % result.m_developmentDeck.size();
	DevelopmentCard card = result.m_developmentDeck[cardIndex];

	result.m_players[m_playerTurn].resources[Resource::Wool]--;
	result.m_players[m_playerTurn].resources[Resource::Ore]--;
	result.m_players[m_playerTurn].resources[Resource::Grain]--;

	//Add development card to player hand
	result.m_players[m_playerTurn].development[card]++;
	if (card == DevelopmentCard::VictoryPoint) {
		result.m_players[m_playerTurn].score++;
		result.CheckGameEnd();
	}

	//Remove from deck
	result.m_developmentDeck.erase(result.m_developmentDeck.begin() + cardIndex);

	return result;
}

BoardState BoardState::DiscardCards(const std::map<Resource, int> amount) const
{
	BoardState result = *this;

	for (auto& discard : amount) {
		result.m_players[m_playerTurn].resources[discard.first] -= discard.second;
	}

	if (result.m_playerTurn != result.m_robberPlayer) {
		result.m_playerTurn = (result.m_playerTurn + 1) % m_numPlayers;
	}
	else {
		result.m_robberPhase = RobberPhase::Move;
	}

	return result;
}

BoardState BoardState::MoveRobber(const HexCoord location) const
{
	BoardState result = *this;

	result.m_robberLocation = location;
	result.m_robberPhase = RobberPhase::Steal;

	return result;
}

BoardState BoardState::StealCard(const int otherPlayer) const
{
	BoardState result = *this;

	int handSize = 0;
	for(auto& res : result.m_players[otherPlayer].resources) {
		handSize += res.second;
	}

	int choice = rand() % handSize;
	int count = 0;

	for (auto& res : result.m_players[otherPlayer].resources) {
		count += res.second;
		if (choice < count) {
			result.m_players[m_playerTurn].resources[res.first]++;
			res.second--;
			break;
		}
	}

	result.m_robberPhase = RobberPhase::Inactive;

	return result;
}

BoardState BoardState::Knight() const
{
	BoardState result = *this;

	result.m_players[m_playerTurn].development[DevelopmentCard::Knight]--;
	result.UpdateLargestArmy();
	result.m_robberPhase = RobberPhase::Move;

	return result;
}

BoardState BoardState::Monopoly(const Resource resource) const
{
	BoardState result = *this;

	//Find all player's resources of a type, remove it from their hands, and add it the current player's hand
	for (auto& plyr : result.m_players) {
		if (plyr.index != m_playerTurn) {
			result.m_players[m_playerTurn].resources[resource] += plyr.resources[resource];
			plyr.resources[resource] = 0;
		}
	}

	result.m_players[m_playerTurn].development[DevelopmentCard::Monopoly]--;

	return result;
}

BoardState BoardState::YearOfPlenty(const Resource resource1, const Resource resource2) const
{
	BoardState result = *this;

	result.m_players[m_playerTurn].resources[resource1]++;
	result.m_players[m_playerTurn].resources[resource2]++;

	result.m_players[m_playerTurn].development[DevelopmentCard::YearOfPlenty]--;

	return result;
}

BoardState BoardState::RoadBuilding() const
{
	BoardState result = *this;

	result.m_roadBuildingPhase = 1;

	return result;
}

BoardState BoardState::EndPhase() const
{
	BoardState result = *this;
	result.m_turnPhase = (TurnPhase)((int)(result.m_turnPhase) + 1);
	return result;
}

BoardState BoardState::EndTurn() const
{
	BoardState result = *this;
	result.m_playerTurn = (result.m_playerTurn + 1) % result.m_numPlayers;
	result.m_turnPhase = TurnPhase::Produce;
	return result;
}

std::vector<std::function<void>> BoardState::GetAvailableActions()
{
	std::vector<std::function<void>> validMoves = std::vector<std::function<void>>();

	switch (m_setupPhase) 
	{
		case SetupPhase::Done:
			switch (m_robberPhase)
			{
				case RobberPhase::Inactive:
					switch (m_turnPhase)
					{
						case TurnPhase::Produce:
							validMoves.push_back(ProduceResources);
							break;

						case TurnPhase::EndProduce:
							validMoves.push_back(EndPhase);
							break;

						case TurnPhase::Build:
							if (m_roadBuildingPhase > 0) {
								validMoves.push_back(BuildRoad);
							}
							else {
								validMoves.push_back(PlayerTrade);
								validMoves.push_back(BankTrade);
								if (m_players[m_playerTurn].resources[Resource::Brick] > 0 && m_players[m_playerTurn].resources[Resource::Wool] > 0
									&& m_players[m_playerTurn].resources[Resource::Lumber] > 0 && m_players[m_playerTurn].resources[Resource::Grain] > 0) {
									validMoves.push_back(BuildSettlement);
								}
								if (m_players[m_playerTurn].resources[Resource::Ore] > 2 && m_players[m_playerTurn].resources[Resource::Grain] > 1) {
									validMoves.push_back(BuildCity);
								}
								if (m_players[m_playerTurn].resources[Resource::Brick] > 0 && m_players[m_playerTurn].resources[Resource::Lumber] > 0) {
									validMoves.push_back(BuildRoad);
								}
								if (m_players[m_playerTurn].resources[Resource::Ore] > 0 && m_players[m_playerTurn].resources[Resource::Wool] > 0
									&& m_players[m_playerTurn].resources[Resource::Grain] > 0) {
									validMoves.push_back(BuyDevelopment);
								}
								validMoves.push_back(EndTurn);
							}
							break;
					}

					if (m_players[m_playerTurn].development[DevelopmentCard::Knight] > 0) {
						validMoves.push_back(Knight);
					}
					if (m_players[m_playerTurn].development[DevelopmentCard::Monopoly] > 0) {
						validMoves.push_back(Monopoly);
					}
					if (m_players[m_playerTurn].development[DevelopmentCard::YearOfPlenty] > 0) {
						validMoves.push_back(YearOfPlenty);
					}
					if (m_players[m_playerTurn].development[DevelopmentCard::RoadBuilding] > 0) {
						validMoves.push_back(RoadBuilding);
					}
					break;

				case RobberPhase::Move:
					validMoves.push_back(MoveRobber);
					break;

				case RobberPhase::Discard:
					validMoves.push_back(DiscardCards);
					break;

				case RobberPhase::Steal:
					validMoves.push_back(StealCard);
					break;
			}
			break;

		case SetupPhase::Settlement1:
		case SetupPhase::Settlement2:
			validMoves.push_back(BuildSettlement);
			break;

		case SetupPhase::Road1:
		case SetupPhase::Road2:
			validMoves.push_back(BuildRoad);
			break;

	}

	return validMoves;
}
