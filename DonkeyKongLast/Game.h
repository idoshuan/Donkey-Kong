#pragma once
#include "Mario.h"
#include "Board.h"
#include "Point.h"
#include "Barrel.h"

#include <iostream>
#include <windows.h>
#include <conio.h>


class Game {
	static constexpr int numBarrels =10;
	static constexpr int initMarioX = 4;
	static constexpr int initMarioY = 23;
	static constexpr int initLeftBarrelX = 37;
	static constexpr int initRightBarrelX = 39;
	static constexpr int initBarrelY = 3;
	static constexpr int ESC = 27;

	Mario mario;
	Board pBoard;
	Barrel barrelArr[numBarrels];

public:
	Game() : mario(initMarioX, initMarioY, &pBoard) {
		initBarrels();
	}

	void startGame();
	void initBarrels() {
		for (int i = 0; i < numBarrels; i++) {
			if (i % 2 == 0) {
				barrelArr[i] = Barrel(initLeftBarrelX, initBarrelY, &pBoard);  
			}
			else {
				barrelArr[i] = Barrel(initRightBarrelX, initBarrelY, &pBoard);  
			}
		}
};

