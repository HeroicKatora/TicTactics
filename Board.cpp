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

__attribute__((const))
bool TicTacBoard::isWon(WonState wonState){
	return wonState > 1;
}
__attribute__((const))
bool TicTacBoard::hasPlayerOneWon(WonState wonState){
	return wonState & WONP1;
}

__attribute__((const))
bool TicTacBoard::hasPlayerTwoWon(WonState wonState){
	return wonState & WONP2;
}

__attribute__((const))
bool TicTacBoard::canOnlyBothWin(WonState wonState){
	return wonState & ONLYBOTH;
}

bool TicTacBoard::isWon() const{
	return wonState > 1;
}

bool BoardField::getStateOfCell(unsigned index) const {
	if(index > 8) return false;
	return (0x1<<index)&bitsUsed;
}

bool TicTacBoard::checkPlayerOneWon() const {
	if(hasPlayerOneWon()) return true;
	if(wonState == ONLYBOTH && !((setPlayerOne|setPlayerTwo) == 0x1FF)) return false;
	return isWinBoard(setPlayerOne);
}

bool TicTacBoard::checkPlayerTwoWon() const{
	if(hasPlayerTwoWon()) return true;
	if(wonState == ONLYBOTH && !((setPlayerOne|setPlayerTwo) == 0x1FF)) return false;
	return isWinBoard(setPlayerTwo);
}

bool TicTacBoard::hasPlayerOneWon() const {
	return wonState & WONP1;
}

bool TicTacBoard::hasPlayerTwoWon() const {
	return wonState & WONP2;
}

bool TicTacBoard::canOnlyBothWin() const {
	return wonState & ONLYBOTH;
}

void TicTacBoard::applyMove(bool playerOne, FieldBits field) {
	if(playerOne){
		setPlayerOne.bitsUsed |= field;
		if(checkPlayerOneWon()) wonState |= WONP1;
	}else{
		setPlayerTwo.bitsUsed |= field;
		if(checkPlayerTwoWon()) wonState |= WONP2;
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

FieldBits TicTacBoard::getBlockedFields() const{
	return (FieldBits)(setPlayerOne|setPlayerTwo);
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
