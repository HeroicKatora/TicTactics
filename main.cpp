/*
 * main.cpp
 *
 *  Created on: 18.05.2015
 *      Author: Andreas Molzer
 */
#include <cstdio>
#include "Input.hpp"
#include "GameState.hpp"
#include <cstdlib>

void gameLoop(GameState& state){
	while(!state.isWon()){
		printf(state.isPlayerOneTurn()?"Player One move\n":"Player Two move\n");
		MoveDescriptor desc = getDescriptionOnStream();
		state.playMove(Move(desc));
	}
}

void initGame(GameState& state){
	MoveDescriptor playerOne[9];
	MoveDescriptor playerTwo[9];
	InitResult result;
	result.type = INVALID;
	result.info.index = -1;
	while(result.type != PASSED){
		printf("Player One, please input initial moves, comma separated\n");
		for(int i = 0;i<8;i++){
			playerOne[i] = getInitDescriptorOnStream();
		}
		playerOne[8] = getDescriptionOnStream();

		system("cls"); //Only works on Windows/MS DOS
		printf("Player Two, please input initial moves, comma separated\n");
		for(int i = 0;i<8;i++){
			playerTwo[i] = getInitDescriptorOnStream();
		}
		playerTwo[8] = getDescriptionOnStream();
		result = state.initializeWithMoves(playerOne, playerTwo);
		if(result.type != PASSED){
			printf("Some player failed to give correct input, retry (More info soon)\n");
			printf("%u %u %u\n", result.type, result.info.index, result.info.playerOne);
		}
	}
}

int main(int argc, char **argv) {
	GameState state{};
	printf("Welcome to TicTactis Engine\n");
	initGame(state);
	gameLoop(state);
}
