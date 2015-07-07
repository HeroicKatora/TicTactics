/*
 * EngineController.hpp
 *
 *  Created on: 21.05.2015
 *      Author: Andreas Molzer
 */
#pragma once
#include "Move.hpp"
#include "Searcher.hpp"

struct EngineController{
	Searcher& controlledSearcher;

	EngineController(Searcher& s);

	void notifyMoveMade(Move& m);

	bool notifyLine(const char * line);

	void notifySetup(const char * line);
};
