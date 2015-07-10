/*
 * Rating.hpp
 *
 *  Created on: 08.06.2015
 *      Author: Andreas Molzer
 */
#pragma once
#include "types.hpp"

struct Ratings{
	static Rating RATING_P1_WON;
	static Rating RATING_P2_WON;
	static Rating RATING_P1_GAME;
	static Rating RATING_P2_GAME;
};

class TicTacBoard;
class TacTicBoard;
class GameState; //Declaration, so we don't run into include loops and their problems

Rating rate(const GameState& state);

Rating rate(const TicTacBoard& board);

Rating rate(const TacTicBoard& board);

inline Rating minRating(bool playerOne){
	return playerOne?Ratings::RATING_P2_GAME:Ratings::RATING_P1_GAME;
}
inline Rating maxRating(bool playerOne){
	return playerOne?Ratings::RATING_P1_GAME:Ratings::RATING_P2_GAME;
}
bool isOneBetterThan(Rating one, Rating two, bool playerOne);
