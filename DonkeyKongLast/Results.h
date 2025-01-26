#pragma once

#include <list>

/**
 * @brief Manages a list of game results, including event iterations and result types.
 */

 // ------------------- Class Declaration -------------------
class Results {
public:
	enum ResultValue { death, finished, noResult };
	enum ErrorValue { NO_DEATH = 0, NO_FINISH = 0 };

private:
	std::list<std::pair<size_t, ResultValue>> results; ///< List of iteration-result pairs

public:

	// ------------------- Result Manager -------------------
	static Results loadResults(const std::string& filename);
	void saveResults(const std::string& filename,int score) const;
	void addResult(size_t iteration, ResultValue result) {
		results.push_back({ iteration, result });
	}
	std::pair<size_t, ResultValue> popResult();
	void clearResults() {
		results.clear();
	}

	// ------------------- Getters -------------------
	size_t getNextDeathIteration() const;
	size_t getFinishedIteration() const;
	bool isFinishedBy(size_t iteration) const {
		return results.empty() || results.back().first <= iteration;
	}
	size_t lastEventIteration() const{
		return(!results.empty() ? results.back().first : 0);
	}
	
};
