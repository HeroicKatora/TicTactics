/*
 * Searcher.cpp
 *
 *  Created on: 13.05.2015
 *      Author: Andreas Molzer
 */

#include "Searcher.hpp"
#include "GameState.hpp"
#include "Output.hpp"
#include "TTTP.hpp"
#include <stack>
#include <vector>

#include <cmath>
#include <thread>
#include <chrono>
#include <algorithm>
#include <functional>
#include <future>

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
__attribute__((const))
float functionN(unsigned depth){
	return (float) depth;
}

__attribute__((pure))
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

Searcher::Searcher(const GameState * state):gameState(state), pause(true){
	if(state->searcher) throw std::exception();
	state->searcher = this;
}

MoveSuggestion Searcher::getBestKnownMove() const{
	SearchNode * m = topNode.children;
	if(!m) return MoveSuggestion{MoveDescriptor{}, nan("")};
	return MoveSuggestion{m->move, m->rating};
}

void Searcher::runParallel() {
	/*end = false;
	searchThread = std::thread(std::bind(startSearch, this, &topNode, 0, 0));
	searchThread.detach();*/ // Disabled for now
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
	if(searchThread.joinable()) searchThread.join();
}

/**
 * Counts the moves that are possible on a board without considering any limitations
 */
__attribute__((const))
size_t countPossibleMoves(const TicTacBoard& board, FieldBits forbidden){
	return 9-__builtin_popcount((FieldBits) (board.setPlayerOne| board.setPlayerTwo) | forbidden);
}

/**
 * Counts the possible moves that do not win the board
 */
__attribute__((const))
size_t countNonWinMoves(const TicTacBoard& board){
	FieldBits set = board.getBlockedFields();
	set |= winMoves(board.setPlayerOne);
	return 9-__builtin_popcount(set);
}

__attribute__((const))
size_t countBeginMoves(const TacTicBoard& state){
	size_t moves = 0;
	for(int i = 0;i<9;i++){
		moves += countNonWinMoves(state.components[i]);
	}
	return moves-1; //Minus the middle move since that is guaranteed to be open
}

/**
 * Returns positive values if the moves are on the board and negative if not
 */
signed countPlayMoves(const TacTicBoard& state, const MoveDescriptor& oldMove){
	size_t index = oldMove.getBoardIndex();
	size_t moves = countPossibleMoves(state.components[index], getFieldOfIndex(index));
	bool allExcept = false;
	if(moves == 0){
		allExcept = true;
		for(size_t i = 0;i<9;i++){
			if(i == index) continue;
			moves += countPossibleMoves(state.components[index], getFieldOfIndex(index));
		}
	}
	return allExcept?-moves:moves;
}

size_t discoverMoves(const TacTicBoard& state, SearchNode *&dest, const MoveDescriptor& oldMove) {
	if(state.isWon()){
		dest = NULL;
		return 0;
	}
	signed count;
	size_t nodeIndex = 0;
	if(oldMove.isInvalidDefault()){
		//This means we are on the first more after init
		count = countBeginMoves(state);
		dest = new SearchNode[count];
		for(int i = 0;i<9;i++){
			FieldBits nonMoves = state.components[i].getBlockedFields();
			if(i == 4) nonMoves |= MID_FIELD;
			nonMoves |= winMoves(state.components[i].setPlayerOne);
			FieldBits moves = invertField(nonMoves);
			while(moves){
				dest[nodeIndex++].move = {getBoardOfIndex(i), (FieldBits)~(moves^(-moves))};
				moves &= (moves-1);
			}
		}
	}else{
		count = countPlayMoves(state, oldMove);
		bool fullBoard = count < 0;
		if(__builtin_expect(fullBoard, false)){
			count = -count;
		}
		dest = new SearchNode[count];
		size_t intoIndex = oldMove.getFieldIndex();
		FieldBits backField = getFieldOfIndex(oldMove.getBoardIndex());
		if(__builtin_expect(fullBoard, false)){
			for(unsigned i = 0;i<9;i++){
				if(i == oldMove.getBoardIndex()) continue;
				FieldBits nonMoves = state.components[i].getBlockedFields();
				nonMoves |= backField;
				FieldBits moves = invertField(nonMoves);
				while(moves){
					dest[nodeIndex++].move = {getBoardOfIndex(i), (FieldBits)~(moves^(-moves))};
					moves &= (moves-1);
				}
			}
		}else{
			FieldBits nonMoves = state.components[intoIndex].getBlockedFields();
			nonMoves |= backField;
			FieldBits moves = invertField(nonMoves);
			while(moves){
				dest[nodeIndex++].move = {getBoardOfIndex(intoIndex), (FieldBits)~(moves^(-moves))};
				moves &= (moves-1);
			}
		}
	}
	return count;
}

