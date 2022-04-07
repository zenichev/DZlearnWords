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
		std::ifstream learningFile;

		/* general required classes for the work */
		mainMenu myMainMenu;
		dataKeeper myData;
		SDLgraphics myWindow;

		/* work with words and buttons on cards */
		int pressedBut = 0;
		std::pair<std::string, std::string> pickedWord;

		/* data to show */
		std::string results;

		/* collect given arguments */
		int tmp = agrc;
		while (--tmp > 0) {
			std::string argument(argv[tmp]);
			if (argument.empty()) continue;
			if (myMainMenu.CLIappendToArgvList(argument)) {
				printf("WARNING: Something went wrong while appending arguments, terminating..\n");
				return 1;
			}
		}
		/* re-order arguments list, to let it look originally in the program */
		std::reverse(myMainMenu.argvList.begin(), myMainMenu.argvList.end());

		/* parse arguments and define the interaction mode
			0 - interaction only via cli arguments
			1 - interaction via usual cli menu
			2 - interaction via gui menu */
		int interactionMode = myMainMenu.defineTheMode();

		/* ret: 0 - start processing cards / 1 - normal clearing / -1 - something went wrong */
		int ret = 0;

		switch (interactionMode) {
			/* interaction only via cli arguments */
			case 0:
				if (!myMainMenu.inArguments(myMainMenu.argvList, ARG_OPEN_FILE)) {
					printf("WARNING: --cli-mode picked out, but no source file provided.\n");
					ret = -1;
					break;
				}
				ret = myMainMenu.CLIgetPathSourceFile();	/* get file to open */
				myMainMenu.CLIdefineTimeRanges();					/* define borders for a random time step */
				break;
			/* interaction via usual cli menu */
			case 1:
				ret = myMainMenu.launchInteractionWindow(); /* start usual cli menu */
				break;
			/* interaction via gui menu */
			case 2:
				printf("WARNING: Sorry, but the interaction via GUI is not available at the moment.\n");
				printf("WARNING: Try to use the program in CLI mode / CLI menu mode\n");
				ret = -1;
				break;
			default:
				printf("WARNING: Cannot determine the interaction mode, terminating..\n");
				ret = -1;
				break;
		}

		/* basic checks before start flipping cards */
		switch (ret) {
			case -1:
				printf("ERROR: Something went wrong, terminating the program.\n");
				return 1;
			case 1:
				printf("DEBUG: Normal program clearing.\n");
				return 0;
		}

		if (strlen(myMainMenu.openFile) == 0 ) {
				printf("ERROR: Something went wrong, the source file wasn't defined properly.\n");
				printf("\tHave you given the proper path to a file? Try to launch the program again!\n");
				return 1;
		}

		/* try to open the given file and save all words in the memory stored inside */
		learningFile.open(myMainMenu.openFile, std::ios::in);
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
				myWindow.createNewWindow(0, 0, WINDOW_TYPE_QUIZ);
				/* shows and destorys it */
				pickedWord = myData.randomlyGiveWords();
				pressedBut = myWindow.showWindow("The word to learn:", pickedWord.first.c_str());
				switch(pressedBut) {
						case 0: /* no */
								myData.setWordStatus(false, pickedWord.first);
								myWindow.createNewWindow(0, 0, WINDOW_TYPE_NO_ANSWER);
								myWindow.showWindow("The translation was:", pickedWord.second.c_str());
								break;
						case 1: /* yes */
								myData.setWordStatus(true, pickedWord.first);
								break;
						case 2: /* a termination of the program - normal clearing */
								printf("DEBUG: Thanks for using the program, good bye!\n");
								return 0;
						default:
								break;
				}

				/* check if we have to stop cycline already now */
				if (myData.showStopperA >= myData.wordsAmount ||
						myData.showStopperB >= myData.wordsAmount ||
						myData.showStopperA + myData.showStopperB >= myData.wordsAmount ||
						myData.showStopperA >= ALLOWED_AMOUNT_LINES ||
						myData.showStopperB >= ALLOWED_AMOUNT_LINES ||
						myData.showStopperA + myData.showStopperB >= ALLOWED_AMOUNT_LINES
				) break;	/* quit the cycle if all processed */
				std::this_thread::sleep_for(std::chrono::seconds(myData.getNextTime())); /* now sleep */
		}

		/* show results */
		myData.giveResults(results);
		myWindow.createNewWindow(0, 0, WINDOW_TYPE_RESULT);
		myWindow.showWindow("Given results:", results.c_str());
		return 0;
}