/*
 * GameState.cpp
 *
 *  Created on: 11.05.2015
 *      Author: Andreas Molzer
 */

#include "GameState.hpp"
#include <cstdio>
#include <popcntintrin.h>
#include "Output.hpp"

[[gnu::hot]]
void GameState::applyAndChangeMove(Move& m) {
	TicTacBoard *ticTacBoard = gameboard.components+getIndexOfBoard(m.getBoardSet());

	TicTacBoard::WonState prevWon = ticTacBoard->wonState;
	ticTacBoard->applyMove(playerOneTurn, m.getFieldSet(), false);
	m.setWonState(prevWon, ticTacBoard->wonState);
	if(m.hasWinOccurred(ticTacBoard->wonState)){
		TicTacBoard::WonState algorithm = ticTacBoard->wonState&TicTacBoard::ONLYBOTH;
		if(algorithm != prevWon){
		printBigBoard(gameboard);
			//printBoard(*ticTacBoard);
			printChannel(TTTPConst::channelDebug, "WinState prev, now: %u, %u", prevWon, ticTacBoard->wonState);
			printChannel(TTTPConst::channelDebug, "Real/Algorithm output: %u/%u", prevWon, algorithm);
		}
	}else{
		TicTacBoard::WonState algorithm = ticTacBoard->wonState;
		if(algorithm != prevWon){
			printBigBoard(gameboard);
			//printBoard(*ticTacBoard);
			printChannel(TTTPConst::channelDebug, "Non WinState prev, now: %u, %u", prevWon, ticTacBoard->wonState);
			printChannel(TTTPConst::channelDebug, "Real/Algorithm output: %u/%u", prevWon, algorithm);
		}
	}
	if(!TicTacBoard::isWon(prevWon)){
		//Potential new win
		BoardBits wonBoard = getFieldOfBoard(m.getBoardSet());
		bool p1won = ticTacBoard->hasPlayerOneWon();
		bool p2won = ticTacBoard->hasPlayerTwoWon();
		if(p1won && p2won){
			gameboard.applyMove(playerOneTurn, wonBoard, true);
		}else if(p1won){
			gameboard.applyMove(true, wonBoard, false);
		}else if(p2won){
			gameboard.applyMove(false, wonBoard, false);
		}

	}
	playerOneTurn = !playerOneTurn;
}

void GameState::_applyMove(Move&& m){
	applyAndChangeMove(m);
}

[[gnu::hot]]
void GameState::undoMove(Move& m) {
	TicTacBoard *ticTacBoard = gameboard.components+m.getBoardSet();
	playerOneTurn = !playerOneTurn;
	gameboard.setPlayerOne.bitsUsed -= m.getWonBoardPOne(ticTacBoard->wonState);
	gameboard.setPlayerTwo.bitsUsed -= m.getWonBoardPTwo(ticTacBoard->wonState);
	if(TicTacBoard::isWon(gameboard.wonState)){
		gameboard.wonState ^= ticTacBoard->wonState;
	}
	if(playerOneTurn) ticTacBoard->setPlayerOne.bitsUsed -= m.getFieldSet();
	else ticTacBoard->setPlayerTwo.bitsUsed -= m.getFieldSet();
	ticTacBoard->wonState = m.getPrevWonState(ticTacBoard->wonState);
}

bool GameState::playMove(Move m) {
	if(isValidMove(m)){
		applyAndChangeMove(m);
		if(searcher)
			searcher->notifyMoveMade(m);
		history.push(m);
		return true;
	}else{
		char moveS[30];
		sprintMove(moveS, m);
		printInfo("Invalid move %s\n", moveS);
		return false;
	}
}

void GameState::undoMove() {
	Move m = history.top();
	undoMove(m);
	if(searcher)
		searcher->notifyUndo(m);
	history.pop();
}

