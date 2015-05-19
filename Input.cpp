/*
 * Input.cpp
 *
 *  Created on: 18.05.2015
 *      Author: Andreas Molzer
 */
#include "Input.hpp"
#include "Debug.hpp"
#include <cstdio>
#include <string.h>

std::regex reg::commentStart{":.*"};
std::regex reg::commentEnd {".*:"};
std::regex reg::asynchronous {"\\[.*\\].*"};
std::regex reg::startGame {"Start game"};
std::regex reg::startTurns {"Start turns"};
std::regex reg::endSettings {"End settings"};
std::regex reg::initP1 {"Player 1 init"};
std::regex reg::initP2 {"Player 2 init"};
std::regex reg::move {"B[1..9]F[1..9]"};
std::regex reg::engineOp {""};

/**
 * Reads the next line, but at most most characters into the string.
 * Returns the number of characters read.
 */
int readLine(char* dest, int most) {
	fgets(dest, most, stdin);
	return strlen(dest);
}

int nextProtocolLine(char *dest, int maxLineLength){
	bool anotherLine = true;
	bool inComment = false;
	int len = 0;
	while(anotherLine){
		len = readLine(dest, maxLineLength);
		if(!inComment){
			inComment = matches(dest, reg::commentStart);
		}
		if(inComment){
			inComment = !matches(dest, reg::commentEnd);
		}else{
			if(matches(dest, reg::asynchronous)){
				anotherLine = true;
			}else{
				anotherLine = false;
			}
		}
	}
	return len;
}

bool matches(char *word, char *pattern){
	std::regex reg(pattern);
	return matches(word, reg);
}

bool matches(char *word, std::regex& reg){
	return std::regex_match(word, reg);
}

MoveDescriptor getMoveDescriptor(char * source){
	MoveDescriptor desc{};
	sscanf(source, "B%1hhuF%1hu", desc.whichBoard, desc.whichField);
	desc.whichBoard--;
	desc.whichField--;
	desc.whichField = 0x1 << desc.whichField;
	return desc;
}
