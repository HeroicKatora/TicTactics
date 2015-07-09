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
	if(hasPlayerTwoWon()) return false;
	if(hasPlayerOneWon()) return true;
	if(wonState == ONLYBOTH && !((setPlayerOne|setPlayerTwo) == 0x1FF)) return false;
	return isWinBoard(setPlayerOne);
}

bool TicTacBoard::checkPlayerTwoWon() const{
	if(hasPlayerOneWon()) return false;
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

void TicTacBoard::applyMove(bool playerOne, FieldBits field, bool triState) {
	if(triState){
		setPlayerOne.bitsUsed |= field;
		setPlayerTwo.bitsUsed |= field;
		bool p1 = checkPlayerOneWon();
		bool p2 = checkPlayerTwoWon();
		if(p1 && p2) wonState |= WONP1|WONP2;
		if(p1) wonState |= WONP1;
		if(p2) wonState |= WONP2;
	}else{
		if(playerOne){
			setPlayerOne.bitsUsed |= field;
			if(checkPlayerOneWon()) wonState |= WONP1;
		}else{
			setPlayerTwo.bitsUsed |= field;
			if(checkPlayerTwoWon()) wonState |= WONP2;
		}
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
