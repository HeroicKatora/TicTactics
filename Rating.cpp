/*
 * Rating.cpp
 *
 *  Created on: 08.06.2015
 *      Author: Andreas Molzer
 */
#include "Rating.hpp"
#include "GameState.hpp"
#include "RatingTable.h"
#include <limits>

signed Ratings::RATING_P1_WON = 100;
signed Ratings::RATING_P2_WON = -100;
signed Ratings::RATING_P1_GAME = std::numeric_limits<signed>::max();
signed Ratings::RATING_P2_GAME = std::numeric_limits<signed>::min();

[[gnu::hot, gnu::pure]]
signed rate(const GameState& state){
	return rate(state.gameboard);
}

[[gnu::hot, gnu::pure]]
signed rate(const TicTacBoard& board){
	if(board.isWon()){
		bool p1 = board.hasPlayerOneWon() && !board.hasPlayerTwoWon();
		bool p2 = board.hasPlayerTwoWon() && !board.hasPlayerOneWon();
		if(p1) return Ratings::RATING_P1_WON;
		if(p2) return Ratings::RATING_P2_WON;
		return 0;
	}
	unsigned index = ((FieldBits) board.setPlayerOne << 9) + (FieldBits) board.setPlayerTwo;
	return ratingTable[index];
}

[[gnu::hot, gnu::pure]]
signed rate(const TacTicBoard& board){
	if(board.isWon()){
		bool p1 = board.hasPlayerOneWon() && !board.hasPlayerTwoWon();
		bool p2 = board.hasPlayerTwoWon() && !board.hasPlayerOneWon();
		if(p1) return Ratings::RATING_P1_GAME;
		if(p2) return Ratings::RATING_P2_GAME;
		return 0;
	}
	signed sum = 0;
	for(int i = 0;i<9;i++){
		sum += rate(board.components[i]);
	}
	sum += rate(static_cast<const TicTacBoard&>(board));
	return sum;
}
