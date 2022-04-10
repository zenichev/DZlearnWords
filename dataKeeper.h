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

#include <fstream>
#include <cstring>
#include <chrono>
#include <thread>
#include "SDL2/SDL.h"

#include <vector>
#include <iterator>
#include <map>

#include <random>
#include <bits/stdc++.h>

#include <iostream>
#include <cstdio>
#include <stdio.h>

#include <algorithm>

#define ALLOWED_AMOUNT_LINES    50      // alowed amount of processable text lines

#define NEXT_TIME_MIN           5       // min amount of seconds before next appear
#define NEXT_TIME_MAX           10      // max amount of seconds before next appear

#define DELIMITER               ":"     // delimiter used to divide word and translation

/* the class which is dedicated to work with the data */
class dataKeeper {
private:
		std::map<std::string,std::string> myWords;
		std::vector<std::string> myLearnedWords;
		std::vector<std::string> myForgottenWords;

		int wordsLearned;
		int wordsForgotten;
		int slotsIDs;
		int nextTime;

		bool inArray(const std::vector<std::string> & array, const std::string & value) {
				return std::find(array.begin(), array.end(), value) != array.end() ? true : false;
		}

		bool inIDsArray(const std::vector<int> & array, const int & value) {
				return std::find(array.begin(), array.end(), value) != array.end() ? true : false;
		}
public:
		int wordsAmount;
		const int & showStopperA = wordsLearned; /* word learned */
		const int & showStopperB = wordsForgotten; /* words forgotten */
public:
		dataKeeper();
		~dataKeeper() {}

		void appendToWords(std::string & newWord);
		void setWordStatus(bool status, std::string & word);
		void showTranslation();
		std::pair<std::string, std::string> randomlyGiveWords();

		int getNextTime();

		template<typename Iter, typename RandomGenerator>
		Iter select_randomly(Iter start, Iter end, RandomGenerator& g);

		template<typename Iter>
		Iter select_randomly(Iter start, Iter end);

		void giveResults(std::string & givenResults);
};