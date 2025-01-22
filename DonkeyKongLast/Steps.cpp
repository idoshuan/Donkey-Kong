#include "Steps.h"




void Steps::saveSteps(const std::string& filename) const {
	std::ofstream steps_file(filename);
	steps_file << randomSeed << '\n' << steps.size();
	for (const auto& step : steps) {
		steps_file << '\n' << step.iteration << ' ' << (char)step.entity << ' ' << (char)step.key;
	}
	steps_file.close();
}
