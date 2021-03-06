/*
 * TTTP.cpp
 *
 *  Created on: 19.05.2015
 *      Author: Andreas Molzer
 */

#include "TTTP.hpp"

CC TTTPConst::p1 [] = "Player One";
CC TTTPConst::p2 [] = "Player Two";

CC TTTPConst::VALID = '*';
CC TTTPConst::EMPTY = '_';

CC TTTPConst::Set1W1 = 'X';
CC TTTPConst::Set1WA = 'x';

CC TTTPConst::Set3W0 = '%';
CC TTTPConst::Set3W1 = '+';
CC TTTPConst::Set3W3 = '&';
CC TTTPConst::Set3W2 = '0';

CC TTTPConst::Set2W2 = 'O';
CC TTTPConst::Set2WA = 'o';

CC TTTPConst::channelDebug [] = "TTTP:Debug";
CC TTTPConst::channelChat [] = "TTTP:Chat";
CC TTTPConst::channelMeta [] = "TTTP:Meta";
CC TTTPConst::channelEngine [] = "TTTP:Engine";

CC TTTPConst::lineSettingsEngine [] = "Engine setting";
CC TTTPConst::lineSettingsGame [] = "Game settings";
CC TTTPConst::lineSettingsEnd [] = "End settings";

CC TTTPConst::linePrintFull [] = "Print full";

CC TTTPConst::linePlayerLost [] = "%s lost himself the game";
CC TTTPConst::linePlayerWon [] = "%s has won";
CC TTTPConst::linePlayerReady [] = "%s is ready";
CC TTTPConst::lineInvalidMove [] = "Invalid move";
CC TTTPConst::lineInitError [] = "Init error %u %u %u";
CC TTTPConst::lineWrongInput [] = "Wrong input";
CC TTTPConst::lineStartGame [] = "Start game";
CC TTTPConst::lineEndTTTP [] = "End TTTP";

CC EngineConstants::name [] = "Triple T Madness v0.1";
CC EngineConstants::welcome [] = "Welcome to TicTactis Engine";

CC EngineConstants::commentInitIncorrectInput [] = "Some player failed to give correct input";
CC EngineConstants::commentInitMistake [] = "%s has a%s with move %s%s.";
CC EngineConstants::commentInitPlayerReady [] = "%s is ready";
CC EngineConstants::commentInitDuringGame [] = "The game is already ongoing, what were you thinking";

CC EngineConstants::initDuplicate [] = " duplicate move";
CC EngineConstants::initInvalid [] = "n invalid move";
CC EngineConstants::initMiddleMove [] = " middle move";
CC EngineConstants::initBoardOverflow [] = " field overflow";
CC EngineConstants::initFieldOverflow [] = " board overflow";

CC EngineConstants::initMiddleComment [] = "\nYou can not move into the mid during initial phase.";
CC EngineConstants::initBoardComment [] = "\nYou can only place one mark on equivalent fields.";
CC EngineConstants::initFieldComment [] = "\nYou can only place two marks on the same board.";

CC EngineConstants::computationTimeExceeded [] = "Maximal computation time was exceeded";
CC EngineConstants::depthLimitReached [] = "Computation depth limit was reached";