[[gnu::const]]
bool GameState::isValidMove(Move& m) const {
	//Trivial invalid
	BoardBits boardB = m.getBoardSet();
	FieldBits fieldB = m.getFieldSet();
	if(boardB > 8) return false; //Invalid board index
	if(fieldB > 0x100) return false; //Invalid field index
	if(__builtin_popcount(fieldB) != 1) return false; //Invalid field descriptor (more than one bit set)

	//More complex invalid
	bool valid = true;
	FieldBits setInTarget = ((const FieldBits)gameboard.components[boardB].setPlayerOne) |
			((const FieldBits) gameboard.components[boardB].setPlayerTwo);
	valid &= ((fieldB & setInTarget) == 0);  //Is not on top of a set field
	if(history.empty()){
		valid &= boardB != 4 || fieldB != 0x10; //not the mid mid field
		valid &= (winMoves(gameboard.components[boardB].setPlayerOne) & fieldB) == 0; //Not a board win
	}else{
		const Move& previousMove = history.top();
		FieldBits backMove = getFieldOfBoard(previousMove.getBoardSet());
		valid &= fieldB != backMove; //Don't make back move
		if((setInTarget | backMove) < 0x1FF){
			//Has a move free, has to make a move in field
			valid &= getBoardOfField(previousMove.getFieldSet()) == boardB;
		}
	}
	return valid;
}

[[gnu::const]]
bool GameState::isWon() const{
	return gameboard.isWon(gameboard.wonState);
}

[[gnu::const]]
bool GameState::hasWon(bool playerOne) const{
	if(playerOne) return gameboard.checkPlayerOneWon();
	else return gameboard.checkPlayerTwoWon();
}

[[gnu::const, gnu::hot]]
bool GameState::isPlayerOneTurn()const{
	return playerOneTurn;
}

[[gnu::const]]
unsigned isIn(MoveDescriptor m, MoveDescriptor playerMoves [9]){
	unsigned count = 0;
	for(int i = 0;i<9;i++){
		if(m == playerMoves[i]) count++;
	}
	return count;
}

[[gnu::const]]
InitResult GameState::checkSingleValidity(MoveDescriptor playerMoves[9]){
	InitResult result{};
	result.type = PASSED;

	//Check validity of each move, check duplicates, too many in the same board, all on
	//different field
	char playerBoards[9] = {};
	char playerFields[9] = {};
	for(int i = 0;i<9;i++){
		MoveDescriptor m = playerMoves[i];
		Move move = Move(m);
		if(!isValidMove(move)){
			if(m.getBoardIndex() == 4 && m.getFieldIndex() == 4){
				result.type = MIDDLE_MOVE;
			}else{
				result.type = INVALID;
			}
			result.info.index = i;
		}
		if(isIn(playerMoves[i], playerMoves) != 1){
			result.type = DUPLICATE;
			return result;
		}
		unsigned board = m.getBoardIndex();
		unsigned field = m.getFieldIndex();
		if(playerBoards[board] >= 2){
			result.type = BOARD_OVERFLOW;
			result.info.index = i;
			return result;
		}
		playerBoards[board]++;
		if(playerFields[field] >= 1){
			result.type = FIELD_OVERFLOW;
			result.info.index = i;
			return result;
		}
		playerFields[field]++;
	}
	return result;
}

InitResult GameState::initializeWithMoves(MoveDescriptor playerOne[9], MoveDescriptor playerTwo[9]){
	InitResult result{};

	//Check if in init phase
	if(history.size() > 0){
		result.type = NOT_DURING_INIT;
		return result;
	}

	result = checkSingleValidity(playerOne);
	if(result.type != PASSED){
		result.info.playerOne = true;
		return result;
	}

	result = checkSingleValidity(playerTwo);
	if(result.type != PASSED){
		result.info.playerOne = false;
		return result;
	}

	//All valid, lets carry it out
	for(int i = 0;i<9;i++){
		if(!isIn(playerOne[i], playerTwo)) _applyMove(Move(playerOne[i]));
		if(!isIn(playerTwo[i], playerOne)) _applyMove(Move(playerTwo[i]));
	}
	result.type = InitResultType::PASSED;
	return result;
}

int GameState::print() const{
	return printBigBoard(gameboard, this);
}

int GameState::sprint(char * dest) const{
	return sprintBigBoard(dest, gameboard, this);
}

int GameState::print(int index) const{
	return printBoard(gameboard.components[index], this, index);
}

int GameState::sprint(int index, char * dest) const{
	return sprintBoard(dest, gameboard.components[index], this, index);
}

void GameState::start() {
}

Searcher* GameState::getSearcher() const{
	return searcher;
}
