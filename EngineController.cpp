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

bool EngineController::notifyLine(const char* line) {
	if(matches(line, "Go")){
		controlledSearcher.setPause(false);
		controlledSearcher.runParallel();
	}else if(matches(line, "Stop")){
		controlledSearcher.endParallel();
	}else if(matches(line, "Pause")){
		controlledSearcher.setPause(true);
	}else if(matches(line, "Unpause")){
		controlledSearcher.setPause(false);
	}else if(matches(line, "Rate")){
		controlledSearcher.eval();
	}else{
		return false;
	}
	return true;
}

void EngineController::notifySetup(const char* line) {
	//TODO handle engine setup
}
