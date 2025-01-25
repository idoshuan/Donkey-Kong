#include "GameBase.h"
#include "GameFromFile.h"
#include "GameFromInput.h"
#include <iostream>
#include <cstdlib> // for system("pause")


int main(int argc, const char** argv) {
	bool isLoad = argc > 1 && std::string(argv[1]) == "-load";
	bool isSilent = isLoad && argc > 2 && std::string(argv[2]) == "-silent";
	GameBase* game;
	if (isLoad) {
		GameFromFile a = GameFromFile(isSilent);
		game = &a;
	}
	else {
		GameFromInput b = GameFromInput();
		game = &b;
	}
}