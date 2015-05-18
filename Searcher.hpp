/*
 * Searcher.hpp
 *
 *  Created on: 12.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

#include <atomic>
#include "GameState.hpp"

template<unsigned size>
class Searcher;

struct MoveSuggestion{
	Move move;
	float rating;
};

struct SearchNode{
	MoveDescriptor move;
	float rating;
};

template<unsigned size>
class Searcher{
	GameState const * gameState;
	MoveHistory movesGenerated;
	std::atomic<bool> pause;
public:
	Searcher(GameState * state);

	MoveSuggestion getBestKnownMove();

	void runParallel();

	void notifyUndo(Move& move);

	void notifyMoveMade(Move& move);

	void setPause(bool pause);
};
