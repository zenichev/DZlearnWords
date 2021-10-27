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

#include "intro.h"

/* constructor */
mainMenu::mainMenu() :
	nextTimeMin(NEXT_TIME_MIN), nextTimeMax(NEXT_TIME_MAX), argvAmount(0) {};

/* general interaction window with possible options
	-1 - terminating the programm
	 0 - start flipping the cards
*/
int mainMenu::launchInteractionWindow() {

		int takenAttempts = 0;
		int temp = 0, choice = 0, base = 10;
		int ret = 0;
		char * end;
		bool notDone = true;

		/* allow a choice only within a given range */
		START_CYCLING:
		std::system("clear");
		printf("%s", mainMenu::mainInteractionWindow);

		takenAttempts = 0;

		memset(mainMenu::ch,0,sizeof(mainMenu::ch));
		while (takenAttempts < MAIN_MENU_GIVEN_ATTEMPTS)
		{
			if ( (temp=scanf("%s",mainMenu::ch)) <= 0 ) {
				printf("WARNING: Impossible choice, are you giving a char? Try again.\n");
				goto AGAIN;
			}

			/* convert into long int */
			choice=std::strtol(mainMenu::ch, &end, base);
			if (end == NULL || (strlen(end) != 0)) {
				printf("WARNING: Impossible choice, are you giving a char? Try again.\n");
				goto AGAIN;
			}

			/* make sure it is not out of range */
			if (choice > MAIN_MENU_CHOICE_AMOUNT) {
				printf("WARNING: Your choice is out of range, try again.\n");
				goto AGAIN;
			}

			break; /* everything fine */

			AGAIN:	/* if checks failed try scanf once again unless all given attempts are out */
			std::system("clear");
			takenAttempts++;
			temp = 0;
			if (takenAttempts < MAIN_MENU_GIVEN_ATTEMPTS) printf("%s", mainMenu::mainInteractionWindow);
		}

		/* general menu */
		switch(choice) {
			/* start flipping the cards */
			case 0:
				return 0;
			/* for definition of time ranges */
			case 1:
				if ((ret = mainMenu::defineTimeRanges()) != 0) {
					printf("WARNING: Something went wrong. Terminating the program.\n");
					return -1;
				}
				break;
			/* for adding a new source file (words list) */
			case 2:
				if ((ret = mainMenu::addNewSourceList()) != 0) {
					printf("WARNING: Something went wrong. Terminating the program.\n");
					return -1;
				}
				break;
			/* for help */
			case 3:
				if ((ret = mainMenu::provideHelp()) != 0) {
					printf("WARNING: Something went wrong. Terminating the program.\n");
					return -1;
				}
				break;
			/* to quit */
			case 4:
				printf("DEBUG: Terminating the program..\n");
				return 1;
			/* by default - terminate */
			default:
				printf("DEBUG: Sorry, but all of your choices are not correct. Terminating the program.\n");
				sleep(2);
				return -1;
		}
		/* there is some work left to do */
		goto START_CYCLING;
}

/* define time ranges, which control time steps of a window pop up */
int mainMenu::defineTimeRanges() {

		int temp = 0;

		std::system("clear");
		printf("Define time ranges:\n\tMinimum time step for randomity (without quotes): ");

		if ((temp=scanf("%d",&(mainMenu::nextTimeMin))) <= 0) {
					printf("\tWARNING: Impossible to set this value, setting to default.\n\n");
					mainMenu::nextTimeMin = ALLOWED_TIME_BORDERS_MIN;
		}
		if (mainMenu::nextTimeMin < ALLOWED_TIME_BORDERS_MIN ||
				mainMenu::nextTimeMin > ALLOWED_TIME_BORDERS_MAX) {
			printf("\tWARNING: The given value is outside of the borders, setting to default.\n\n");
			mainMenu::nextTimeMin = ALLOWED_TIME_BORDERS_MIN;
		}

		printf("\tMaximum time step for randomity (without quotes): ");

		if ((temp=scanf("%d",&(mainMenu::nextTimeMax))) <= 0) {
					printf("\tWARNING: Impossible to set this value, setting to default.\n\n");
					mainMenu::nextTimeMin = ALLOWED_TIME_BORDERS_MAX;
		}
		if (mainMenu::nextTimeMax < ALLOWED_TIME_BORDERS_MIN ||
				mainMenu::nextTimeMax > ALLOWED_TIME_BORDERS_MAX) {
			printf("\tWARNING: The given value is outside of the borders, setting to default.\n\n");
			mainMenu::nextTimeMax = ALLOWED_TIME_BORDERS_MAX;
		}

		return 0;
}

