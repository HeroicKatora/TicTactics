/*
 * Searcher.cpp
 *
 *  Created on: 13.05.2015
 *      Author: Andreas Molzer
 */

#include "Searcher.hpp"
#include "GameState.hpp"
#include <stack>
#include <vector>

#include <cmath>
#include <thread>
#include <chrono>
#include <functional>

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
float functionN(unsigned depth){
	return (float) depth;
}

unsigned getWeightedMaxChildNumber(float weight, unsigned depth){
	//We use caching since that is O(c), while the function n might not be
	static std::vector<float> calculationResults {};
	std::function<float (unsigned int)> functionN_ = functionN;
	unsigned size = calculationResults.size();
	while(size <= depth){
		calculationResults.push_back(functionN_(size++));
	}
	return weight * calculationResults[depth];
}

Searcher::Searcher(GameState * state):gameState(state), pause(true){
	if(state->searcher) throw std::exception();
	state->searcher = this;
}

MoveSuggestion Searcher::getBestKnownMove(){
	SearchNode * m = topNode.children;
	if(!m) return MoveSuggestion{MoveDescriptor{}, nan("")};
	return MoveSuggestion{m->move, m->rating};
}

void Searcher::runParallel() {
	end = false;
	static MoveHistory movesGenerated{};
	//TODO implement spawning of threads

}

void Searcher::notifyUndo(Move& move) {

}

void Searcher::notifyMoveMade(Move& move) {

}

void Searcher::setPause(bool pauseNow) {
	if(pauseNow){
		if(!pause){
			pauseMutex.lock();
			pause = true;
		}
	}else{
		if(pause){
			pauseMutex.unlock();
			pause = false;
		}
	}
}

void Searcher::endParallel() {
	setPause(false);
	end = true;
}


size_t discoverMoves(const GameState *state, SearchNode *&dest, size_t maxNumber) {
}

void Searcher::parallelSearch(SearchNode * startNode){
	if(!startNode)
		return;

	GameState searchState = *gameState;
	struct SearchPathNode{
		SearchNode *node;
		unsigned childIndex;
	};

	unsigned maxDepth, depth;
	std::stack<SearchPathNode> nodePath{};
	SearchPathNode current{startNode, 0};
	auto out = [&](unsigned retain){
		if(nodePath.size()){
			for(unsigned i = retain;i<current.node->childCount;i++){
				current.node->children[i].close();
			}
			Move m {current.node->move};
			searchState.undoMove(m);
			current = nodePath.top();
			nodePath.pop();
			return false;
		}else{
			return true;
		}
	};
	auto in = [&](SearchPathNode& newNode){
		Move m {newNode.node->move};
		searchState.applyAndChangeMove(m);
		nodePath.push(current);
		current = newNode;
		depth++;
	};
	bool load = false;
	for(;!end;maxDepth++){
		depth = 0;
		while(!end){
			if(load){
				if(out(getWeightedMaxChildNumber(current.node->weight, depth))){
					break;
				}
			}
			if(depth <= maxDepth){
				//Expand this node
				current.node->discover(gameState);
			}

			//Search code
			if(current.childIndex == current.node->childCount||
					depth == maxDepth){
				current.node->reorderChildren(searchState.isPlayerOneTurn());
				load = true;
			}else{
				SearchPathNode next = {&current.node->children[current.childIndex], 0};
				current.childIndex++;
				in(next);
			}


			//Makes the thread pause until pauseMutex is available
			if(!end){
				pauseMutex.lock();
				pauseMutex.unlock();
			}
		}
		current.childIndex = 0;
	}
}

void SearchNode::discover(const GameState *state){
	if(!children){
		childCount = discoverMoves(state, children, -1);
	}
}

void SearchNode::close(){
	free(children);
	childCount = 0;
}

void SearchNode::reorderChildren(bool playerOne){

}
