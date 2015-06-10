/*
 * Rating.cpp
 *
 *  Created on: 08.06.2015
 *      Author: Andreas Molzer
 */
#include "Rating.hpp"
#include "GameState.hpp"
#include "RatingTable.h"

float RATING_P1_WON = 5;
float RATING_P2_WON = -5;

float rate(GameState& state){
	return rate(state.board);
}

float rate(TicTacBoard& board){
	if(board.isWon()){
		bool p1 = board.hasPlayerOneWon() && !board.hasPlayerTwoWon();
		bool p2 = board.hasPlayerTwoWon() && !board.hasPlayerOneWon();
		if(p1) return RATING_P1_WON;
		if(p2) return RATING_P2_WON;
		return 0;
	}
	unsigned index = ((FieldBits) board.setPlayerOne << 9) + (FieldBits) board.setPlayerTwo;
	return ratingTable[index];
}

float rate(TacTicBoard& board){
	float sum = 0;
	for(int i = 0;i<9;i++){
		sum += rate(board.components[i]);
	}
	sum += rate(static_cast<TicTacBoard&>(board));
	return sum;
}
