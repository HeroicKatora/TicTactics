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
struct [[gnu::packed]] Move{
	using WonState = TicTacBoard::WonState;

	Move():winOccurred(false), onlyBothOccurred(false),set(~0){};
	Move(BoardBits board, FieldBits field):// prev(),
		winOccurred(false), onlyBothOccurred(false),
		set((getIndexOfBoard(board)<<4)+getIndexOfField(field)){}
	Move(MoveDescriptor description):
		Move(description.getBoard(), description.getField()){};
	//WonState prev;
	bool winOccurred;
	bool onlyBothOccurred;
	std::uint8_t set;
	inline void setWonState(WonState prevWonState, WonState afterMove){
		winOccurred = !TicTacBoard::isWon(prevWonState) && TicTacBoard::isWon(afterMove);
		onlyBothOccurred = afterMove > 0 && prevWonState == 0;
	}
	//Previous win state of the board
	inline WonState getPrevWonState(WonState afterMove) const{
		WonState prev = hasWinOccurred(afterMove)?afterMove&TicTacBoard::ONLYBOTH:afterMove;
		prev = hasOnlyBothOccurred(afterMove)?0:prev;
		return prev;
	}
	inline void setIndices(unsigned boardIndex, unsigned fieldIndex){
		set = (boardIndex<<4)+fieldIndex;
	}
	inline BoardBits getBoardSet() const{
		return getBoardOfIndex(getBoardIndex());
	}
	inline FieldBits getFieldSet() const{
		return getFieldOfIndex(getFieldIndex());
	}
	inline unsigned getBoardIndex() const{
		return set >> 4;
	}
	inline unsigned getFieldIndex() const{
		return set & 0xF;
	}
	inline bool hasWinOccurred(WonState afterMove) const{
		return winOccurred;
	}
	inline bool hasOnlyBothOccurred(WonState afterMove) const{
		return onlyBothOccurred;
	}
	//Field bits if a board was conquered by P1
	inline FieldBits getWonBoardPOne(WonState afterMove) const{
		return (TicTacBoard::hasPlayerOneWon(afterMove) && hasWinOccurred(afterMove))?
				getFieldOfIndex(getBoardIndex()):0;
	}
	//Field bits if a board was conquered by P2
	inline FieldBits getWonBoardPTwo(WonState afterMove) const{
		return (TicTacBoard::hasPlayerTwoWon(afterMove) && hasWinOccurred(afterMove))?
				getFieldOfIndex(getBoardIndex()):0;
	}
	bool isInvalidDefault() const{
		return set == ~0;
	}
} ;

class MoveHistory:public std::stack<Move>{
};
