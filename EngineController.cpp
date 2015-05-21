/*
 * EngineController.cpp
 *
 *  Created on: 21.05.2015
 *      Author: Andreas Molzer
 */

#include "EngineController.hpp"

EngineController::EngineController(Searcher& s):controlledSearcher(s) {
}

void EngineController::notifyMoveMade(Move& m) {
	controlledSearcher.notifyMoveMade(m);
}

void EngineController::notifyLine(const char* line) {
	//TODO handle engine commands
}

void EngineController::notifySetup(const char* line) {
	//TODO handle engine setup
}
