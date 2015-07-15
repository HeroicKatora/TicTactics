/*
 * Move.cpp
 *
 *  Created on: 09.07.2015
 *      Author: Andreas Molzer
 */
#include "Move.hpp"

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
