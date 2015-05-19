/*
 * Output.cpp
 */

#include "Output.hpp"

#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <mutex>

std::mutex outMut{};
std::mutex errMut{};

void printOut(char * content ...){
	char out[TTTPConst::lineLength];
	sprintf(out, "%s\n", content);
	va_list args;
	va_start(args, content);
	outMut.lock();
	vfprintf(stdout, out, args);
	outMut.unlock();
	va_end(args);
}

void printErr(char * content, ...){
	char out[TTTPConst::lineLength];
	sprintf(out, "%s\n", content);
	va_list args;
	va_start(args, content);
	errMut.lock();
	vfprintf(stderr, out, args);
	errMut.unlock();
	va_end(args);
}

void infoPrint(char * s, ...){
	char out[TTTPConst::lineLength];
	sprintf(out, ":%s:", s);
	va_list args;
	va_start(args, s);
	printOut(out, args);
	va_end(args);
}

int printMove(Move& move) {
	char mv[20];
	int ret = sprintMove(mv, move);
	printOut(mv);
	return ret;
}

int printMove(MoveDescriptor& desc) {
	char mv[20];
	int ret = sprintMove(mv, desc);
	printOut(mv);
	return ret;
}

int sprintMove(char* dest, Move& move) {
	return sprintf(dest, "B%uF%u", move.getBoardSet(), getBoardOfField(move.getFieldSet()));
}

int sprintMove(char* dest, MoveDescriptor& descriptor) {
	return sprintf(dest, "B%uF%u", descriptor.getBoardIndex(), descriptor.getFieldIndex());
}

int sprintRow(char * dest,TicTacBoard& board, int row, GameState * surrGame, int boarInd){
	int off = 0;
	bool p1 = board.hasPlayerOneWon(), p2 = board.hasPlayerTwoWon();
	for(int j = 0;j<3;j++){
		char c = TTTPConst::EMPTY;
		if(surrGame){
			Move corrMove = Move(boarInd, 0x1<<(row*3+j));
			if(surrGame->isValidMove(corrMove)) c = TTTPConst::VALID;
		}
		if(board.setPlayerOne.getStateOfCell(j+row*3) &&
				!board.setPlayerTwo.getStateOfCell(j+row*3)){ //One has this
			if(p1){
				c = TTTPConst::Set1W1;
			}else{
				c = TTTPConst::Set1WA;
			}
		}else if(board.setPlayerOne.getStateOfCell(j+row*3)){ //Both have this
			if(p1 && !p2){	//p1 won
				c = TTTPConst::Set3W1;
			}else if(p1){	//Both won
				c = TTTPConst::Set3W3;
			}else if(p2){	//p2 won
				c = TTTPConst::Set3W2;
			}else{			//None won
				c = TTTPConst::Set3W0;
			}
		}else if(board.setPlayerTwo.getStateOfCell(j+row*3)){ //Two has this
			if(board.hasPlayerTwoWon()){
				c = TTTPConst::Set2W2;
			}else{
				c = TTTPConst::Set2WA;
			}
		}
		off += sprintf(dest+off, "%c", c);
	}
	return off;
}

int printRow(TicTacBoard& board, int row, GameState * surrGame, int boarInd){
	char mv [10];
	int ret = sprintRow(mv, board, row, surrGame, boarInd);
	printOut(mv);
	return ret;
}

int printBoard(TicTacBoard& board) {
	char mv [15];
	int ret = sprintBoard(mv, board);
	printOut(mv);
	return ret;
}

int printBigBoard(TacTicBoard& board){
	return printBigBoard(board, NULL);
}

int printBigBoard(TacTicBoard& board, GameState * surrGame) {
	char mv [155];
	int ret = sprintBigBoard(mv, board, surrGame);
	printOut(mv);
	return ret;
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
