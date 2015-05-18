/*
 * main.cpp
 *
 *  Created on: 18.05.2015
 *      Author: Andreas Molzer
 */
#include <cstdio>
#include "Input.hpp"
#include "GameState.hpp"

void gameLoop(GameState& state){
	while(!state.isWon()){
		printf(state.isPlayerOneTurn()?"Player One move\n":"Player Two move\n");
		MoveDescriptor desc = getDescriptionOnStream(stdin);
		state.playMove(Move(desc));
	}
}

void initGame(GameState& state){

}

int main(int argc, char **argv) {
	GameState state{};
	puts("Welcome to TicTactis Engine");
	initGame(state);
	gameLoop(state);
}
