/*
 * types.hpp
 *
 *  Created on: 11.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

#include <cinttypes>

typedef std::uint_fast16_t FieldBits;
typedef std::uint_fast8_t BoardBits;

inline BoardBits getFieldOfBoard(BoardBits boardID){
	return 0x1 << boardID;
}

inline std::uint_fast8_t getBoardOfField(FieldBits bits){
	return 8*sizeof(int)-__builtin_clz(bits)-1;
}
