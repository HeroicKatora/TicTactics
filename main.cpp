/*
 * main.cpp
 *
 *  Created on: 18.05.2015
 *      Author: Andreas Molzer
 */
#include <cstdio>
#include "Input.hpp"
#include "GameState.hpp"
#include "Debug.hpp"
#include "Output.hpp"
#include <cstdlib>
#include <string>

GameState state{};

/**
 * Looses the game for a player (from his fault)
 */
void looseGame(bool playerOne);

/**
 * Wins the game for a player (from rules)
 */
void winGame(bool winnerPlayerOne);

/**
 * Ends the game and prints end position
 */
void endGame();

/**
 * Starts initialization
 */
void initGame();
void settingsPhase();
void engineSettingsPhase();
void gameSettingsPhase();
void gameLoop();

void playMove();
void engineOp();
void printReq();
void setupEngine();
void setupGame();

char line [TTTPConst::lineLength];

void goToNextLine(){
	nextProtocolLine(line, TTTPConst::lineLength);
}

/**
 * Waits for a line matching the regex
 */
void waitForLine(std::regex& regex){
	do{
		goToNextLine();
	}while(!matches(line, regex));
}

void waitForLine(const char * wait){
	do{
		goToNextLine();
	}while(!strcmp(line, wait));
}

int main(int argc, char **argv) {
	printOut(EngineConstants::welcome);
	waitForLine(reg::protocolStart);
	printOut(EngineConstants::name);
	settingsPhase();
	state.start();
	printOut(TTTPConst::lineStartGame);
	initGame();
	gameLoop();
}

void settingsPhase(){
	for(int i = 1000;i-->0;){
		goToNextLine();
		if(matches(line, TTTPConst::lineSettingsEngine))
			engineSettingsPhase();
		else if(matches(line, TTTPConst::lineSettingsGame))
			gameSettingsPhase();
		else if(matches(line, TTTPConst::lineSettingsEnd))
			break;
	}
}

void gameLoop(){
	while(!state.isWon()){
		goToNextLine();
		if(matches(line, reg::move)){
			playMove();
		}else if(matches(line, reg::engineOp)){
			engineOp();
		}else if(matches(line, reg::printReq)){
			printReq();
		}else{
			printOut(TTTPConst::lineWrongInput);
			winGame(!state.isPlayerOneTurn());
		}
	}
	winGame(state.hasWon(true));
}

void nextInitMove(MoveDescriptor *array, int& existing, bool playerOne){
	goToNextLine();
	if(matches(line, reg::move)){
		MoveDescriptor desc = getMoveDescriptor(line);
		Move m(desc);
		if(!state.isValidMove(m)){
			printErr(TTTPConst::lineInvalidMove);
			looseGame(playerOne);
		}
		array[existing] = desc;
		existing++;
	}else if(matches(line, reg::engineOp)){
		engineOp();
	}else{
		printErr(TTTPConst::lineWrongInput);
		winGame(!playerOne);
	}
}

void initGame(){
	MoveDescriptor playerOne[9];
	MoveDescriptor playerTwo[9];

	int done = 0;
	waitForLine(reg::initP1);
	while(done < 9) nextInitMove(playerOne, done, true);

	done = 0;
	waitForLine(reg::initP2);
	while(done < 9) nextInitMove(playerTwo, done, false);

	InitResult result;
	result.type = INVALID;
	result.info.index = -1;
	result = state.initializeWithMoves(playerOne, playerTwo);
	if(result.type != PASSED){
		printErr(TTTPConst::lineInvalidInit, result.type, result.info.index, result.info.playerOne);
		if(result.type == NOT_DURING_INIT){
			infoPrint(EngineConstants::commentInitDuringGame);
		}else{
			infoPrint(EngineConstants::commentInitIncorrectInput);
			const char * player = result.info.playerOne? TTTPConst::p1:TTTPConst::p2;
			std::string fault, move, additional;
			additional = "";
			char mov[30];
			MoveDescriptor wrong;
			if(result.info.playerOne) wrong = playerOne[result.info.index];
			else wrong = playerTwo[result.info.index];
			sprintMove(mov, wrong);
			move = mov;
			switch(result.type){
			case DUPLICATE:
				fault = EngineConstants::initDuplicate;
				break;
			case INVALID:
				fault = EngineConstants::initInvalid;
				sprintf(mov, "%u", result.info.index+1);
				break;
			case MIDDLE_MOVE:
				fault = EngineConstants::initMiddleMove;
				additional = EngineConstants::initMiddleComment;
				break;
			case FIELD_OVERFLOW:
				fault = EngineConstants::initFieldOverflow;
				additional = EngineConstants::initFieldComment;
				break;
			case BOARD_OVERFLOW:
				fault = EngineConstants::initBoardOverflow;
				additional = EngineConstants::initBoardComment;
				break;
			default:
				break;
			}
			infoPrint(EngineConstants::commentInitMistake, player, fault.data(), move.data(), additional.data());
		}
		looseGame(result.info.playerOne);
	}
	waitForLine(reg::startTurns);
}

void looseGame(bool playerOne){
	printOut(TTTPConst::linePlayerLost, playerOne? TTTPConst::p1 : TTTPConst::p2);
	winGame(!playerOne);
}

void winGame(bool winnerOne){
	printOut(TTTPConst::linePlayerWon, winnerOne? TTTPConst::p1 : TTTPConst::p2);
	endGame();
}

void endGame(){
	state.print();
	printOut(TTTPConst::lineEndTTTP);
	exit(0);
}

void engineSettingsPhase(){
	for(goToNextLine(); !matches(line, reg::endSettings); goToNextLine()){
		setupEngine();
	}
}

void gameSettingsPhase(){
	for(goToNextLine(); !matches(line, reg::endSettings); goToNextLine()){
		setupGame();
	}
}

void playMove(){
	MoveDescriptor desc = getMoveDescriptor(line);
	//FIXME according to protocol
	state.playMove(Move(desc));
}

/**
 * Handles engine operation line
 */
void engineOp(){
	//TODO
}

/**
 * Handles print operation line
 */
void printReq(){
	if(matches(line, TTTPConst::linePrintFull)){
		state.print();
	}else{
		char c = line[7]-'1'; //Dirty way !!
		state.print(c);
	}
}

/**
 * Sets up the engine with the current line
 */
void setupEngine(){
	//TODO
}

/**
 * Sets up the game with the current line
 */
void setupGame(){
	//TODO
}

