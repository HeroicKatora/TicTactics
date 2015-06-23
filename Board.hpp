/*
 * Board.h
 *
 *  Created on: 22.04.2015
 *      Author: Andreas Molzer
 */

#pragma once
#include "types.hpp"

/**
 * A wrapper that holds the state of a board. Each of the 9 cells can be ticked or unticked.
 */
struct BoardField{

	BoardField();
	BoardField(const FieldBits bits);

	FieldBits bitsUsed;

	BoardField operator &(const BoardField& other) const;
	BoardField operator |(const BoardField& other) const;
	bool operator==(const BoardField& other) const;

	explicit operator FieldBits() const{
		return (FieldBits) bitsUsed;
	};
	/**
	 * Gets if the cell with the index is ticked.
	 * Probably don't use this often if you want to tune for performance
	 */
	bool getStateOfCell(unsigned index) const;
};

/**
 * A small board with 9 single fields
 */
struct TicTacBoard{

	typedef std::uint_fast8_t WonState;

	static const WonState ONLYBOTH = 0x1; //Can only win when a win occurs for both
	static const WonState WONP1 = 0x2;
	static const WonState WONP2 = 0x4;

	static const bool isWon(WonState wonState){
		return wonState > 1;
	}

	bool isWon() const{
		return wonState > 1;
	}

	WonState wonState;
	BoardField setPlayerOne, setPlayerTwo;

	bool hasPlayerOneWon() const;
	bool hasPlayerTwoWon() const;

	void applyMove(bool playerOne, FieldBits field);
};

/**
 * The big board with 9 smaller boards
 */
struct TacTicBoard:TicTacBoard{
	TicTacBoard components[9];
	TacTicBoard(){
		for(int i = 0;i<9;i++){
			components[i] = TicTacBoard();
		}
	}
};

struct Fields{
	static const BoardField winBoards [9];
	static const BoardField fullBoard;
};
