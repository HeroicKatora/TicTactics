/*
 * Searcher.cpp
 *
 *  Created on: 13.05.2015
 *      Author: Andreas Molzer
 */

#include "Searcher.hpp"

/**
 * Calculates the number n of childs allowed for a node of given weight and depth.
 * The top n children (best for the player whos turn it is) keep their own children, the others don't.
 * It is valid to expect depth to be sequential (and use that for buffering performance)
 * The aim is to keep memory usage in O(f(d)) for a function f and depth d with
 * f in o(e^n). Thus one has to find a function g(d) such that (sum from i=0 to d g(i)) in O(f(d)).
 * Since we want to have a function for each element of the tree, we can interpret g to also
 * be restricted by
 * g(d) = sum over all nodes n(node, d) for a function n we want
 *
 * g is the function below and to achieve the formula, we may distribute a total of 1 across
 * all nodes of one depth and set n(node, d) = g(d)*node.weigth
 * For special functions, different solutions might also work and can be used.
 *
 * Examples for f(d) = d:
 * We instantly find g(d) = 1
 * Then n is equal to node.weigth
 *
 * Example for f(d) = b^d (the unwanted case, but possible):
 * We can use g(d) = b^d
 * Thus n = node.weigth*(b^d) or if b is the breadth factor of the tree, n in O(1) is also possible
 *
 * Example for f = d^2 (maybe not worst):
 * We can use g = d
 * And n = node.weigth * d
 *
 * Example for f = d log d:
 * We can use g = log d
 * And n = node.weigth * log d
 */
unsigned getWeightedMaxChildNumber(float weight, unsigned depth){

}


Searcher::Searcher(GameState * state):gameState(state), pause(true){
}

MoveSuggestion Searcher::getBestKnownMove(){
	SearchNode * m = topNode.children[0];
	if(!m) return MoveSuggestion{MoveDescriptor{},0};
	return MoveSuggestion{m->move, m->rating};
}

void Searcher::runParallel() {
	end = false;
	static MoveHistory movesGenerated{};

}

void Searcher::notifyUndo(Move& move) {

}

void Searcher::notifyMoveMade(Move& move) {

}

void Searcher::setPause(bool pause) {
	this->pause = pause;
}

void Searcher::endParallel() {
	end = true;
}
