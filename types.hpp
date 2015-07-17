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

typedef signed Rating;

static const FieldBits MID_FIELD = 0x10;

[[gnu::const]]
inline constexpr FieldBits getFieldOfBoard(const BoardBits& boardID){
	return 0x1 << boardID;
}

[[gnu::const]]
inline constexpr BoardBits getBoardOfField(const FieldBits& bits){
	return __builtin_ffs(bits)-1;
}

[[gnu::const]]
inline constexpr unsigned getIndexOfField(const FieldBits& bits){
	return (unsigned) getBoardOfField(bits);
}

[[gnu::const]]
inline constexpr unsigned getIndexOfBoard(const BoardBits& bits){
	return (unsigned) bits;
}

[[gnu::const]]
inline constexpr FieldBits getFieldOfIndex(const unsigned& index){
	return 0x1 << index;
}

[[gnu::const]]
inline constexpr BoardBits getBoardOfIndex(const unsigned& index){
	return (BoardBits) index;
}

[[gnu::const]]
inline constexpr FieldBits invertField(const FieldBits& field){
	return 0x1FF-field;
}

[[gnu::const]]
inline constexpr FieldBits invertField(const FieldBits&& field){
	return 0x1FF-field;
}
