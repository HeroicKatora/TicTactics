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

void gameLoop(GameState& state){
	while(!state.isWon()){
		state.print();
		printf(state.isPlayerOneTurn()?"Player One move\n":"Player Two move\n");
		MoveDescriptor desc = getDescriptionOnStream();
		state.playMove(Move(desc));
	}
	printf(state.hasWon(true)?"Player one has won the game":"Player two has won the game");
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

		//system("cls"); //Only works on Windows/MS DOS
		printf("Player Two, please input initial moves, comma separated\n");
		for(int i = 0;i<8;i++){
			playerTwo[i] = getInitDescriptorOnStream();
		}
		playerTwo[8] = getDescriptionOnStream();
		result = state.initializeWithMoves(playerOne, playerTwo);
		if(result.type != PASSED){
			if(result.type == NOT_DURING_INIT){
				printf("The game is already ongoing, make sure to report this bug!");
			}else{
				printf("Some player failed to give correct input, retry\n");
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
				printf("%s has a%s with move %s%s.\n", player, fault.data(), move.data(), additional.data());
			}
			dbgPrint("%u %u %u\n", result.type, result.info.index, result.info.playerOne);
		}
	}
}

int main(int argc, char **argv) {
	GameState state{};
	printf("Welcome to TicTactis Engine\n");
	initGame(state);
	gameLoop(state);
}
