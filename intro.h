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
#include <iostream>
#include <cstdio>
#include <stdio.h>

#include <vector>
#include <iterator>

#include <algorithm>

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <curses.h>

#define ALLOWED_AMOUNT_ARGV				10			// allowed amount of arguments
#define MAIN_MENU_GIVEN_ATTEMPTS 	3
#define MAIN_MENU_CHOICE_AMOUNT		4				// possible values for the main menu
#define NEXT_TIME_MIN							5				// min amount of seconds before next appear
#define NEXT_TIME_MAX							10			// max amount of seconds before next appear
#define ALLOWED_TIME_BORDERS_MIN	1				// minimum allowed value for nextTimeMin / nextTimeMax
#define ALLOWED_TIME_BORDERS_MAX	60			// maximum allowed value for nextTimeMin / nextTimeMax
#define MAX_LENGTH_FILE_PATH			100			// maximum allowed length for a file name including a path

/* arguments list */
#define ARG_CLI_MENU			"--cli-menu"		// interactive cli based menu
#define ARG_GUI_MENU			"--gui-menu"		// GUI menu
#define ARG_CLI_MODE			"--cli-mode"		// no menus at all, just cli arguments

#define ARG_OPEN_FILE			"--open-file"		// followed by the path to a file
#define ARG_NEXT_TIME_MIN	"--next-time-min" // followed by min time (sec) before next appear
#define ARG_NEXT_TIME_MAX	"--next-time-max" // followed by max time (sec) before next appear


class mainMenu {
private:
		const char *mainInteractionWindow =
		"Main menu:\n"
		"0. Press to start ;\n"
		"1. Press for definition of time ranges ;\n"
		"2. Press for adding a new source file (words list) ;\n"
		"3. Press for help ;\n"
		"4. Press to quit ;\nEnter choice: ";
		const char *mainHelpInformation =
		"Helpful information:\n"
		"\t- Time ranges\n"
		"\t\tTime ranges - are time borders, which are used in order to tell the program\n"
		"\t\twhat is the approximate value for the random time step choice.\n"
		"\t\tWhat does that mean? The program uses a random value for giving the cards out,\n"
		"\t\twhich allows you to invoke your (human) memory all of the sudden. You don't exactly know\n"
		"\t\twhen the next occurence of the card will happen. And that's the main point of it.\n"
		"\t\tSo that, relax, do your work and when the next card appears, try to recall the word.\n\n"
		"\t- The source file (words/phrases list)\n"
		"\t\tThat is a source file you use, to give the program a list of words/phrases to learn.\n"
		"\t\tThe syntax for defining words is as follows:\n"
		"\t\t\t<word>:<translation>\n"
		"\t\tEach new word must be defined beginning from the next row, consider this:\n"
		"\t\t\t$bash> cat /tmp/my_words_list.txt\n"
		"\t\t\t\tdie Vermittlung:a mediation\n"
		"\t\t\t\tdie Vorwarnung:a warning\n"
		"\t\t\t\tdie Einschätzung:an estimation\n"
		"\t\tThe maximum amount of words is limited by the definition: ALLOWED_AMOUNT_LINES.\n\n"
		"\t- Options to be used in --cli-mode\n"
		"\t\t'--open-file' - a source file to open.\n"
		"\t\t'--next-time-min' - the minimum time step for randomity.\n"
		"\t\t'--next-time-max' - the maximum time step for randomity.\n"
		"\nPress any key to continue.. ";

		char * ch = (char*) malloc( sizeof(char) * ( 2 + 1 ) ); /* give a possibility to enter 2digit*/
		int nextTimeMin;
		int nextTimeMax;

		int argvAmount;

public:
		std::vector<std::string> argvList = std::vector<std::string>( ALLOWED_AMOUNT_ARGV );
		//static char openFile[MAX_LENGTH_FILE_PATH];
		//char openFile[MAX_LENGTH_FILE_PATH];
		char * openFile = (char*) malloc( sizeof(char) * ( MAX_LENGTH_FILE_PATH + 1 ) );
public:
		mainMenu();
		//~mainMenu();

		/* interaction via cli based interactive menu */
		int launchInteractionWindow();
		int defineTimeRanges();
		int addNewSourceList();
		int provideHelp();

		/* interaction via cli arguments */
		int CLIappendToArgvList(std::string & arg);
		int CLIgetPathSourceFile();
		void CLIdefineTimeRanges();

		/* other */
		int defineTheMode();

		/* search for an argument in arguments list */
		bool inArguments(const std::vector<std::string> & array, const std::string & value) {
				return std::find(array.begin(), array.end(), value) != array.end() ? true : false;
		}

};