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
	MoveDescriptor(BoardBits board, FieldBits field){
		whichBoard = board;
		whichField = field;
	}
	bool operator==(MoveDescriptor& comp){
		return whichBoard == comp.whichBoard && whichField == comp.whichField;
	}
	inline BoardBits getBoard(){
		return whichBoard;
	}
	inline FieldBits getField(){
		return whichField;
	}
	unsigned getBoardIndex(){
		return getIndexOfBoard(whichBoard);
	}
	unsigned getFieldIndex(){
		return getIndexOfField(whichField);
	}
};

//64 bit long representing a move made. Does not guarantee for the correctness of the moves
struct Move{
	using WonState = TicTacBoard::WonState;
	Move(BoardBits board, FieldBits field):
		prevWonState(0), boardSet(board),
		fieldSet(field), boardWinOne(0), boardWinTwo(0){}
	Move(MoveDescriptor description):
		Move(description.getBoard(), description.getField()){};
	WonState prevWonState;
	BoardBits boardSet;
	FieldBits fieldSet;
	FieldBits boardWinOne;
	FieldBits boardWinTwo;
	//Previous win state of the board
	inline WonState getPrevWonState(WonState afterMove){
		return prevWonState;
	}
	inline BoardBits getBoardSet(){
		return boardSet;
	}
	inline FieldBits getFieldSet(){
		return fieldSet;
	}
	//Field bits if a board was conquered by P1
	inline FieldBits getWonBoardPOne(){
		return boardWinOne;
	}
	//Field bits if a board was conquered by P2
	inline FieldBits getWonBoardPTwo(){
		return boardWinTwo;
	}
};

class MoveHistory:public std::stack<Move>{
};
