/*
 * Searcher.cpp
 *
 *  Created on: 13.05.2015
 *      Author: Andreas Molzer
 */

#include "Searcher.hpp"

template<unsigned size>
Searcher<size>::Searcher(GameState * state):gameState(state), pause(true), movesGenerated(){

}

template<unsigned size>
MoveSuggestion Searcher<size>::getBestKnownMove(){

}


template<unsigned size>
void Searcher<size>::runParallel() {

}

template<unsigned size>
void Searcher<size>::notifyUndo(Move& move) {

}

template<unsigned size>
void Searcher<size>::notifyMoveMade(Move& move) {

}

template<unsigned size>
void Searcher<size>::setPause(bool pause) {
	this->pause = pause;
}

