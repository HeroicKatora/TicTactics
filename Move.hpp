/*
 * Move.hpp
 *
 *  Created on: 11.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

#include <stack>
#include "Board.hpp"

struct Move;
struct MoveDescriptor;
class MoveHistory;

/**
 * The most minimalistic description of a move, leaves out all the undo information
 */
struct MoveDescriptor{
	BoardBits whichBoard;
	FieldBits whichField;
	MoveDescriptor():whichBoard(-1), whichField(-1){}
	MoveDescriptor(BoardBits board, FieldBits field){
		whichBoard = board;
		whichField = field;
	}
	bool operator==(MoveDescriptor& comp) const{
		return whichBoard == comp.whichBoard && whichField == comp.whichField;
	}
	inline BoardBits getBoard() const{
		return whichBoard;
	}
	inline FieldBits getField() const{
		return whichField;
	}
	unsigned getBoardIndex() const{
		return getIndexOfBoard(whichBoard);
	}
	unsigned getFieldIndex() const{
		return getIndexOfField(whichField);
	}
	bool isInvalidDefault() const{
		return whichBoard == (BoardBits)-1 && whichField == (FieldBits)-1;
	}
};

//Does not guarantee for the correctness of the moves
struct Move{
	using WonState = TicTacBoard::WonState;
	Move(BoardBits board, FieldBits field):
		winOccurred(false), boardSet(board),
		fieldSet(field){}
	Move(MoveDescriptor description):
		Move(description.getBoard(), description.getField()){};
	bool winOccurred;
	BoardBits boardSet;
	FieldBits fieldSet;
	inline void setWonState(WonState prevWonState, WonState afterMove){
		winOccurred = afterMove > prevWonState && TicTacBoard::isWon(afterMove);
	}
	//Previous win state of the board
	inline WonState getPrevWonState(WonState afterMove) const{
		return hasWinOccurred(afterMove)?afterMove&0x1:afterMove;
	}
	inline BoardBits getBoardSet() const{
		return boardSet;
	}
	inline FieldBits getFieldSet() const{
		return fieldSet;
	}
	inline unsigned getBoardIndex() const{
		return getIndexOfBoard(getBoardSet());
	}
	inline unsigned getFieldIndex() const{
		return getIndexOfField(getFieldSet());
	}
	inline bool hasWinOccurred(WonState afterMove) const{
		return winOccurred;
	}
	//Field bits if a board was conquered by P1
	inline FieldBits getWonBoardPOne(WonState afterMove) const{
		return (TicTacBoard::hasPlayerOneWon(afterMove) && hasWinOccurred(afterMove))?
				getFieldOfIndex(getFieldIndex()):0;
	}
	//Field bits if a board was conquered by P2
	inline FieldBits getWonBoardPTwo(WonState afterMove) const{
		return (TicTacBoard::hasPlayerTwoWon(afterMove) && hasWinOccurred(afterMove))?
				getFieldOfIndex(getFieldIndex()):0;
	}
};

class MoveHistory:public std::stack<Move>{
};
