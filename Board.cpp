/*
 * Board.c
 *
 *  Created on: 11.05.2015
 *      Author: Andreas Molzer
 */
#include "Board.hpp"
#include "WinMoveTable.h"

const BoardField Fields::winBoards [] =
		{BoardField(0x7u), BoardField(0x38u), BoardField(0x1Cu), 	//waagrecht
		BoardField(0x49u), BoardField(0xA2u), BoardField(0x124u),	//senkrecht
		BoardField(0x111u), BoardField(0x54u) };					//diagonal
const BoardField Fields::fullBoard = BoardField(0x1FF);


BoardField::BoardField(const FieldBits bits):bitsUsed(bits){
}

bool BoardField::getStateOfCell(unsigned index) const {
	if(index > 8) return false;
	return (0x1<<index)&bitsUsed;
}

bool TicTacBoard::hasPlayerOneWon() const {
	if(wonState & WONP1) return true;
	if(wonState == ONLYBOTH) return false;
	return isWinBoard(setPlayerOne);
}

bool TicTacBoard::hasPlayerTwoWon() const{
	if(wonState & WONP2) return true;
	if(wonState == ONLYBOTH) return false;
	return isWinBoard(setPlayerTwo);
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

BoardField BoardField::operator &(const BoardField& other) const {
	return BoardField(bitsUsed&other.bitsUsed);
}

BoardField BoardField::operator |(const BoardField& other) const {
	return BoardField(bitsUsed|other.bitsUsed);
}

bool BoardField::operator ==(const BoardField& other) const {
	return bitsUsed == other.bitsUsed;
}

BoardField::BoardField() :bitsUsed((FieldBits) 0){
}

__attribute__((const))
bool isWinBoard(const BoardField field){
	for(int i = 0;i<8;i++){
		if((field & Fields::winBoards[i])==Fields::winBoards[i]) return true;
	}
	return false;
}

__attribute__((const))
FieldBits winMoves(BoardField set){
	return winMoveTable[(FieldBits) set];
}
