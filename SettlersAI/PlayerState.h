#pragma once
#include "stdafx.h"
#include "Pieces.h"

struct PlayerState
{
	PlayerState() : index(0)
	{
		for (int i = 1; i < 6; i++) {
			resources.insert(std::pair<Resource, int>((Resource)i, 0));
		}
		for (int i = 0; i < 5; i++) {
			development.insert(std::pair<DevelopmentCard, int>((DevelopmentCard)i, 0));
		}
	}

	PlayerState(int index_) : index(index_)
	{

	}

	~PlayerState() {}

	std::map<Resource, int> resources;
	std::map<Resource, int> publicResources;
	std::map<DevelopmentCard, int> development;
	std::map<DevelopmentCard, int> playedCards;
	int roadLength = 0;
	int armySize = 0;
	int score = 0;
	int remainingSettlements = 5;
	int remainingCities = 4;
	int remainingRoads = 15;

	const int index;
};