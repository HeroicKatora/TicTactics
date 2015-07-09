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

	Move();
	Move(BoardBits board, FieldBits field);
	Move(MoveDescriptor description);

	bool winOccurred;
	bool onlyBothOccurred;
	std::uint8_t set;

	bool isInvalidDefault() const;
	bool operator==(Move& other);

	void setWonState(WonState prevWonState, WonState afterMove);
	WonState getPrevWonState(WonState afterMove) const;
	void setIndices(unsigned boardIndex, unsigned fieldIndex);
	BoardBits getBoardSet() const;
	FieldBits getFieldSet() const;
	unsigned getBoardIndex() const;
	unsigned getFieldIndex() const;
	bool hasWinOccurred(WonState afterMove) const;
	bool hasOnlyBothOccurred(WonState afterMove) const;

	//Field bits if a board was conquered by P1
	FieldBits getWonBoardPOne(WonState afterMove) const;
	//Field bits if a board was conquered by P2
	FieldBits getWonBoardPTwo(WonState afterMove) const;

} ;

class MoveHistory:public std::stack<Move>{
};
