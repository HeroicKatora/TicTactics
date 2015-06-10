/*
 * types.hpp
 *
 *  Created on: 11.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

#include <cinttypes>

//0000 000$ $$$$ $$$$ bit set according to the target field
typedef unsigned short FieldBits;
//0..8, index of board
typedef unsigned short BoardBits;

inline FieldBits getFieldOfBoard(BoardBits boardID){
	return 0x1 << boardID;
}

inline BoardBits getBoardOfField(FieldBits bits){
	return 32-__builtin_clz(bits)-1;
}

inline unsigned getIndexOfField(FieldBits bits){
	return (unsigned) getBoardOfField(bits);
}

inline unsigned getIndexOfBoard(BoardBits bits){
	return (unsigned) bits;
}

inline FieldBits getFieldOfIndex(unsigned index){
	return 0x1 << index;
}

inline BoardBits getBoardOfIndex(unsigned index){
	return (BoardBits) index;
}
