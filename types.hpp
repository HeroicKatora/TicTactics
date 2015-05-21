/*
 * types.hpp
 *
 *  Created on: 11.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

#include <cinttypes>

typedef unsigned short FieldBits;
typedef unsigned short BoardBits;

inline FieldBits getFieldOfBoard(BoardBits boardID){
	return 0x1 << boardID;
}

inline BoardBits getBoardOfField(FieldBits bits){
	return 32-__builtin_clz(bits)-1;
}
