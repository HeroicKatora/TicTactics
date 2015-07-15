/*
 * Board.c
 *
 *  Created on: 11.05.2015
 *      Author: Andreas Molzer
 */
#include "Board.hpp"
#include "WinMoveTable.h"

const BoardField Fields::winBoards [] =
		{BoardField(07), BoardField(070), BoardField(0700), 	//waagrecht
		BoardField(0111), BoardField(0222), BoardField(0444),	//senkrecht
		BoardField(0124), BoardField(0421) };					//diagonal
const BoardField Fields::fullBoard = BoardField(0x777);

[[gnu::const]]
bool BoardField::getStateOfCell(unsigned index) const {
	if(index > 8) return false;
	return (0x1<<index)&bitsUsed;
}


bool TicTacBoard::checkPlayerOneWon() const {
	if(hasPlayerOneWon()) return true;
	bool full = ((setPlayerOne|setPlayerTwo) == 0x1FF);
	if(hasPlayerTwoWon() && !full) return false; //Full means possibly both
	if(wonState == ONLYBOTH && !full) return false;
	if(full)
		return isWinBoard(setPlayerTwo)?false:true;
	else
		return isWinBoard(setPlayerOne);
}

bool TicTacBoard::checkPlayerTwoWon() const{
	if(hasPlayerTwoWon()) return true;
	bool full = ((setPlayerOne|setPlayerTwo) == 0x1FF);
	if(hasPlayerOneWon() && !full) return false;
	if(wonState == ONLYBOTH && !full) return false;
	if(full)
		return isWinBoard(setPlayerOne)?false:true;
	else
		return isWinBoard(setPlayerTwo);
}

void TicTacBoard::applyMove(bool playerOne, FieldBits field, bool triState) {
	safe = false;
	if(triState){
		setPlayerOne.bitsUsed |= field;
		setPlayerTwo.bitsUsed |= field;
	}else{
		if(playerOne){
			setPlayerOne.bitsUsed |= field;
		}else{
			setPlayerTwo.bitsUsed |= field;
		}
	}
	bool p1 = checkPlayerOneWon();
	bool p2 = checkPlayerTwoWon();
	if(p1) wonState |= WONP1;
	if(p2) wonState |= WONP2;
}

[[gnu::pure]]
bool isWinBoard(const BoardField field){
	return winsTable[field.bitsUsed];
}

[[gnu::pure]]
FieldBits winMoves(BoardField set){
	return winMoveTable[(FieldBits) set];
}
