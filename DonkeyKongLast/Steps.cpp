#include "Steps.h"

/**
 * @brief Saves the steps to a file, including the random seed and step details.
 * @param filename The file to save steps to.
 */
void Steps::saveSteps(const std::string& filename) const {
	std::ofstream steps_file(filename);
	steps_file << randomSeed << '\n' << steps.size();
	for (const auto& step : steps) {
		steps_file << '\n' << step.iteration << ' ' << (char)step.key;
	}
	steps_file.close();
}

/**
 * @brief Loads the steps from a file, including the random seed and step details.
 * @param filename The file to load steps from.
 * @return A Steps object with the loaded data.
 */
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