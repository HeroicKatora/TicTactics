/*
 * Board.c
 *
 *  Created on: 11.05.2015
 *      Author: Andreas Molzer
 */
#include "Board.hpp"

BoardField::BoardField(FieldBits bits){
	bitsUsed = bits;
}

bool BoardField::getStateOfCell(unsigned index) {
	if(index > 8) return false;
	return (0x1<<index)&bitsUsed;
}

bool TicTacBoard::hasPlayerOneWon() {
	if(wonState & WONP1) return true;
	if(wonState == ONLYBOTH) return false;
	for(int i = 0;i<8;i++){
		if((setPlayerOne & winBoards[i])==winBoards[i]) return true;
	}
	return false;
}

bool TicTacBoard::hasPlayerTwoWon() {
	if(wonState & WONP2) return true;
	if(wonState == ONLYBOTH) return false;
	for(int i = 0;i<8;i++){
		if((setPlayerTwo & winBoards[i])==winBoards[i]) return true;
	}
	return false;
}

void TicTacBoard::applyMove(bool playerOne, FieldBits field) {
	if(playerOne){
		setPlayerOne.bitsUsed |= field;
		if(hasPlayerOneWon()) wonState |= WONP1;
	}else{
		setPlayerTwo.bitsUsed |= field;
		if(hasPlayerOneWon()) wonState |= WONP2;
	}
}

BoardField BoardField::operator &(const BoardField& other) {
	return BoardField(bitsUsed&other.bitsUsed);
}

BoardField BoardField::operator |(const BoardField& other) {
	return BoardField(bitsUsed|other.bitsUsed);
}

bool BoardField::operator ==(const BoardField& other) {
	return bitsUsed == other.bitsUsed;
}

BoardField::BoardField() :bitsUsed(0){
}
