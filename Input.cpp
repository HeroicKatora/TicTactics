/*
 * Input.cpp
 *
 *  Created on: 18.05.2015
 *      Author: Andreas Molzer
 */
#include "Input.hpp"
#include <string>
#include <iostream>

MoveDescriptor getMoveDescriptionFromInput(const char * input){
	BoardBits board;
	FieldBits field;
	printf(input);
	printf("->Input\n");
	while(sscanf(input, " ")); //Skip previous spaces
	if(sscanf(input, "%1hhu%1hu%c", &board, &field)!=2){
		board = 10;
		field = 10;
	}
	board--;
	field--;
	field = 0x1 << field;
	printf("Interpreted as %u, %u\n",board+1, field);
	return MoveDescriptor{board, field};
}

MoveDescriptor getDescriptionOnStream(){
	std::string input;
	std::getline(std::cin, input);
	return getMoveDescriptionFromInput(input.data());
}

MoveDescriptor getInitDescriptorOnStream(){
	std::string input;
	std::getline(std::cin, input, ',');
	return getMoveDescriptionFromInput(input.data());
}
