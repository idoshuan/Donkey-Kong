#pragma once
#include <list>
#include <fstream>

#include "GameConfig.h"
struct Step {
	
	size_t iteration;
	ENTITIES_CHARACTERS entity;
	KEYS key;

};
class Steps {
	long randomSeed = 0;
	std::list<Step> steps; // pair: iteration, entity ,step
public:
	static Steps loadSteps(const std::string& filename);
	void saveSteps(const std::string& filename) const;
	long getRandomSeed() const {
		return randomSeed;
	}
	void setRandomSeed(long seed) {
		randomSeed = seed;
	}
	void addStep(Step step) {
		steps.push_back(step);
	}
	bool isNextStepOnIteration(size_t iteration) const {
		return !steps.empty() && steps.front().iteration == iteration;
	}
	KEYS popStep(size_t iteration) {
		KEYS step = steps.front().key;
		steps.pop_front();
		return step;
	}
};
