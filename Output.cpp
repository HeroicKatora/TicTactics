/*
 * Output.cpp
 *
 *  Created on: 19.05.2015
 *      Author: Andreas Molzer
 */

#include "Output.hpp"
#include <string.h>
#include <cstdio>
#include <cstdlib>

int printMove(Move& move) {
	return printf("B%uF%u", move.getBoardSet(), getBoardOfField(move.getFieldSet()));
}

int printMove(MoveDescriptor& desc) {
	return printf("B%uF%u", desc.getBoardIndex(), desc.getFieldIndex());
}

int sprintMove(char* dest, Move& move) {
	return sprintf(dest, "B%uF%u", move.getBoardSet(), getBoardOfField(move.getFieldSet()));
}

int sprintMove(char* dest, MoveDescriptor& descriptor) {
	return sprintf(dest, "B%uF%u", descriptor.getBoardIndex(), descriptor.getFieldIndex());
}

int printRow(TicTacBoard& board, int row, GameState * surrGame, int boarInd){
	int off = 0;
	bool p1 = board.hasPlayerOneWon(), p2 = board.hasPlayerTwoWon();
	for(int j = 0;j<3;j++){
		char c = EMPTY;
		if(surrGame){
			Move corrMove = Move(boarInd, 0x1<<(row*3+j));
			if(surrGame->isValidMove(corrMove)) c = VALID;
		}
		if(board.setPlayerOne.getStateOfCell(j+row*3) &&
				!board.setPlayerTwo.getStateOfCell(j+row*3)){ //One has this
			if(p1){
				c = Set1W1;
			}else{
				c = Set1WA;
			}
		}else if(board.setPlayerOne.getStateOfCell(j+row*3)){ //Both have this
			if(p1 && !p2){	//p1 won
				c = Set3W1;
			}else if(p1){	//Both won
				c = Set3W3;
			}else if(p2){	//p2 won
				c = Set3W2;
			}else{			//None won
				c = Set3W0;
			}
		}else if(board.setPlayerTwo.getStateOfCell(j+row*3)){ //Two has this
			if(board.hasPlayerTwoWon()){
				c = Set2W2;
			}else{
				c = Set2WA;
			}
		}
		off += printf("%c", c);
	}
	return off;
}

int sprintRow(char * dest,TicTacBoard& board, int row, GameState * surrGame, int boarInd){
	int off = 0;
	bool p1 = board.hasPlayerOneWon(), p2 = board.hasPlayerTwoWon();
	for(int j = 0;j<3;j++){
		char c = EMPTY;
		if(surrGame){
			Move corrMove = Move(boarInd, 0x1<<(row*3+j));
			if(surrGame->isValidMove(corrMove)) c = VALID;
		}
		if(board.setPlayerOne.getStateOfCell(j+row*3) &&
				!board.setPlayerTwo.getStateOfCell(j+row*3)){ //One has this
			if(p1){
				c = Set1W1;
			}else{
				c = Set1WA;
			}
		}else if(board.setPlayerOne.getStateOfCell(j+row*3)){ //Both have this
			if(p1 && !p2){	//p1 won
				c = Set3W1;
			}else if(p1){	//Both won
				c = Set3W3;
			}else if(p2){	//p2 won
				c = Set3W2;
			}else{			//None won
				c = Set3W0;
			}
		}else if(board.setPlayerTwo.getStateOfCell(j+row*3)){ //Two has this
			if(board.hasPlayerTwoWon()){
				c = Set2W2;
			}else{
				c = Set2WA;
			}
		}
		off += sprintf(dest+off, "%c", c);
	}
	return off;
}

int printBoard(TicTacBoard& board) {
	int off = 0;
	for(int i = 0;i<3;i++){
		off += printRow(board, i, NULL, 0);
		off += printf("\n");
	}
	return off;
}

int printBigBoard(TacTicBoard& board){
	return printBigBoard(board, NULL);
}

int printBigBoard(TacTicBoard& board, GameState * surrGame) {
	int off = 0;
	for(int i = 0;i<3;i++){
		for(int j = 0;j<3;j++){
			off += printRow(board.components[3*i], j, surrGame, 3*i);
			off += printf(" ");
			off += printRow(board.components[3*i+1], j, surrGame, 3*i+1);
			off += printf(" ");
			off += printRow(board.components[3*i+2], j, surrGame, 3*i+2);
			off += printf("\n");
		}
		off += printf("\n");
	}
	return off;
}

int sprintBoard(char* dest, TicTacBoard& board) {
	int off = 0;
	for(int i = 0;i<3;i++){
		off += sprintRow(dest+off,board, i, NULL, 0);
		off += sprintf(dest+off, "\n");
	}
	return off;
}

int sprintBigBoard(char* dest, TacTicBoard& board){
	return sprintBigBoard(dest, board, NULL);
}

int sprintBigBoard(char* dest, TacTicBoard& board, GameState * surrGame) {
	int off = 0;
	for(int i = 0;i<3;i++){
		for(int j = 0;j<3;j++){
			off += sprintRow(dest+off, board.components[3*i], j, surrGame, 3*i);
			off += sprintf(dest+off, " ");
			off += sprintRow(dest+off, board.components[3*i+1], j, surrGame, 3*i+1);
			off += sprintf(dest+off, " ");
			off += sprintRow(dest+off, board.components[3*i+2], j, surrGame, 3*i+2);
			off += sprintf(dest+off, "\n");
		}
		off += sprintf(dest+off, "\n");
	}
	return off;
}
