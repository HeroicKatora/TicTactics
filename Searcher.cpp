/*
 * Searcher.cpp
 *
 *  Created on: 13.05.2015
 *      Author: Andreas Molzer
 */

#include "Searcher.hpp"

Searcher::Searcher(GameState * state):gameState(state), movesGenerated(), pause(true){

}

MoveSuggestion Searcher::getBestKnownMove(){

}

void Searcher::runParallel() {

}

void Searcher::notifyUndo(Move& move) {

}

void Searcher::notifyMoveMade(Move& move) {

}

void Searcher::setPause(bool pause) {
	this->pause = pause;
}
