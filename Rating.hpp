/*
 * Rating.hpp
 *
 *  Created on: 08.06.2015
 *      Author: Andreas Molzer
 */
#pragma once

class TicTacBoard;
class TacTicBoard;
class GameState; //Declaration, so we don't run into include loops and their problems

float rate(GameState& state);

float rate(TicTacBoard& board);

float rate(TacTicBoard& board);
