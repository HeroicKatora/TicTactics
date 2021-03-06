/*
 * Searcher.hpp
 *
 *  Created on: 12.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

class Searcher;

#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include "Move.hpp"
#include "Rating.hpp"

constexpr unsigned SEARCHER_SIZE = 500000;
class EngineController;

struct MoveSuggestion{
	MoveDescriptor move;
	Rating rating;
};

struct SearchNode{ //If rating = +-MAX, win of one player,
	SearchNode(): rating(),weight(), childCount(0), move{}, children(0){
	}
	//if no child nodes and rating != MAX, obviously no children are existent
	Rating rating;
	//Weight of this note, depends on depth and rating relative to others
	Weigth weight;

	unsigned childCount;
	Move move;
	SearchNode *children;

	/**
	 * Discovers all possible moves for this node
	 */
	void discover(const TacTicBoard&);
	/**
	 * Frees all children for this node
	 */
	void close();
	/**
	 * Reorders the child nodes so that the most valuable is at index 0
	 * If playerOne, max the rating, else minimize it
	 * Then it sets its rating to the maximum
	 */
	void revalueChildren(bool playerOne);
	void revalueChildren(bool playerOne, unsigned maxIndex);
	/**
	 * Only sets the rating to the maximum in its children
	 */
	void setToMaxChild(bool playerOne);
};

class Searcher{
	friend EngineController;
	const GameState *gameState;
	std::atomic<bool> pause;
	std::mutex pauseMutex;
	std::atomic<bool> end;
	std::thread searchThread;
	SearchNode topNode;

	void startSearch(SearchNode *startNode, unsigned maximalDepth, std::chrono::seconds maxDuration);

public:
	Searcher(const GameState * state);
	~Searcher();

	MoveSuggestion getBestKnownMove() const;

	/**
	 * Called to spawn parallel searcher threads
	 */
	void runParallel();

	void notifyUndo(Move& move);

	void notifyMoveMade(Move& move);

	void setPause(bool pause);

	void endParallel();

	void eval();
};

/**
 * Allocates a new array containing the discovered move nodes and stores it to dest.
 * The return value is how many were discovered. An old move of invalid means the game is at the start.
 */
size_t discoverMoves(const TacTicBoard& state, SearchNode *&dest, const Move& oldMove);
