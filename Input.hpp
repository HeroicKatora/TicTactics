/*
 * Input.hpp
 *
 *  Created on: 18.05.2015
 *      Author: Andreas Molzer
 */
#pragma once
#include "Move.hpp"
#include <cstdio>

MoveDescriptor getMoveDescriptionFromInput(char * input);

MoveDescriptor getDescriptionOnStream();
