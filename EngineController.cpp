/*
 * EngineController.cpp
 *
 *  Created on: 21.05.2015
 *      Author: Andreas Molzer
 */

#include "EngineController.hpp"
#include "Input.hpp"

EngineController::EngineController(Searcher& s):controlledSearcher(s) {
}

void EngineController::notifyMoveMade(Move& m) {
	controlledSearcher.notifyMoveMade(m);
}

void EngineController::notifyLine(const char* line) {
	if(matches(line, "Go")){
		controlledSearcher.runParallel();
	}else if(matches(line, "Stop")){
		controlledSearcher.endParallel();
	}
}

void EngineController::notifySetup(const char* line) {
	//TODO handle engine setup
}
