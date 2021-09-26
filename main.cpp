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

#include "main.h"

int main(int agrc, char **argv)
{
		dataKeeper myData;
		std::ifstream learningFile;
		SDLgraphics myWindow;

		std::string results;	// data to show

		agrc = agrc - 1;
		char * openedFile = argv[1];
		int pressedBut = 0;
		std::string pickedWord;

		if (agrc != ALLOWED_AMOUNT_ARGV) {
				printf("WARNING: wrong amount of arguments.\n");
				return 1;
		}

		/* try to open the given file and save all words stored inside */
		learningFile.open(openedFile, std::ios::in);
		if(learningFile.is_open()) {
				std::string line;
				while(getline(learningFile, line)) myData.appendToWords(line);
		} else {
			printf("ERROR: cannot open the requested file, please try again.\n");
			return 1;
		}

		/* start showing the words and do that until an amount of cycles is done */
		for(int cyclesPassed = 0; cyclesPassed < myData.wordsAmount; cyclesPassed++)
		{
				//char * myWord = (char *)myData.randomlyGiveWords().c_str();
				myWindow.createNewWindow(0, 0);
				/* shows and destorys it */
				pickedWord = myData.randomlyGiveWords().c_str();
				pressedBut = myWindow.showWindow("The word to learn:", pickedWord.c_str());
				switch(pressedBut) {
						case 0: // no
								myData.setWordStatus(false, pickedWord);
								break;
						case 1: // yes
								myData.setWordStatus(true, pickedWord);
								break;
						case 2: // a termination of the program - normal clearing
								printf("DEBUG: Thanks for using the program, good bye!\n");
								return 0;
						default:
								break;
				}
				if (myData.showStopperA >= ALLOWED_AMOUNT_LINES ||
						myData.showStopperB >= ALLOWED_AMOUNT_LINES ||
						myData.showStopperA + myData.showStopperB >= ALLOWED_AMOUNT_LINES
				) break;	// quit the cycle if all processed
				std::this_thread::sleep_for(std::chrono::seconds(myData.getNextTime())); // now sleep
		}

		// show results
		myData.giveResults(results);
		myWindow.createResultWindow(0, 0);
		myWindow.showWindowOneButton(results.c_str());

		return 0;
}