/*
 * Output.hpp
 *
 *  Created on: 19.05.2015
 *      Author: Andreas Molzer
 */
#pragma once
#include "GameState.hpp"

char p1 = "One";
char p2 = "Two";

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

/**
 * These methods give synchronized access to out and err, but may take longer to complete
 */
void printOut(char *);
void printErr(char *);

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
