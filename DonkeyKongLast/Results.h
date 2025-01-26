#pragma once

#include <list>

class Results {
public:
	enum ResultValue { death, finished ,noResult };
	enum ErrorValue {NO_DEATH = 0, NO_FINISH = 0};
private:
	std::list<std::pair<size_t, ResultValue>> results; // pair: iteration, result
public:
	static Results loadResults(const std::string& filename);
	void saveResults(const std::string& filename,int score) const;
	void addResult(size_t iteration, ResultValue result) {
		results.push_back({ iteration, result });
	}
	std::pair<size_t, ResultValue> popResult() {
		if (results.empty()) return { 0, Results::noResult };
		auto result = results.front();
		results.pop_front();
		return result;
	}
	bool isFinishedBy(size_t iteration) const {
		return results.empty() || results.back().first <= iteration;
	}
	size_t getNextDeathIteration() const;
	size_t getFinishedIteration() const;

	void clearResults() {
		results.clear();
	}
};
