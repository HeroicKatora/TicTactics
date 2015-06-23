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

enum InitResultType{
	PASSED = 0,			//All good
	DUPLICATE = 1,		//Move was duplicate
	INVALID = 2,		//A move was invalid (input could already catch)
	MIDDLE_MOVE = 3,	//Move was in the most center, 5|5
	FIELD_OVERFLOW = 4,	//More than one move on the same field
	BOARD_OVERFLOW = 5,	//More than two moves on the same board
	NOT_DURING_INIT = 6	//Game already going
};

struct InitResultInfo{
	bool playerOne;
	BoardBits index;
};

struct InitResult{
	InitResultType type;
	InitResultInfo info;
};

class GameState{
	friend Searcher;
	friend float rate(GameState&);
	TacTicBoard board;
	MoveHistory history;
	Searcher * searcher;
	bool playerOneTurn;
	bool inSetup;

	/**
	 * Applies a move to the board and sets its fields according to the current board state.
	 */
	void applyAndChangeMove(Move& m);
	void _applyMove(Move m); //Convenience, should use this only in init
	/**
	 * Undo the given move (asserts it is correct)
	 */
	void undoMove(Move& m);

	/**
	 * Checks if the initial moves of a player are valid
	 */
	InitResult checkSingleValidity(MoveDescriptor playerMoves[9]);
public:

	/**
	 * Constructs a game that is in setup phase
	 */
	GameState():board(), history(), searcher(0), playerOneTurn(true), inSetup(true){}
	GameState(GameState&) = default;
	GameState(const GameState&) = default;
	/**
	 * Checks if a move is valid and then plays it.
	 * Return is if the move was valid
	 */
	bool playMove(Move m);

	/**
	 * Undo the last played move
	 */
	void undoMove();

	/**
	 * Checks if a move is valid
	 */
	bool isValidMove(Move& m) const;

	/**
	 * Initializes the board with the first 9 moves of each player
	 */
	InitResult initializeWithMoves(MoveDescriptor playerOne[9], MoveDescriptor playerTwo[9]);

	/**
	 * True if a player has won the game
	 */
	bool isWon() const;

	/**
	 * Checks if a specific player has won the game
	 */
	bool hasWon(bool playerOne) const;

	/**
	 * True if player one is to play, false if player two
	 */
	bool isPlayerOneTurn() const;

	/**
	 * Prints the full game board
	 */
	int print() const;
	int sprint(char *) const;

	/**
	 * Prints a single board, but with the surrounding game
	 */
	int print(int index) const;
	int sprint(int index, char *) const;
	/**
	 * Ends setup phase and starts the game as it is initialized
	 */
	void start();

	/**
	 * Returns a pointer to the searcher if there is one
	 */
	Searcher* getSearcher() const;
};