/* for adding a new source file (words list) */
int mainMenu::addNewSourceList() {
		int temp = 0;
		int takenAttempts = 0;

		memset(mainMenu::openFile,0,MAX_LENGTH_FILE_PATH + 1);

		std::system("clear");
		printf("Define a source file to read (without quotes): ");

		while ((temp=scanf("%s",mainMenu::openFile)) <= 0 &&
			takenAttempts < MAIN_MENU_GIVEN_ATTEMPTS) {
				printf("\tWARNING: Impossible to set this value, try again.\n");
				printf("\tDefine a source file to read (without quotes): ");
				takenAttempts++;
		}

		if (takenAttempts >= MAIN_MENU_GIVEN_ATTEMPTS) {
				printf("DEBUG: Sorry, max attempts have been reached. Terminating the program.\n");
				return -1;
		}

		return 0;
}

/* basic help */
int mainMenu::provideHelp() {
		std::system("clear");

		/* a way to implement 'press any key' using ncurses, not the best one */
		initscr();
		addstr( mainMenu::mainHelpInformation );
		cbreak();    /* turn off line-buffering */
		noecho();    /* turn off character echo */
		getch();     /* read and discard a single character (caveats!) */
		echo();      /* turn echo back on */
		nocbreak();  /* turn line-buffering back on */
		endwin();

		return 0;
}

/*
 * work with arguments
 */

/* add new words to an existing list */
int mainMenu::CLIappendToArgvList(std::string & arg) {
		if (argvAmount > ALLOWED_AMOUNT_ARGV) {
			printf("WARNING: Impossible to add new argument, possible amount of argv list exceeded.\n");
			return -1;
		}
		argvList[argvAmount] += arg;
		argvAmount++;
		return 0;
}

/* using given arguments define the interaction mode
	0 - interaction only via cli arguments
	1 - interaction via usual cli menu
	2 - interaction via gui menu
	*/
int mainMenu::defineTheMode() {
	int ret = 1; /* default - via cli menu */
	for (auto it = argvList.begin(); it != argvList.end(); ++it)
	{
		std::string tmp = *it;
		if(tmp.empty()) continue;

		/* interaction using cli arguments */
		if(tmp == ARG_CLI_MODE) {
			ret = 0;
			break;

		/* interaction using cli menu */
		} else if(tmp == ARG_CLI_MENU) {
			ret = 1;
			break;

		/* interaction using graphical interface */
		} else if(tmp == ARG_GUI_MENU) {
			ret = 2;
			break;
		}
	}
	return ret;
}

/* get a path to the source file
	0 - file was defined by user
	-1 - something went wrong			*/
int mainMenu::CLIgetPathSourceFile() {
	int fileNameLength = 0;
	memset(mainMenu::openFile,0,MAX_LENGTH_FILE_PATH + 1);
	for (auto it = argvList.begin(); it != argvList.end(); ++it) {
		std::string tmp = *it;
		if(!tmp.empty() && tmp == ARG_OPEN_FILE) {
			++it; /* next after that must be path to a file */
			tmp = *it;
			fileNameLength = tmp.length();
			if (!tmp.empty()) {
				memcpy(mainMenu::openFile, tmp.c_str(), fileNameLength);
				mainMenu::openFile[fileNameLength] = '\0';
				return 0;
			}
			return -1; /* path next to the argument was empty */
		}
	}
	return -1;	/* argument has been not found */
}

/* get values for nextTimeMin and nextTimeMax
	0 - at least one argument has been found and set
	1 - no arguments have been found
*/
void mainMenu::CLIdefineTimeRanges() {

	/* try to find in argyments list ARG_NEXT_TIME_MIN or ARG_NEXT_TIME_MAX */
	for (auto it = argvList.begin(); it != argvList.end(); ++it) {
		std::string tmp = *it;
		if (tmp.empty()) continue;

		if(tmp == ARG_NEXT_TIME_MIN) {
			++it; /* next after that must be a value */
			tmp = *it;
			if (!tmp.empty()) {
				mainMenu::nextTimeMin = atoi(tmp.c_str());
				if (mainMenu::nextTimeMin == 0) {
					printf("WARNING: Wrong value has been given for <%s>. Setting to default.\n",
								ARG_NEXT_TIME_MIN);
					mainMenu::nextTimeMin = NEXT_TIME_MIN;
				}
			}
		}

		if(tmp == ARG_NEXT_TIME_MAX) {
			++it; /* next after that must be a value */
			tmp = *it;
			if (!tmp.empty()) {
				mainMenu::nextTimeMax = atoi(tmp.c_str());
				if (mainMenu::nextTimeMax == 0) {
					printf("WARNING: Wrong value has been given for <%s>. Setting to default.\n",
								ARG_NEXT_TIME_MAX);
					mainMenu::nextTimeMax = NEXT_TIME_MAX;
				}
			}
		}

	}
}