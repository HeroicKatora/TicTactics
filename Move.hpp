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
	bool operator==(MoveDescriptor& comp){
		return whichBoard == comp.whichBoard && whichField == comp.whichField;
	}
	unsigned getBoardIndex(){
		return whichBoard;
	}
	unsigned getFieldIndex(){
		return getBoardOfField(whichField);
	}
};

//64 bit long representing a move made. Does not guarantee for the correctness of the moves
struct Move{
	using WonState = TicTacBoard::WonState;
	Move(BoardBits board, FieldBits field):
		prevWonState(0), boardSet(board),
		fieldSet(field), boardWinOne(0), boardWinTwo(0){}
	Move(MoveDescriptor description):
		Move(description.whichBoard, description.whichField){};
	WonState prevWonState;	//Previous win state of the board
	BoardBits boardSet; 	//0..8, index of board
	FieldBits fieldSet;	//0000 000$ $$$$ $$$$ bit set according to the target field
	FieldBits boardWinOne;	//0000 000$ $$$$ $$$$ bit set if a board was conquered by P1
	FieldBits boardWinTwo;	//0000 000$ $$$$ $$$$ bit set if a board was conquered by P2
};

class MoveHistory:public std::stack<Move>{
};
