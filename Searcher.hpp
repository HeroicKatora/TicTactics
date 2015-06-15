/*
 * Searcher.hpp
 *
 *  Created on: 12.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

#include <atomic>
#include <mutex>
#include "Move.hpp"
#include "Rating.hpp"

constexpr unsigned SEARCHER_SIZE = 500000;
constexpr size_t CHILD_COUNT = 71;
class GameState;

class Searcher;

struct MoveSuggestion{
	MoveDescriptor move;
	float rating;
};

struct SearchNode{ //If rating = +-inf, win of one player,
	SearchNode(): rating(),weight(), move{}{
	}
	//if no child nodes and rating != inf, obviously no children are existent
	float rating;
	//Weight of this note, depends on depth and rating
	float weight;
	MoveDescriptor move;
	SearchNode *children [CHILD_COUNT];
};

class Searcher{
	const GameState *gameState;
	std::atomic<bool> pause;
	std::mutex pauseMutex;
	std::atomic<bool> end;
	SearchNode topNode;

	size_t discoverMoves(const GameState *state,SearchNode **dest, size_t maxNumber);

	void parallelSearch(SearchNode *startNode);

public:
	Searcher(GameState * state);

	MoveSuggestion getBestKnownMove();

	/**
	 * Called to spawn parallel searcher threads
	 */
	void runParallel();

	void notifyUndo(Move& move);

	void notifyMoveMade(Move& move);

	void setPause(bool pause);

	void endParallel();
};
