/*
 * Input.hpp
 *
 *  Created on: 18.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

#include "Move.hpp"
#include <regex>

std::regex reg_commentStart (":.*");
std::regex reg_commentEnd (".*:");
std::regex reg_startGame("Start Game");
std::regex reg_endSettings("End settings");
std::regex reg_initP1("Player 1 init");
std::regex reg_initP2("Player 2 init");
std::regex reg_move("B[1..9]F[1..9]");
std::regex reg_engineOp("");

int readLine(char * dest, int most);

/**
 * Skips all lines marked as optional and then returns the next line that is not
 */
int nextProtocolLine(char * dest, int maxLineLength);

bool matches(char * , char * pattern);

bool matches(char *word, std::regex& reg);

MoveDescriptor getMoveDescriptor(char *);
