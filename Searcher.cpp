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
[[gnu::const]]
inline constexpr float functionN(unsigned depth){
	return (float) 10e7;
}

[[gnu::pure]]
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

Searcher::Searcher(const GameState * state):gameState(state), pause(true),
		pauseMutex(), end(false), topNode(){
	if(state->searcher) throw std::exception();
	state->searcher = this;
	topNode.weight = 16;
}

MoveSuggestion Searcher::getBestKnownMove() const{
	SearchNode * m = topNode.children;
	if(!m) return MoveSuggestion{MoveDescriptor{}, 0};
	return MoveSuggestion{{m->move.getBoardSet(), m->move.getFieldSet()}, m->rating};
}

void Searcher::runParallel() {
	end = false;
	searchThread = std::thread(std::bind(startSearch, this, &topNode, 0, std::chrono::seconds::zero()));
	searchThread.detach();
}

void Searcher::notifyUndo(Move& move) {
	endParallel();
	topNode.childCount = 0;
	topNode.close();
	topNode.move = move;
	topNode.rating = 0;
	topNode.weight = 1;
}

void Searcher::notifyMoveMade(Move& move) {
	bool pauseAfter = pause;
	//setPause(true);
	endParallel();
	SearchNode *newNode = NULL;
	for(unsigned i = 0;topNode.children && i<topNode.childCount;i++){
		if(topNode.children[i].move == move){
			newNode = topNode.children+i;
		}else{
			topNode.children[i].close();
		}
	}
	if(newNode){
		auto ptr = topNode.children;
		topNode = *newNode;
		delete(ptr);
	}else{
		topNode.move = move;
		topNode.childCount = 0;
		topNode.close();
		topNode.rating = 0;
	}
	topNode.weight = 1;
	setPause(pauseAfter);
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
[[gnu::const]]
size_t countPossibleMoves(const TicTacBoard& board, FieldBits forbidden){
	return 9-__builtin_popcount((FieldBits) (board.setPlayerOne| board.setPlayerTwo) | forbidden);
}

/**
 * Counts the possible moves that do not win the board
 */
[[gnu::const]]
size_t countNonWinMoves(const TicTacBoard& board){
	FieldBits set = board.getBlockedFields();
	set |= winMoves(board.setPlayerOne);
	return 9-__builtin_popcount(set);
}

[[gnu::const]]
size_t countBeginMoves(const TacTicBoard& state){
	size_t moves = 0;
	for(int i = 0;i<9;i++){
		moves += countNonWinMoves(state.components[i]);
	}
	return moves-1; //Minus the middle move since that is guaranteed to be open
}

struct Movecount{
	size_t moves;
	enum Type{
		Normal,
		Back,
		Out,
		OutBack
	};
	Type type;
};
/**
 * Returns positive values if the moves are on the board and negative if not
 */
[[gnu::const]]
Movecount countPlayMoves(const TacTicBoard& state, const Move& oldMove){
	size_t backIndex = oldMove.getBoardIndex();
	size_t index = oldMove.getFieldIndex();
	size_t moves = countPossibleMoves(state.components[index], getFieldOfIndex(backIndex));
	size_t moveWBack = countPossibleMoves(state.components[index], 0);
	if(moveWBack == 0){
		for(size_t i = 0;i<9;i++){
			moves += countPossibleMoves(state.components[i], getFieldOfIndex(backIndex));
		}
		if(moves == 0){
			for(size_t i = 0;i<9;i++){
				moves += countPossibleMoves(state.components[i], 0);
			}
			return {moves, Movecount::OutBack};
		}
		return {moves, Movecount::Out};
	}else if(moves == 0){
		return {1, Movecount::Back};
	}
	return {moves, Movecount::Normal};
}

size_t discoverMoves(const TacTicBoard& state, SearchNode *&dest, const Move& oldMove) {
	if(state.isWon()){
		dest = NULL;
		return 0;
	}
	size_t nodeIndex = 0;
	if(oldMove.isInvalidDefault()){
		//This means we are on the first more after init
		signed count = countBeginMoves(state);
		dest = new SearchNode[count];
		for(int i = 0;i<9;i++){
			FieldBits nonMoves = state.components[i].getBlockedFields();
			if(i == 4) nonMoves |= MID_FIELD;
			nonMoves |= winMoves(state.components[i].setPlayerOne);
			FieldBits moves = invertField(nonMoves);
			while(moves){
				dest[nodeIndex].move = {getBoardOfIndex(i), (FieldBits)(moves&(~moves+1))};
				nodeIndex++;
				moves &= (moves-1);
			}
		}
		return count;
	}else{
		Movecount mCount = countPlayMoves(state, oldMove);
		signed count = mCount.moves;
		if(count == 0){
			dest = NULL;
			return 0;
		}
		dest = new SearchNode[count];
		size_t intoIndex = oldMove.getFieldIndex();
		FieldBits backField = getFieldOfIndex(oldMove.getBoardIndex());
		if(__builtin_expect(mCount.type == Movecount::Normal, true)){
			FieldBits nonMoves = state.components[intoIndex].getBlockedFields();
			nonMoves |= backField;
			FieldBits moves = invertField(nonMoves);
			while(moves){
				dest[nodeIndex].move = {getBoardOfIndex(intoIndex), (FieldBits)(moves&(~moves+1))};
				nodeIndex++;
				moves &= (moves-1);
			}
		}else if(__builtin_expect(mCount.type == Movecount::Out, true)){
			for(unsigned i = 0;i<9;i++){
				if(i == oldMove.getFieldIndex()) continue;
				FieldBits nonMoves = state.components[i].getBlockedFields();
				nonMoves |= backField;
				FieldBits moves = invertField(nonMoves);
				while(moves){
					dest[nodeIndex].move = {getBoardOfIndex(i), (FieldBits)(moves&(~moves+1))};
					nodeIndex++;
					moves &= (moves-1);
				}
			}
		}else if(__builtin_expect(mCount.type == Movecount::Back, false)){
			dest[0].move = {getBoardOfIndex(intoIndex), backField};
		}else{
			for(unsigned i = 0;i<9;i++){
				if(!(state.components[i].getBlockedFields() & backField)){
					dest[nodeIndex].move = {getBoardOfIndex(i), backField};
					nodeIndex++;
				}
			}
		}
		return count;
	}
}

void printBestPath(const SearchNode * node, unsigned depth, long long signed nodes, std::chrono::milliseconds time){
	char buffer [1000];
	int off = sprintf(buffer, "score: %d depth: %u nodes: %-15llu time: %-10u ", node->rating, depth, nodes, time);
	node = node->children;
	for(int i = 0;i<100 && node;i++){
		off += sprintMove(buffer+off, node->move);
		off += sprintf(buffer+off, " ");
		node = node->children;
	}
	//printChannel(TTTPConst::channelEngine, "%s", buffer);
	printOut("%s", buffer);
}

void Searcher::startSearch(SearchNode * startNode, unsigned maximalSearchDepth, const std::chrono::seconds maxDuration){
	if(!startNode)
		return;
	const unsigned maximalDepth = maximalSearchDepth == 0? 81 : std::min(maximalSearchDepth, (unsigned)81);
	using namespace std::chrono;
	bool autoPrint = false;

	const auto startTime = steady_clock::now();
	GameState searchState = *gameState;
	struct SearchPathNode{
		SearchPathNode(SearchNode *node, Rating marginAlpha, Rating marginBeta):node(node),
				childIndex(0), allFinal(true), marginAlpha(marginAlpha), marginBeta(marginBeta)
		{
		}
		SearchNode *node;
		unsigned childIndex;
		bool allFinal;
		Rating marginAlpha; //If max possible rating (from turn players perspective) is better than alpha, skip this node
		Rating marginBeta; //Max possible rating of all nodes that were already viewed, used as new marginAlpha one depth further
	};

	unsigned maxDepth = 1, depth = 0;
	std::vector<SearchPathNode> vector;
	vector.reserve(50);
	std::stack<SearchPathNode, std::vector<SearchPathNode>> nodePath{vector};

	SearchPathNode current{startNode, maxRating(searchState.isPlayerOneTurn()), minRating(searchState.isPlayerOneTurn())};
	const auto out = [&](unsigned retain){
		if(__builtin_expect(nodePath.size() > 0, true)){
			Rating newRating = current.node->rating;
			bool final = current.allFinal;
			for(unsigned i = retain;i<current.node->childCount;i++){
				current.node->children[i].close();
			}
			searchState.undoMove(current.node->move);
			//char s[4];
            //sprintMove(s, current.node->move);

			current = nodePath.top();
			nodePath.pop();
			current.marginBeta = isOneBetterThan(current.marginBeta, newRating, searchState.isPlayerOneTurn())?current.marginBeta:newRating;
			current.allFinal &= final;
            //printDebug<3>("%d Undone move %s with rating %d Beta %d Alpha %d", depth, s, newRating, current.marginBeta, current.marginAlpha);
			current.childIndex++;
			depth--;
			return false;
		}else{
			depth--;
			return true;
		}
	};
	const auto in = [&](SearchPathNode& newNode){
		nodePath.push(current);
		current = newNode;
		searchState.applyAndChangeMove(current.node->move);
		//char s[4];
        //sprintMove(s, current.node->move);
		depth++;
        //printDebug<3>("%d Done move %s Beta %d Alpha %d", depth, s, current.marginBeta, current.marginAlpha);
	};

	printInfo("Started searching");
	long long signed nodesSearched = 0;
	long long signed nodesSearchedRecent = 0;
	bool load = false;
	for(;!end && (maximalDepth == 0 || maxDepth < maximalDepth);maxDepth++){
		depth = 0;
		printOut(":Search depth %u:", maxDepth);
		size_t finalNodes = 0;
		size_t cutsMade = 0;
		bool allNodes = false;
		const auto timeStart = steady_clock::now();
		while(!end){
			if(load){
				load = false;
				if(out(getWeightedMaxChildNumber(current.node->weight, depth))){
					allNodes = true;
					break;
				}
			}

			//char s[4];
			//sprintMove(s, current.node->move);
			//printOut("Move: %s Beta: %d  Alpha %d  Depth %u",s , current.marginBeta, current.marginAlpha, depth);
			//Search code
			if(depth < maxDepth){ // Besser: Funktion(rating, depth) gegen eine Schranke vergleichen, Schranke nach Stossen erhoehen
				//Expand this node
				current.node->discover(searchState.gameboard);
				if(searchState.isWon() || current.node->childCount == 0){
					finalNodes++;
				}
			}else{
				if(!searchState.isWon()){
					current.allFinal = false;
				}
			}
			if(__builtin_expect(current.childIndex == current.node->childCount, false)){
				if(current.childIndex == 0){ //depth == maxDepth oder keine Childnodes
					current.node->rating = searchState.rate();
				}else{
					current.node->revalueChildren(searchState.isPlayerOneTurn(), current.childIndex);
				}
				load = true;
			}else{
				if(__builtin_expect(compare(current.marginBeta, current.marginAlpha, searchState.isPlayerOneTurn(), std::greater_equal<Rating>()), false)){
					current.node->revalueChildren(searchState.isPlayerOneTurn(), current.childIndex);
					cutsMade++;
					//sprintMove(s, current.node->children[0].move);
					//printDebug<3>("Cut made because of %s", s);
					load = true;
				}else{
					SearchPathNode next = {&current.node->children[current.childIndex],
							current.marginBeta, maxRating(searchState.isPlayerOneTurn())};
					nodesSearched++;
					in(next);
				}
			}

			if(nodesSearched%0x1000 == 0 && maxDuration > std::chrono::seconds::zero() && steady_clock::now()-startTime > maxDuration){
				printInfo(EngineConstants::computationTimeExceeded);
				end = true;
			}

			if(!end && pause){
				//Makes the thread pause until pauseMutex is available
				pauseMutex.lock();
				pauseMutex.unlock();
			}
		}
		current.childIndex = 0;
		current.marginAlpha = maxRating(searchState.isPlayerOneTurn());
		current.marginBeta = minRating(searchState.isPlayerOneTurn());
		auto nowTime = steady_clock::now();
		printBestPath(startNode, maxDepth, nodesSearched, duration_cast<milliseconds>(nowTime-startTime));
		printChannel(TTTPConst::channelDebug, "Nodes layer: %-10u Time layer: %-8u Cuts layer: %-10u",
				(size_t) (nodesSearched - nodesSearchedRecent), duration_cast<milliseconds>(nowTime-timeStart), cutsMade);

		nodesSearchedRecent = nodesSearched;
		if(allNodes && current.allFinal) //current.node->rating == maxRating(true) || current.node->rating == minRating(true)){
		{
			printInfo("All nodes are final, finished searching");
			autoPrint = true;
			break;
		}
	}
	while(nodePath.size()){
		out(getWeightedMaxChildNumber(current.node->weight, depth));
	}
	if(autoPrint){
		char move[5];
		sprintMove(move, current.node->children[0].move);
		printOut("move %s", move);
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

[[gnu::const]]
bool isOneBetterThanNode(const SearchNode& one, const SearchNode& two, bool playerOne){
	return isOneBetterThan(one.rating, two.rating, playerOne);
}

void SearchNode::revalueChildren(bool playerOne){
	revalueChildren(playerOne, childCount);
}

void SearchNode::revalueChildren(bool playerOne, unsigned maxChilds){
	if(!children) return;
	maxChilds = std::min(maxChilds, childCount);
	std::stable_sort(children, children+maxChilds, std::bind(isOneBetterThanNode, std::placeholders::_1, std::placeholders::_2, playerOne));
	float chweight = weight;
	for(unsigned i = 0;i<maxChilds;i++){
		chweight /= 2;
		children[i].weight = chweight;
	}
	rating = children[0].rating;
}

void SearchNode::setToMaxChild(bool playerOne){
	rating = minRating(playerOne);
	for(unsigned int i = 0;i<childCount;i++){
		rating = isOneBetterThan(rating, children[i].rating, playerOne)?children[i].rating:rating;
	}
}

void Searcher::eval(){
	printOut("Board evaluation: %d", gameState->rate());
}

Searcher::~Searcher(){
	endParallel();
	setPause(false);
	topNode.close();
}
