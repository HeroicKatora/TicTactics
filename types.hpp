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

static const FieldBits MID_FIELD = 0x10;

inline FieldBits getFieldOfBoard(const BoardBits boardID){
	return 0x1 << boardID;
}

inline BoardBits getBoardOfField(const FieldBits bits){
	return __builtin_ffs(bits)-1;
}

inline unsigned getIndexOfField(const FieldBits bits){
	return (unsigned) getBoardOfField(bits);
}

inline unsigned getIndexOfBoard(const BoardBits bits){
	return (unsigned) bits;
}

inline FieldBits getFieldOfIndex(const unsigned index){
	return 0x1 << index;
}

inline BoardBits getBoardOfIndex(const unsigned index){
	return (BoardBits) index;
}

inline FieldBits invertField(const FieldBits& field){
	return 0x1FF-field;
}

inline FieldBits invertField(const FieldBits&& field){
	return 0x1FF-field;
}
