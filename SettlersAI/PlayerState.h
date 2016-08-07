#pragma once
#include "stdafx.h"
#include "Pieces.h"

struct PlayerState
{
	PlayerState() : 
		index(0), 
		resources(std::vector<Resource>()),
		development(std::vector<DevelopmentCard>()) {}

	PlayerState(int index_) : 
		index(index_),
		resources(std::vector<Resource>()),
		development(std::vector<DevelopmentCard>()) {}

	~PlayerState() {}

	std::vector<Resource> resources;
	std::vector<DevelopmentCard> development;
	int roadLength = 0;
	int armySize = 0;
	int score = 0;
	int remainingSettlements = 5;
	int remainingCities = 4;
	int remainingRoads = 15;

	const int index;
};

