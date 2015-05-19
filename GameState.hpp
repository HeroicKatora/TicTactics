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
	TacTicBoard board;
	MoveHistory history;
	Searcher searcher;
	bool playerOneTurn;

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

	GameState():board(), history(), searcher(this), playerOneTurn(true){}

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

	/**
	 * Initializes the board with the first 9 moves of each player
	 */
	InitResult initializeWithMoves(MoveDescriptor playerOne[9], MoveDescriptor playerTwo[9]);

	/**
	 * True if a player has won the game
	 */
	bool isWon();

	/**
	 * Checks if a specific player has won the game
	 */
	bool hasWon(bool playerOne);

	/**
	 * True if player one is to play, false if player two
	 */
	bool isPlayerOneTurn();

	int print();
	int sprint(char *);
};
