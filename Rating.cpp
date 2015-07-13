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

Rating Ratings::RATING_P1_WON = 100;
Rating Ratings::RATING_P2_WON = -100;
Rating Ratings::RATING_P1_GAME = std::numeric_limits<signed>::max();
Rating Ratings::RATING_P2_GAME = std::numeric_limits<signed>::min();

[[gnu::hot, gnu::pure]]
Rating rate(const GameState& state){
	return rate(state.gameboard);
}

[[gnu::hot, gnu::pure]]
Rating rate(const TicTacBoard& board){
	if(board.isWon()){
		if((board.wonState & 0x6) == 0x2) return Ratings::RATING_P1_WON;
		if((board.wonState & 0x6) == 0x4) return Ratings::RATING_P2_WON;
		return 0;
	}
	unsigned index = ((FieldBits) board.setPlayerOne << 9) + (FieldBits) board.setPlayerTwo;
	return ratingTable[index];
}

[[gnu::hot, gnu::pure]]
Rating rate(const TacTicBoard& board){
	if(board.isWon()){
		if((board.wonState & 0x6) == 0x2) return Ratings::RATING_P1_WON;
		if((board.wonState & 0x6) == 0x4) return Ratings::RATING_P2_WON;
		return 0;
	}
	signed sum = 0;
	for(int i = 0;i<9;i++){
		sum += rate(board.components[i]);
	}
	sum += rate(static_cast<const TicTacBoard&>(board)) * 5;
	return sum;
}

[[gnu::const, gnu::hot]]
bool isOneBetterThan(Rating one, Rating two, bool playerOne){
	if(!playerOne) return one < two;
	else return one > two;
}

[[gnu::const, gnu::hot]]
bool compare(Rating one, Rating two, bool playerOne, const std::function<bool(Rating, Rating)>& f){
	if(playerOne) return f(one, two);
	return f(two, one);
}

