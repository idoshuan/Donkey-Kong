#pragma once
#include "Mario.h"
#include "Board.h"
#include "Point.h"
#include "Barrel.h"

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>
 

class Game {
	static constexpr int marioInitX = 4;
	static constexpr int marioInitY = 23;
	static constexpr int marioMaxFallHeight = 5;
	static constexpr int initLives = 3;

	static constexpr int numBarrels =10;
	static constexpr int leftBarrelInitX = 37;
	static constexpr int rightBarrelInitX = 39;
	static constexpr int barrelInitY = 2;
	static constexpr int barrelSpawnInterval = 5000;
	static constexpr int firstBarrelSpawnDelay = 4000;
	static constexpr int barrelMaxFallHeight = 8;


	static constexpr int ESC = 27;
	using clock = std::chrono::steady_clock;
	using milliseconds = std::chrono::milliseconds;
	using time = std::chrono::time_point<clock>;

	Mario mario;
	Board board;
	Barrel barrelArr[numBarrels];
	int lives;
	int barrelCount;
	bool firstBarrelSpawned;
	time lastBarrelTime;
	time gameStartTime;

public:
	Game() : mario(marioInitX, marioInitY, &board), lives(initLives), barrelCount(0), firstBarrelSpawned(false) {}

	void startGame();
	void resetGame();
	void marioBlink();
	void checkForKeyPress();
	void eraseCharacters();
	void drawAndMoveCharacters();
	void trySpawnBarrel();
	bool checkMarioDeath();
	bool checkMarioDeathFromBarrel();
	bool isInExplosionRadius(Barrel& barrel);
	bool checkMarioDeathFromFall();
	bool checkBarrelExplode(Barrel& barrel);

};

