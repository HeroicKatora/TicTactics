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

inline BoardBits getFieldOfBoard(std::uint_fast8_t boardID){
	return 0x1 << boardID;
}

inline std::uint_fast8_t getBoardOfField(BoardBits bits){
	return 15-__builtin_clz(bits);
}
