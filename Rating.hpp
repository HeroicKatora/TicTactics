/*
 * Rating.hpp
 *
 *  Created on: 08.06.2015
 *      Author: Andreas Molzer
 */
#pragma once
#include "types.hpp"
#include <functional>

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

[[gnu::const]]
inline Rating minRating(const bool playerOne){
	return playerOne?Ratings::RATING_P2_GAME:Ratings::RATING_P1_GAME;
}
[[gnu::const]]
inline Rating maxRating(const bool playerOne){
	return playerOne?Ratings::RATING_P1_GAME:Ratings::RATING_P2_GAME;
}
[[gnu::const]]
bool isOneBetterThan(Rating one, Rating two, bool playerOne);

[[gnu::const]]
bool compare(Rating one, Rating two, bool playerOne, const std::function<bool(Rating, Rating)>& f);
