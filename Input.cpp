/*
 * Input.cpp
 *
 *  Created on: 18.05.2015
 *      Author: Andreas Molzer
 */
#include "Input.hpp"
#include "Debug.hpp"
#include <string>
#include <iostream>

MoveDescriptor getMoveDescriptionFromInput(char * input){
	BoardBits board;
	FieldBits field;
	dbgPrint(input);
	dbgPrint("->Input\n");
	int off = 0;
	while(off+=sscanf(input+off, " ")); //Skip previous spaces
	if(sscanf(input+off, "%1hhu%1hu%c", &board, &field)!=2){
		printf("Wrong input %s.\n", input);
		board = 10;
		field = 10;
	}
	board--;
	field--;
	field = 0x1 << field;
	dbgPrint("Interpreted as %u, %u\n",board, field);
	return MoveDescriptor{board, field};
}

MoveDescriptor getDescriptionOnStream(){
	char read[30];
	while(scanf("%2c[\n]", read) != 2); //Wait for valid input
	return getMoveDescriptionFromInput(read);
}

MoveDescriptor getInitDescriptorOnStream(){
	char read[30];
	while(scanf(" ", read)); //Skip previous ' '
	while(scanf("%2c[,\n ]", read) != 2); // Wait for valid input
	return getMoveDescriptionFromInput(read);
}