void printBestPath(const SearchNode * node){
	char buffer [1000];
	int off = 0;
	for(int i = 0;i<100 && node;i++){
		off += sprintMove(buffer+off, node->move);
		node = node->children;
	}
	printChannel(TTTPConst::channelEngine, buffer);
}

void Searcher::startSearch(SearchNode * startNode, unsigned maximalDepth, time_t maxDuration){
	if(!startNode)
		return;

	time_t startTime = time(NULL);
	GameState searchState = *gameState;
	struct SearchPathNode{
		SearchPathNode(SearchNode *node):node(node),
				childIndex(0), moveMade(node->move){
		}
		SearchNode *node;
		unsigned childIndex;
		Move moveMade;
	};

	unsigned maxDepth = 1, depth = 0;
	std::stack<SearchPathNode> nodePath{};
	SearchPathNode current{startNode};
	auto out = [&](unsigned retain){
		if(nodePath.size()){
			for(unsigned i = retain;i<current.node->childCount;i++){
				current.node->children[i].close();
			}
			searchState.undoMove(current.moveMade);
			current = nodePath.top();
			nodePath.pop();
			depth--;
			return false;
		}else{
			return true;
		}
	};
	auto in = [&](SearchPathNode& newNode){
		nodePath.push(current);
		current = newNode;
		searchState.applyAndChangeMove(current.moveMade);
		current.node->rating = rate(searchState);
		depth++;
	};
	printInfo("Started searching");
	bool load = false;
	for(;!end && (maximalDepth <= 0 || maxDepth < maximalDepth);maxDepth++){
		depth = 0;
		printOut(":Search depth %u:", maxDepth);
		while(!end){
			if(load){
				if(out(getWeightedMaxChildNumber(current.node->weight, depth))){
					break;
				}
				load = false;
			}
			if(depth < maxDepth){
				//Expand this node
				current.node->discover(searchState.board);
			}

			//Search code

			if(depth == maxDepth){
				current.node->rating = rate(searchState);
				load = true;
			}else if(current.childIndex == current.node->childCount){
				current.node->revalueChildren(searchState.isPlayerOneTurn());
				load = true;
			}else{
				SearchPathNode next = {&current.node->children[current.childIndex]};
				current.childIndex++;
				in(next);
			}

			if(maxDuration > 0 && time(NULL)-startTime > maxDuration){
				printInfo(EngineConstants::computationTimeExceeded);
				end = true;
			}

			//Makes the thread pause until pauseMutex is available
			if(!end){
				pauseMutex.lock();
				pauseMutex.unlock();
			}
		}
		current.childIndex = 0;
		printBestPath(startNode->children);
	}
}

void SearchNode::discover(const TacTicBoard& state){
	if(!children){
		childCount = discoverMoves(state, children, move);
	}
}

void SearchNode::close(){
	for(unsigned i = 0;i<childCount;i++){
		children[i].close();
	}
	delete(children);
	children = NULL;
	childCount = 0;
}

__attribute__((const))
bool isTwoBetterThan(float one, float two, bool playerOne){
	if(playerOne) return one < two;
	else return one > two;
}

__attribute__((const))
bool isTwoBetterThanNode(const SearchNode& one, const SearchNode& two, bool playerOne){
	return isTwoBetterThan(one.rating, two.rating, playerOne);
}

void SearchNode::revalueChildren(bool playerOne){
	if(!children) return;
	std::sort_heap(children, children+childCount-1, std::bind(isTwoBetterThanNode, std::placeholders::_1, std::placeholders::_2, playerOne));
	float weight = 1;
	for(unsigned i = 0;i<childCount;i++){
		weight /= 2;
		children[i].weight = weight;
	}
	rating = children[0].rating;
}

void SearchNode::setToMaxChild(bool playerOne){
	for(unsigned int i = 0;i<childCount;i++){
		rating = isTwoBetterThan(rating, children[i].rating, playerOne)?children[i].rating:rating;
	}
}
