/* Copyright: 2021 Donat Zenichev development <donat.zenichev@gmail.com>

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, either version 3 of the License, or
		(at your option) any later version.

		This package is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		On Debian systems, the complete text of the GNU General
		Public License version 3 can be found in "/usr/share/common-licenses/GPL-3". */

#include "dataKeeper.h"

/* constructor */
dataKeeper::dataKeeper()
			: wordsAmount(0), wordsLearned(0), wordsForgotten(0), slotsIDs(0), nextTime(0) {
		myLearnedWords = std::vector<std::string>( ALLOWED_AMOUNT_LINES );
		myForgottenWords = std::vector<std::string>( ALLOWED_AMOUNT_LINES );
};

/* add new words to an existing list */
void dataKeeper::appendToWords(std::string & newWord) {
		std::string wordItself = newWord.substr(0, newWord.find(DELIMITER));
		std::string wordTranslation = newWord.substr(wordItself.length() + 1, newWord.find(DELIMITER));
		myWords.insert(std::make_pair(wordItself, wordTranslation));
		wordsAmount++;
}

/* define whether the word has been forgotten or has been learned */
void dataKeeper::setWordStatus(bool status, std::string & word) {
		if (status) {
				if (!inArray(myLearnedWords, (const std::string) word) &&
						!inArray(myForgottenWords, (const std::string) word)) {
						myLearnedWords[wordsLearned] += word;
						++wordsLearned;
				}
		} else {
				if (!inArray(myForgottenWords, (const std::string) word) &&
						!inArray(myLearnedWords, (const std::string) word)) {
						myForgottenWords[wordsForgotten] += word;
						++wordsForgotten;
				}
		}
}

/* randomly return some word from an array */
std::pair<std::string, std::string> dataKeeper::randomlyGiveWords() {
		std::pair<std::string, std::string> nextWord = *select_randomly(myWords.begin(), myWords.end());
		myWords.erase(nextWord.first); /* remove it, in order to next time not use it again */
		return nextWord;
}

/* return randomly picked amount of seconds till next window appearing */
int dataKeeper::getNextTime() {
		time_t t;
		srand((unsigned) time(&t));
		int nextTime = (rand()%(NEXT_TIME_MAX-NEXT_TIME_MIN))+NEXT_TIME_MIN;
		return nextTime;
}

template<typename Iter, typename RandomGenerator>
Iter dataKeeper::select_randomly(Iter start, Iter end, RandomGenerator& g) {
		std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
		std::advance(start, dis(g));
		return start;
}

template<typename Iter>
Iter dataKeeper::select_randomly(Iter start, Iter end) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		return select_randomly(start, end, gen);
}

/* give a summary on learned and forgotten words */
void dataKeeper::giveResults(std::string & givenResults) {
	givenResults = "[Words learned]\n";
	for (auto it = myLearnedWords.begin(); it != myLearnedWords.end(); ++it)
	{
		std::string tmp = *it;
		if(!tmp.empty()) givenResults = givenResults + "-   " + tmp + "\n";
	}
	givenResults = givenResults + "\n[Words forgotten]\n";
	for (auto it = myForgottenWords.begin(); it != myForgottenWords.end(); ++it)
	{
		std::string tmp = *it;
		if(!tmp.empty()) givenResults = givenResults + "-   " + tmp + "\n";
	}
}

/* save results for the next program run */
int dataKeeper::saveCurrentResults(std::string & givenResults, const char * dir) {
	std::ofstream resultsFile;
	std::string filename = dir;
	filename = filename + "/" + RESULTS_FILE_NAME;

	resultsFile.open(filename, std::ios::out);
	if(resultsFile.is_open()) {
		resultsFile << givenResults;
		resultsFile.close();
		return 0;
	}

	printf("ERROR: cannot create/open the requested file, please try again.\n");
	return 1;
}
