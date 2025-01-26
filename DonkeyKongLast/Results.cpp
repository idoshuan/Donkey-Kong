#include <fstream>
#include "Results.h"

/**
 * @brief Loads results from a file.
 * @param filename The file to load results from.
 * @return A Results object with the loaded data.
 */
Results Results::loadResults(const std::string& filename) {
    std::ifstream results_file(filename);
    Results results;
    size_t size;

    results_file >> size;

    while (!results_file.eof() && size-- != 0) {
        size_t iteration;
        int result;
        results_file >> iteration >> result;
        results.addResult(iteration, static_cast<ResultValue>(result));
    }

    return results;
}

/**
 * @brief Saves results to a file, including the results list and a score.
 * @param filename The file to save results to.
 * @param score The score to include in the saved data.
 */
void Results::saveResults(const std::string& filename, int score) const {
    std::ofstream results_file(filename);

    results_file << results.size();

    for (const auto& result : results) {
        results_file << '\n' << result.first << ' ' << result.second;
    }

    results_file << '\n' << score;

    results_file.close();
}

/**
 * @brief Gets the iteration of the next "death" result.
 * @return The iteration of the next "death" result, or NO_DEATH if none exist.
 */
size_t Results::getNextDeathIteration() const {
    if (!results.empty() && results.front().second == death) {
        return results.front().first;
    }
    return NO_DEATH;
}

/**
 * @brief Gets the iteration of the next "finished" result.
 * @return The iteration of the next "finished" result, or NO_FINISH if none exist.
 */
size_t Results::getFinishedIteration() const {
    if (!results.empty() && results.front().second == finished) {
        return results.front().first;
    }
    return NO_FINISH;
}

/**
 * @brief Removes and returns the first result in the list.
 * @return A pair containing the iteration and result type of the first result, or noResult if empty.
 */
std::pair<size_t, Results::ResultValue> Results::popResult() {
    if (results.empty()) {
        return { 0, Results::noResult };
    }

    auto result = results.front();
    results.pop_front();
    return result;
}
