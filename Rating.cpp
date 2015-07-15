/*
 * Rating.cpp
 *
 *  Created on: 08.06.2015
 *      Author: Andreas Molzer
 */
#include "Rating.hpp"
#include "GameState.hpp"
#include "RatingTable.h"
#include "WinMoveTable.h"
#include <limits>

Rating Ratings::RATING_P1_WON = 100;
Rating Ratings::RATING_P2_WON = -100;
Rating Ratings::RATING_P1_GAME = std::numeric_limits<signed>::max();
Rating Ratings::RATING_P2_GAME = std::numeric_limits<signed>::min();

constexpr FieldBits midMask = 0b00010000;
constexpr FieldBits edgesMask  = 0b10101010;
constexpr FieldBits cornersMask = 0b101000101;

signed chance_bonus = 7;
signed more_bonus = 0;
signed mid_bonus = 4;
signed corner_bonus = 3;
signed edge_bonus = 2;
Rating minmaxscore = 70;

[[gnu::hot, gnu::pure]]
Rating rate(const GameState& state){
	return rate(state.gameboard);
}

[[gnu::hot, gnu::pure]]
/*Rating rate(const TicTacBoard& board){
	if(board.isWon()){
		if((board.wonState & 0x6) == 0x2) return Ratings::RATING_P1_WON;
		if((board.wonState & 0x6) == 0x4) return Ratings::RATING_P2_WON;
		return 0;
	}
	unsigned index = ((FieldBits) board.setPlayerOne << 9) + (FieldBits) board.setPlayerTwo;
	return ratingTable[index];
}*/

 Rating rate(const TicTacBoard& board){
	if(__builtin_expect(board.safe, true))
		return board.rating;

	if(board.isWon()){
		if((board.wonState & 0x6) == 0x2) return Ratings::RATING_P1_WON;
		if((board.wonState & 0x6) == 0x4) return Ratings::RATING_P2_WON;
		return 0;
	}

	FieldBits setP1 = board.setPlayerOne.bitsUsed;
	FieldBits setP2 = board.setPlayerTwo.bitsUsed;
	__builtin_prefetch(singleRatingTable+setP1);
	__builtin_prefetch(singleRatingTable+setP2);

	FieldBits chancesP1 = chancesTable[setP1];
	FieldBits chancesP2 = chancesTable[setP2];
	signed chancesDiff =  __builtin_popcount(chancesP1 & ~setP2) -__builtin_popcount(chancesP2 & ~setP1);

	Rating rate = 0;
	rate += chancesDiff * chance_bonus;
	rate += singleRatingTable[setP1];
	rate -= singleRatingTable[setP2];

	rate = std::max(-minmaxscore, std::min(minmaxscore, rate));
	board.safe = true;
	board.rating = rate;
	return rate;
}

[[gnu::hot, gnu::pure]]
Rating rate(const TacTicBoard& board){
	if(board.isWon()){
		if((board.wonState & 0x6) == 0x2) return Ratings::RATING_P1_GAME;
		if((board.wonState & 0x6) == 0x4) return Ratings::RATING_P2_GAME;
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
bool isOneBetterThan(const Rating one, const Rating two, const bool playerOne){
	if(!playerOne) return one < two;
	else return one > two;
}

[[gnu::const, gnu::hot]]
bool compare(const Rating one, const Rating two, const bool playerOne, const std::function<bool(Rating, Rating)>& f){
	if(playerOne) return f(one, two);
	return f(two, one);
}
