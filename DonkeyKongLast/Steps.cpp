#include "Steps.h"




void Steps::saveSteps(const std::string& filename) const {
	std::ofstream steps_file(filename);
	steps_file << randomSeed << '\n' << steps.size();
	for (const auto& step : steps) {
		steps_file << '\n' << step.iteration << ' ' << (char)step.key;
	}
	steps_file.close();
}

Steps Steps::loadSteps(const std::string& filename) {
	Steps steps;
	std::ifstream steps_file(filename);
	steps_file >> steps.randomSeed;
	size_t size;
	steps_file >> size;
	while (!steps_file.eof() && size-- != 0) {
		size_t iteration;
		char keyChar; 

		steps_file >> iteration >> keyChar;

		KEYS key = static_cast<KEYS>(keyChar);

		steps.addStep({ iteration, key });
	}
	steps_file.close();
	return steps;
}