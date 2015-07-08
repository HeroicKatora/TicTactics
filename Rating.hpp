/*
 * Rating.hpp
 *
 *  Created on: 08.06.2015
 *      Author: Andreas Molzer
 */
#pragma once

struct Ratings{
	static signed RATING_P1_WON;
	static signed RATING_P2_WON;
	static signed RATING_P1_GAME;
	static signed RATING_P2_GAME;
};

class TicTacBoard;
class TacTicBoard;
class GameState; //Declaration, so we don't run into include loops and their problems

signed rate(const GameState& state);

signed rate(const TicTacBoard& board);

signed rate(const TacTicBoard& board);
