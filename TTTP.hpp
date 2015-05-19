/*
 * TTTP.hpp
 *
 *  Created on: 19.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

struct TTTPConst{
	constexpr static int lineLength = 1000;

	static char *p1; //One
	static char *p2; //"Two"

	static char VALID ;
	static char EMPTY ;
	static char Set1W1 ;
	static char Set1WA ;

	static char Set3W0 ;
	static char Set3W1 ;
	static char Set3W3 ;
	static char Set3W2 ;

	static char Set2W2 ;
	static char Set2WA ;

	static char * playerLost;
	static char * playerWon;
	static char * playerReady;
	static char * lineInvalidMove;
	static char * lineWrongInput;
	static char * lineStartGame;
	static char * lineEndTTTP;
};

struct EngineConstants{
	static char * welcome;
	static char * name;
};
