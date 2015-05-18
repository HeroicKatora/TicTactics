/*
 * GameState.cpp
 *
 *  Created on: 11.05.2015
 *      Author: Andreas Molzer
 */

#include "GameState.hpp"
#include <cstdio>
#include <popcntintrin.h>

void GameState::applyAndChangeMove(Move& m) {
	TicTacBoard *ticTacBoard = board.components+m.boardSet;

	m.prevWonState = ticTacBoard->wonState;
	ticTacBoard->applyMove(playerOneTurn, m.fieldSet);
	if(!TicTacBoard::isWon(m.prevWonState)){
		//Potential new win
		BoardBits wonBoard = 0x1 << m.boardSet;
		if(ticTacBoard->hasPlayerOneWon()) board.applyMove(true, wonBoard);
		if(ticTacBoard->hasPlayerTwoWon()) board.applyMove(false, wonBoard);
	}
	playerOneTurn = !playerOneTurn;
}

void GameState::undoMove(Move& m){
	playerOneTurn = !playerOneTurn;
	TicTacBoard *ticTacBoard = board.components+m.boardSet;
	ticTacBoard->wonState = m.prevWonState;
	if(playerOneTurn) ticTacBoard->setPlayerOne.bitsUsed -= m.fieldSet;
	else ticTacBoard->setPlayerTwo.bitsUsed -= m.fieldSet;
	board.setPlayerOne.bitsUsed -= m.boardWinOne;
	board.setPlayerTwo.bitsUsed -= m.boardWinTwo;
}

bool GameState::playMove(Move m) {
	if(isValidMove(m)){
		applyAndChangeMove(m);
		searcher.notifyMoveMade(m);
		history.push(m);
		return true;
	}else{
		printf("Invalid move Board %u Fieldbits %u\n", m.boardSet, m.fieldSet);
		return false;
	}
}

void GameState::undoMove() {
	Move m = history.top();
	undoMove(m);
	searcher.notifyUndo(m);
	history.pop();
}

MoveSuggestion GameState::getBestKnownMove() {
	return searcher.getBestKnownMove();
}

bool GameState::isValidMove(Move& m) {
	//Trivial invalid
	if(m.boardSet >= 9) return false; //Invalid board index
	if(m.fieldSet >= 0x100) return false; //Invalid field index
	if(__builtin_popcount(m.fieldSet) != 1)return false; //Invalid field descriptor (more than one bit set)

	//More complex invalid
	bool valid = true;
	BoardBits setInTarget = board.components[m.boardSet].setPlayerOne |
			board.components[m.boardSet].setPlayerTwo;
	valid &= ((m.fieldSet & setInTarget) == 0);  //Is not on top of a set field
	if(history.empty()){
		valid &= m.boardSet != 4 || m.fieldSet != 0x10; //not the mid mid field
	}else{
		Move& previousMove = history.top();
		BoardBits backMove = getFieldOfBoard(previousMove.boardSet);
		valid &= m.fieldSet != backMove; //Don't make back move
		if((setInTarget | backMove) < 0x1FF){
			//Has a move free, has to make a move in field
			valid &= getBoardOfField(previousMove.fieldSet) == m.boardSet;
		}
	}
	return valid;
}

bool GameState::isWon(){
	return board.isWon(board.wonState);
}

bool GameState::isPlayerOneTurn(){
	return playerOneTurn;
}
