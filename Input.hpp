/*
 * Input.hpp
 *
 *  Created on: 18.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

#include "TTTP.hpp"
#include "Move.hpp"
#include <regex>

struct reg{
	static std::regex protocolStart;
	static std::regex commentStart;
	static std::regex commentEnd;
	static std::regex asynchronous;
	static std::regex startGame;
	static std::regex startTurns;
	static std::regex endSettings;
	static std::regex initP1;
	static std::regex initP2;
	static std::regex move;
	static std::regex engineOp;
	static std::regex printReq;
};

int readLine(char * dest, int most);

/**
 * Skips all lines marked as optional and then returns the next line that is not
 */
int nextProtocolLine(char * dest, int maxLineLength);

bool matches(const char * word, const char * pattern);

bool matches(const char * word, std::regex& reg);

/**
 * Tries to extract a move descriptor from the given string
 */
MoveDescriptor getMoveDescriptor(const char *);

bool registerDynamicHandler(const char [], void (*)(char *));
