#pragma once
#include "stdafx.h"
#include "Pieces.h"
#include "PlayerState.h"
#include "Board.h"

enum class PlayerGoal { BuildSettlement, BuiltCity, GetLargestArmy, GetLongestRoad, GetPointCard };
typedef std::map<PlayerGoal, std::map<Resource, int>> PlayerNeeds;
typedef std::map<Resource, float> PlayerIncome;
typedef std::map<PlayerGoal, float> TimeToGoal;
typedef std::map<PlayerGoal, float> PlayerPriorities;
typedef std::map<std::function<BoardState>, float> ActionDesire;  

class PlayerAgent
{
public:
	PlayerAgent();
	PlayerAgent(BoardState initial, int index);
	~PlayerAgent();

private:
	BoardState m_currentGameState;
	BoardState m_checkGameState;
	PlayerState m_currentPlayerState;
	PlayerState m_checkPlayerState;

	PlayerNeeds m_resourceNeeds;
	PlayerIncome m_income;
	TimeToGoal m_timeNeeded;
	PlayerPriorities m_priorities;
	ActionDesire m_actionPriorities;

	std::function<BoardState> SelectAction(BoardState board);

	void AcquirePlayerState(BoardState board);
	PlayerNeeds EvaluateResourceNeeds();
	PlayerIncome EvaluateIncome();
	TimeToGoal EvaluateTimeToGoal();
	PlayerPriorities EvalutateGoals();
};

