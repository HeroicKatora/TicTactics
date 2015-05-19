/*
 * Output.hpp
 *
 *  Created on: 19.05.2015
 *      Author: Andreas Molzer
 */
#pragma once
#include "GameState.hpp"

char VALID = '_';
char EMPTY = '~';

char Set1W1 = 'X';
char Set1WA = 'x';

char Set3W0	= '%';
char Set3W1 = '+';
char Set3W3 = '&';
char Set3W2 = '0';

char Set2W2 = 'O';
char Set2WA = 'o';

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
