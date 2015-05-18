/*
 * GameState.hpp
 *
 *  Created on: 11.05.2015
 *      Author: Andreas Molzer
 */
#pragma once
#include "Board.hpp"
#include "Move.hpp"
#include "Searcher.hpp"

constexpr unsigned SEARCHER_SIZE = 500000;

enum InitResultType{
	PASSED, DUPLICATE, MIDMOVE, TOOMANY
};

struct InitResultInfo{
	bool player;
	BoardBits board;
};

struct InitResult{
	InitResultType type;
	InitResultInfo info;
};

class GameState{
	TacTicBoard board;
	MoveHistory history;
	Searcher<::SEARCHER_SIZE> searcher;
	bool playerOneTurn;

	/**
	 * Applies a move to the board and sets its fields according to the current board state.
	 */
	void applyAndChangeMove(Move& m);
	/**
	 * Undo the given move (asserts it is correct)
	 */
	void undoMove(Move& m);
public:

	/**
	 * Checks if a move is valid and then plays it.
	 * Return is if the move was played
	 */
	bool playMove(Move m);

	/**
	 * Undo the last played move
	 */
	void undoMove();

	/**
	 * Get the move that would be optimal for the current player
	 * (Limited to the knowledge of the engine)
	 */
	MoveSuggestion getBestKnownMove();

	/**
	 * Checks if a move is valid
	 */
	bool isValidMove(Move& m);

	InitResult initializeWithMoves(Move playerOne[9], Move playerTwo[9]);

	bool isWon();

	bool isPlayerOneTurn();
};
