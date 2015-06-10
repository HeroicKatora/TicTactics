/*
 * Input.cpp
 *
 *  Created on: 18.05.2015
 *      Author: Andreas Molzer
 */
#include "Input.hpp"
#include "Debug.hpp"
#include <cstdio>
#include <iostream>
#include <string.h>
#include <unordered_map> //I don't know why I even use hash maps here ^^

std::regex reg::protocolStart ("TTTP v1");
std::regex reg::commentStart(":.*");
std::regex reg::commentEnd (".*:");
std::regex reg::asynchronous ("\\[.*\\].*");
std::regex reg::startGame ("Start game");
std::regex reg::startTurns ("Start turns");
std::regex reg::endSettings ("End settings");
std::regex reg::initP1 ("Player One init");
std::regex reg::initP2 ("Player Two init");
std::regex reg::move ("B[1-9]F[1-9]");
std::regex reg::engineOp ("");
std::regex reg::printReq ("Print (B[1-9]|full)");

std::unordered_map<std::string , void (*)(char *)> dynIDMap {};
/**
 * Reads the next line, but at most most characters into the string.
 * Returns the number of characters read.
 */
int readLine(char* dest, unsigned most) {
	std::string tmpL("");
	std::getline(std::cin, tmpL);
	sprintf(dest, "%.*s", most, tmpL.data());
	return strlen(dest);
}

int nextProtocolLine(char *dest, unsigned maxLineLength){
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

bool matches(const char *word, const char *pattern){
	std::regex reg(pattern);
	return matches(word, reg);
}

bool matches(const char *word, std::regex& reg){
	return std::regex_match(word, reg);
}

MoveDescriptor getMoveDescriptor(const char * source){
	BoardBits b;
	BoardBits f;
	sscanf(source, "B%1huF%1hu", &b, &f);
	b--;
	f--;
	FieldBits field = getFieldOfBoard(f);
	return MoveDescriptor{b, field};
}

bool registerDynamicHandler(const char id[], void (*fun)(char *)){
	std::string idS (id);
	return dynIDMap.insert(std::pair<std::string , void (*)(char *)>(idS, fun)).second;
}
