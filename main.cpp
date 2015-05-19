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

constexpr int lineLength = 1000;
char line [lineLength];

void goToNextLine(){
	nextProtocolLine(line, lineLength);
}

int main(int argc, char **argv) {
	printf("Welcome to TicTactis Engine\n");
	fprintf(stdout, "Triple T Madness");
	engineSettingsPhase();
	gameSettingsPhase();

	printf("Start game\n");
	initGame();
	gameLoop();
}

void gameLoop(){
	while(!state.isWon()){
		state.print();
		MoveDescriptor desc = getDescription();
		//FIXME according to protocol
		state.playMove(Move(desc));
	}
	winGame(state.hasWon(true));
}

void nextInitMove(MoveDescriptor *array, int& existing, bool playerOne){
	goToNextLine();
	if(matches(line, reg_move)){
		MoveDescriptor desc = getMoveDescriptor(line);
		Move m(desc);
		if(!state.isValidMove(m)){
			fprintf(stderr, "Invalid move\n");
			looseGame(playerOne);
		}
		array[existing] =
		existing++;
	}else if(matches(line, reg_engineOp)){

	}else{
		fprintf(stderr, "Wrong input\n");
		winGame(!playerOne);
	}
}

void initGame(){
	MoveDescriptor playerOne[9];
	MoveDescriptor playerTwo[9];

	int done = 0;
	do {
		goToNextLine();
	}while(!matches(line, reg_initP1));
	while(done < 9) nextInitMove(playerOne, done, true);

	done = 0;
	do {
		goToNextLine();
	}while(!matches(line, reg_initP2));
	while(done < 9) nextInitMove(playerTwo, done, false);

	InitResult result;
	result.type = INVALID;
	result.info.index = -1;
	result = state.initializeWithMoves(playerOne, playerTwo);
	if(result.type != PASSED){
		fprintf(stderr, "%u %u %u\n", result.type, result.info.index, result.info.playerOne);
		if(result.type == NOT_DURING_INIT){
			dbgPrint("The game is already ongoing, what were you thinking");
		}else{
			dbgPrint("Some player failed to give correct input");
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
			dbgPrint("%s has a%s with move %s%s.", player, fault.data(), move.data(), additional.data());
		}
		looseGame(result.info.playerOne);
	}
}

void looseGame(bool playerOne){
	printf("Player %s lost himself the game\n", playerOne?p1:p2);
	winGame(!playerOne);
}

void winGame(bool winnerOne){
	printf("Player %s has won\n", winnerOne?p1:p2);
	endGame();
}

void endGame(){
	state.print();
	printf("End TTTP\n");
	exit(0);
}

/**
 * Sets up the engine with the current line
 */
void setupEngine(){

}

void engineSettingsPhase(){
	for(goToNextLine(); !matches(line, reg_endSettings); goToNextLine()){
		setupEngine();
	}
}

/**
 * Sets up the game with the current line
 */
void setupGame(){

}

void gameSettingsPhase(){
	for(goToNextLine(); !matches(line, reg_endSettings); goToNextLine()){
		setupGame();
	}
}
