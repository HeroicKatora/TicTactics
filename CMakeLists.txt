cmake_minimum_required(VERSION 3.2)
project(TicTactics)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -ffast-math")

add_executable(TicTactics main.cpp Board.cpp Debug.hpp EngineController.cpp  Rating.cpp GameState.cpp Input.cpp Output.cpp Searcher.cpp TTTP.cpp Move.cpp RatingTable.h types.hpp WinMoveTable.h)