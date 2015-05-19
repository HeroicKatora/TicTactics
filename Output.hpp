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
void printOut(char *,...);
void printErr(char *,...);

void infoPrint(char *, ...);

int printMove(Move&);
int printMove(MoveDescriptor&);

int sprintMove(char *, Move&);
int sprintMove(char *, MoveDescriptor&);

int printBoard(TicTacBoard&);
int printBigBoard(TacTicBoard&);
int printBigBoard(TacTicBoard&, GameState *);

int sprintBoard(char *, TicTacBoard&);
int sprintBigBoard(char *, TacTicBoard&);
int sprintBigBoard(char *, TacTicBoard&, GameState *);
