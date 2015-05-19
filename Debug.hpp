/*
 * Debug.cpp
 *
 *  Created on: 19.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

template<typename... T>
inline void dbgPrint(const char * s, T... args){
#ifdef Debug
	printf(s, args...);
#endif
}
