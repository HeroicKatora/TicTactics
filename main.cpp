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
void engineSettingsPhase();
void gameSettingsPhase();
void gameLoop();

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

void waitForLine(char * wait){
	do{
		goToNextLine();
	}while(!strcmp(line, wait));
}

int main(int argc, char **argv) {
	printOut(EngineConstants::welcome);
	waitForLine(reg::protocolStart);
	printOut(EngineConstants::name);
	engineSettingsPhase();
	gameSettingsPhase();

	printOut(TTTPConst::lineStartGame);
	initGame();
	gameLoop();
}

void gameLoop(){
	while(!state.isWon()){
		goToNextLine();
		MoveDescriptor desc = getMoveDescriptor(line);
		//FIXME according to protocol
		state.playMove(Move(desc));
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
		printErr("%u %u %u", result.type, result.info.index, result.info.playerOne);
		if(result.type == NOT_DURING_INIT){
			infoPrint("The game is already ongoing, what were you thinking");
		}else{
			infoPrint("Some player failed to give correct input");
			const char * player = result.info.playerOne? "Player One":"Player Two";
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
				fault = " duplicate move";
				break;
			case INVALID:
				fault = "n invalid move";
				sprintf(mov, "%u", result.info.index+1);
				break;
			case MIDDLE_MOVE:
				fault = " middle move";
				additional = "\nYou can not move into the mid during initial phase.";
				break;
			case FIELD_OVERFLOW:
				fault = " field overflow";
				additional = "\nYou can only place one mark on equivalent fields.";
				break;
			case BOARD_OVERFLOW:
				fault = " board overflow";
				additional = "\nYou can only place two mark on the same board.";
				break;
			default:
				break;
			}
			infoPrint("%s has a%s with move %s%s.", player, fault.data(), move.data(), additional.data());
		}
		looseGame(result.info.playerOne);
	}
	waitForLine(reg::startTurns);
}

void looseGame(bool playerOne){
	printOut(TTTPConst::playerLost, playerOne? TTTPConst::p1 : TTTPConst::p2);
	winGame(!playerOne);
}

void winGame(bool winnerOne){
	printOut(TTTPConst::playerWon, winnerOne? TTTPConst::p1 : TTTPConst::p2);
	endGame();
}

void endGame(){
	state.print();
	printOut(TTTPConst::lineEndTTTP);
	exit(0);
}

/**
 * Sets up the engine with the current line
 */
void setupEngine(){
	//TODO
}

void engineSettingsPhase(){
	for(goToNextLine(); !matches(line, reg::endSettings); goToNextLine()){
		setupEngine();
	}
}

/**
 * Sets up the game with the current line
 */
void setupGame(){
	//TODO
}

void gameSettingsPhase(){
	for(goToNextLine(); !matches(line, reg::endSettings); goToNextLine()){
		setupGame();
	}
}
