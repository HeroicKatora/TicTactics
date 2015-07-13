/*
 * Output.hpp
 *
 *  Created on: 19.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

#include "TTTP.hpp"
#include "GameState.hpp"

/**
 * Omit the final new line, it is automatically added.
 * These methods give synchronized access to out and err, but may take longer to complete
 * You should access these instead of directly.
 */
void printOut(const char *,...);
void printErr(const char *,...);

void printInfo(const char *, ...);
void printChannel(const char *, const char *,...);

template<unsigned level>
void printDebug(const char *, ...);

int printMove(const Move&);
int printMove(const MoveDescriptor&);

int sprintMove(char *, const Move&);
int sprintMove(char *, const MoveDescriptor&);

int printBoard(const TicTacBoard&);
/**
 * Print the board assuming it is on the index given of the game
 */
int printBoard(const TicTacBoard&, const GameState *, int index);

int printBigBoard(const TacTicBoard&);
int printBigBoard(const TacTicBoard&, const GameState *);

int sprintBoard(char *, const TicTacBoard&);
int sprintBoard(char *, const TicTacBoard&, const GameState *, int index);

int sprintBigBoard(char *, const TacTicBoard&);
int sprintBigBoard(char *, const TacTicBoard&, const GameState *);
