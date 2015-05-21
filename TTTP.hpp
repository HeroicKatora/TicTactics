/*
 * TTTP.hpp
 *
 *  Created on: 19.05.2015
 *      Author: Andreas Molzer
 */
#pragma once

typedef const char CC;

struct TTTPConst{
	constexpr static unsigned lineLength = 1000;

	static CC p1 []; //One
	static CC p2 []; //"Two"

	static CC VALID ;
	static CC EMPTY ;
	static CC Set1W1 ;
	static CC Set1WA ;

	static CC Set3W0 ;
	static CC Set3W1 ;
	static CC Set3W3 ;
	static CC Set3W2 ;

	static CC Set2W2 ;
	static CC Set2WA ;

	static CC channelDebug [];
	static CC channelChat [];
	static CC channelMeta [];

	static CC lineSettingsEngine [];
	static CC lineSettingsGame [];
	static CC lineSettingsEnd [];

	static CC linePrintFull [];

	static CC linePlayerLost [];
	static CC linePlayerWon [];
	static CC linePlayerReady [];
	static CC lineInvalidMove [];
	static CC lineInitError [];
	static CC lineWrongInput [];
	static CC lineStartGame [];
	static CC lineEndTTTP [];
};

struct EngineConstants{
	static CC welcome [];
	static CC name [];

	static CC commentInitIncorrectInput [];
	static CC commentInitMistake [];
	static CC commentInitPlayerReady [];
	static CC commentInitDuringGame [];

	static CC initDuplicate [];
	static CC initInvalid [];
	static CC initMiddleMove [];
	static CC initMiddleComment [];
	static CC initBoardOverflow [];
	static CC initBoardComment [];
	static CC initFieldOverflow [];
	static CC initFieldComment [];
};
