/*
 * Board.h
 *
 *  Created on: 22.04.2015
 *      Author: Andreas Molzer
 */

#pragma once
#include "types.hpp"
#include "WinMoveTable.h"

/**
 * A wrapper that holds the state of a board. Each of the 9 cells can be ticked or unticked.
 */
struct BoardField{

	inline constexpr BoardField() :bitsUsed((FieldBits) 0){
	}
	inline constexpr BoardField(const FieldBits bits):bitsUsed(bits){
	}

	FieldBits bitsUsed;

	[[gnu::const]]
	inline constexpr BoardField operator &(const BoardField& other) const {
		return BoardField(bitsUsed&other.bitsUsed);
	}

	[[gnu::const]]
	inline constexpr BoardField operator |(const BoardField& other) const {
		return BoardField(bitsUsed|other.bitsUsed);
	}

	[[gnu::const]]
	inline constexpr bool operator ==(const BoardField& other) const {
		return bitsUsed == other.bitsUsed;
	}

	inline explicit constexpr operator FieldBits() const{
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

	[[gnu::const]]
	inline constexpr static bool isWon(WonState wonState){
		return wonState > 1;
	}

	[[gnu::const]]
	inline constexpr static bool hasPlayerOneWon(WonState wonState){
		return wonState & WONP1;
	}

	[[gnu::const]]
	inline constexpr static bool hasPlayerTwoWon(WonState wonState){
		return wonState & WONP2;
	}

	[[gnu::const]]
	inline constexpr static bool canOnlyBothWin(WonState wonState){
		return wonState & ONLYBOTH;
	}

	[[gnu::const]]
	inline constexpr bool isWon() const{
		return wonState > 1;
	}

	inline constexpr TicTacBoard():wonState(0), setPlayerOne(0),
			setPlayerTwo(0), rating(0), safe(false){};

	WonState wonState;
	BoardField setPlayerOne, setPlayerTwo;
	mutable Rating rating;
	mutable bool safe;

	bool checkPlayerOneWon() const;
	bool checkPlayerTwoWon() const;

	[[gnu::const]]
	inline constexpr bool hasPlayerOneWon() const {
		return wonState & WONP1;
	}

	[[gnu::const]]
	inline constexpr bool hasPlayerTwoWon() const {
		return wonState & WONP2;
	}

	[[gnu::const]]
	inline constexpr bool canOnlyBothWin() const {
		return wonState & ONLYBOTH;
	}

	[[gnu::const]]
	inline constexpr FieldBits getBlockedFields() const{
		return (FieldBits)(setPlayerOne|setPlayerTwo);
	}

	void applyMove(bool playerOne, FieldBits field, bool triState);
	inline constexpr void undoMoveNotWonState(bool playerOne, FieldBits field){
		safe = false;
		playerOne? setPlayerOne.bitsUsed:setPlayerTwo.bitsUsed -= field;
	}
};

/**
 * The big board with 9 smaller boards
 */
struct TacTicBoard:TicTacBoard{
	TicTacBoard components[9];
	inline constexpr TacTicBoard(){
		for(int i = 0;i<9;i++){
			components[i] = TicTacBoard();
		}
	}
};

struct Fields{
	static const BoardField winBoards [9];
	static const BoardField fullBoard;
};

/**
 * Returns field bits where all bits are set that would win the board if
 * the player was playing alone. Apply & with the remaining fields and
 * you get the possible moves that win the board for a player.
 */
[[gnu::pure]]
constexpr FieldBits winMoves(BoardField set){
	return winMoveTable[(FieldBits) set];
}

[[gnu::pure]]
constexpr bool isWinBoard(const BoardField field);

