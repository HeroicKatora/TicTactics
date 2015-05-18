/*
 * Input.cpp
 *
 *  Created on: 18.05.2015
 *      Author: Andreas Molzer
 */
#include "Input.hpp"

MoveDescriptor getMoveDescriptionFromInput(char * input){
	BoardBits board;
	FieldBits field;
	if(sscanf(input, "%1x%1x%c", &board, &field)!=2){
		board = 10;
		field = 10;
	}
	field = 0x1 << field;
	return MoveDescriptor{board, field};
}

MoveDescriptor getDescriptionOnStream(FILE * stream){
	char input [3];
	fscanf(stream, "%2c", input);
	input[2] = 0;
	return getMoveDescriptionFromInput(input);
}
