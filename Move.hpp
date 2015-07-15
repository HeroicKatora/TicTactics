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
	inline constexpr MoveDescriptor():whichBoard(-1), whichField(-1){};
	inline constexpr MoveDescriptor(BoardBits board, FieldBits field):whichBoard(board), whichField(field){};

	[[gnu::const]]
	inline bool operator==(MoveDescriptor& comp) const{
		return whichBoard == comp.whichBoard && whichField == comp.whichField;
	}
	[[gnu::const]]
	inline constexpr BoardBits getBoard() const{
		return whichBoard;
	}
	[[gnu::const]]
	inline constexpr FieldBits getField() const{
		return whichField;
	}
	[[gnu::const]]
	inline constexpr unsigned getBoardIndex() const{
		return getIndexOfBoard(whichBoard);
	}
	[[gnu::const]]
	inline constexpr unsigned getFieldIndex() const{
		return getIndexOfField(whichField);
	}
	[[gnu::const]]
	inline constexpr bool isInvalidDefault() const{
		return whichBoard == (BoardBits)-1 && whichField == (FieldBits)-1;
	}
};

//Does not guarantee for the correctness of the moves
struct [[gnu::packed]] Move{
	using WonState = TicTacBoard::WonState;

	inline constexpr Move():winOccurred(false), onlyBothOccurred(false),set(~0){};
	inline constexpr Move(BoardBits board, FieldBits field):
			winOccurred(false), onlyBothOccurred(false),
			set((getIndexOfBoard(board)<<4)+getIndexOfField(field)){};
	inline constexpr Move(MoveDescriptor description):
			Move(description.getBoard(), description.getField()){};

	[[gnu::const]]
	inline constexpr bool operator==(Move& other){
		return other.set == set;
	}

	bool winOccurred;
	bool onlyBothOccurred;
	std::uint8_t set;


	[[gnu::const]]
	inline constexpr bool isInvalidDefault() const{
		return set == (decltype(set))-1;
	}

	void setWonState(WonState prevWonState, WonState afterMove);
	WonState getPrevWonState(WonState afterMove) const;
	void setIndices(unsigned boardIndex, unsigned fieldIndex);

	[[gnu::const]]
	inline constexpr BoardBits getBoardSet() const{
		return getBoardOfIndex(getBoardIndex());
	}

	[[gnu::const]]
	inline constexpr FieldBits getFieldSet() const{
		return getFieldOfIndex(getFieldIndex());
	}

	[[gnu::const]]
	inline constexpr unsigned getBoardIndex() const{
		return set >> 4;
	}

	[[gnu::const]]
	inline constexpr unsigned getFieldIndex() const{
		return set & 0xF;
	}

	[[gnu::const]]
	inline constexpr bool hasWinOccurred(WonState afterMove) const{
		return winOccurred;
	}

	[[gnu::const]]
	inline constexpr bool hasOnlyBothOccurred(WonState afterMove) const{
		return onlyBothOccurred;
	}


	//Field bits if a board was conquered by P1
	FieldBits getWonBoardPOne(WonState afterMove) const;
	//Field bits if a board was conquered by P2
	FieldBits getWonBoardPTwo(WonState afterMove) const;

} ;

class MoveHistory:public std::stack<Move>{
};
