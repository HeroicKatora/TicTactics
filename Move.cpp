/*
 * Move.cpp
 *
 *  Created on: 09.07.2015
 *      Author: Andreas Molzer
 */
#include "Move.hpp"

Move::Move():winOccurred(false), onlyBothOccurred(false),set(~0){};
Move::Move(BoardBits board, FieldBits field):
		winOccurred(false), onlyBothOccurred(false),
		set((getIndexOfBoard(board)<<4)+getIndexOfField(field)){};
Move::Move(MoveDescriptor description):
		Move(description.getBoard(), description.getField()){};

[[gnu::const]]
bool Move::operator==(Move& other){
	return other.set == set;
}

void Move::setWonState(WonState prevWonState, WonState afterMove){
	winOccurred = !TicTacBoard::isWon(prevWonState) && TicTacBoard::isWon(afterMove);
	onlyBothOccurred = afterMove > 0 && prevWonState == 0;
}

[[gnu::const]]
Move::WonState Move::getPrevWonState(WonState afterMove) const{
	WonState prev = hasWinOccurred(afterMove)?afterMove&TicTacBoard::ONLYBOTH:afterMove;
	prev = hasOnlyBothOccurred(afterMove)?0:prev;
	return prev;
}
void Move::setIndices(unsigned boardIndex, unsigned fieldIndex){
	set = (boardIndex<<4)+fieldIndex;
}

[[gnu::const]]
BoardBits Move::getBoardSet() const{
	return getBoardOfIndex(getBoardIndex());
}

[[gnu::const]]
FieldBits Move::getFieldSet() const{
	return getFieldOfIndex(getFieldIndex());
}

[[gnu::const]]
unsigned Move::getBoardIndex() const{
	return set >> 4;
}

[[gnu::const]]
unsigned Move::getFieldIndex() const{
	return set & 0xF;
}

[[gnu::const]]
bool Move::hasWinOccurred(WonState afterMove) const{
	return winOccurred;
}

[[gnu::const]]
bool Move::hasOnlyBothOccurred(WonState afterMove) const{
	return onlyBothOccurred;
}

[[gnu::const]]
//Field bits if a board was conquered by P1
FieldBits Move::getWonBoardPOne(WonState afterMove) const{
	return (TicTacBoard::hasPlayerOneWon(afterMove) && hasWinOccurred(afterMove))?
			getFieldOfIndex(getBoardIndex()):0;
}

[[gnu::const]]
//Field bits if a board was conquered by P2
FieldBits Move::getWonBoardPTwo(WonState afterMove) const{
	return (TicTacBoard::hasPlayerTwoWon(afterMove) && hasWinOccurred(afterMove))?
			getFieldOfIndex(getBoardIndex()):0;
}

[[gnu::const]]
bool Move::isInvalidDefault() const{
	return set == (decltype(set))-1;
}
