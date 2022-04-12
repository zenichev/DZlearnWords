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
mainMenu::mainMenu() : nextTimeMin(NEXT_TIME_MIN), nextTimeMax(NEXT_TIME_MAX), argvAmount(0) {
	ch = (char*) malloc( sizeof(char) * ( 2 + 1 ) ); /* give a possibility to enter 2digit*/

	argvList = std::vector<std::string>( ALLOWED_AMOUNT_ARGV );
	openFile = (char*) malloc( sizeof(char) * ( MAX_LENGTH_FILE_PATH + 1 ) );
	workingDir = (char*) malloc( sizeof(char) * ( MAX_LENGTH_FILE_PATH + 1 ) );

	mainInteractionWindow =
	"Main menu:\n"
	"0. Press to start ;\n"
	"1. Press for definition of time ranges ;\n"
	"2. Press for adding a new source file (words list) ;\n"
	"3. Press for setting a working directory (default /tmp);\n"
	"4. Press for help ;\n"
	"5. Press to quit ;\nEnter choice: ";

	mainHelpInformation =
	"Helpful information:\n"
	"\t- Possible modes\n"
	"\t\tModes - define a way a human can interact with the program.\n"
	"\t\tThere are two main modes available and one in a stage of development:\n"
	"\t\t--cli-menu - interactive cli based menu ;\n"
	"\t\t--cli-mode - no cli menu at all, just cli arguments can be used ;\n"
	"\t\t--gui-menu - graphical iface for launching the program (not available now) ;\n\n"
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
	"\t- The working directory\n"
	"\t\tIt is recommended that you set a working directory, otherwise a default: /tmp\n"
	"\t\tdirectory will be chosen. This is a directory where temporary/working data\n"
	"\t\tfor the program will be stored. If the program will have no permission to that directory\n"
	"\t\tsome of the functionality will be unavailable for you.\n\n"
	"\t- Options to be used in the --cli-mode\n"
	"\t\t'--help'          - a help information.\n"
	"\t\t'--open-file'     - a source file to open.\n"
	"\t\t'--set-dir'       - set a working directory for the program.\n"
	"\t\t'--next-time-min' - the minimum time step for randomity.\n"
	"\t\t'--next-time-max' - the maximum time step for randomity.\n";
};

/** general interaction window with possible options
 *	-1 - terminating the programm
 *	 0 - start flipping the cards
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
		printf("%s", mainInteractionWindow);

		takenAttempts = 0;

		memset(ch,0,sizeof(ch));
		while (takenAttempts < MAIN_MENU_GIVEN_ATTEMPTS)
		{
			if ( (temp=scanf("%s",ch)) <= 0 ) {
				printf("WARNING: Impossible choice, are you giving a char? Try again.\n");
				goto AGAIN;
			}

			/* convert into long int */
			choice=std::strtol(ch, &end, base);
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
			if (takenAttempts < MAIN_MENU_GIVEN_ATTEMPTS) printf("%s", mainInteractionWindow);
		}

		/* general menu */
		switch(choice) {
			/* start flipping the cards */
			case 0:
				/* make sure working directory is set, otherwise set to the default one */
				if (strlen(workingDir) == 0) {
					memcpy(workingDir, DEFAULT_WORK_DIR, DEFAULT_WORK_DIR_LENGTH);
					workingDir[DEFAULT_WORK_DIR_LENGTH] = '\0';
					printf("NOTICE: Working directory is set to default: %s \n", DEFAULT_WORK_DIR);
				}
				return 0;
			/* for definition of time ranges */
			case 1:
				if ((ret = defineTimeRanges()) != 0) {
					printf("WARNING: Something went wrong. Terminating the program.\n");
					return -1;
				}
				break;
			/* for adding a new source file (words list) */
			case 2:
				if ((ret = addNewSourceList()) != 0) {
					printf("WARNING: Something went wrong. Terminating the program.\n");
					return -1;
				}
				break;
			/* set a working directory */
			case 3:
				if ((ret = setWorkingDir()) != 0) {
					printf("WARNING: Something went wrong. Terminating the program.\n");
					return -1;
				}

				/* add a slash at the end if not added by a user */
				if (workingDir[strlen(workingDir) - 1] != '/') {
					unsigned int dirLength = strlen(workingDir) + 1; /* we add one more symbol */
					std::string tmpWorkDir = (std::string)workingDir + "/";

					memset(workingDir,0,MAX_LENGTH_WORK_DIR + 1);
					memcpy(workingDir, tmpWorkDir.c_str(), dirLength);
					workingDir[dirLength] = '\0';
				}
				break;
			/* for help */
			case 4:
				if ((ret = provideHelp()) != 0) {
					printf("WARNING: Something went wrong. Terminating the program.\n");
					return -1;
				}
				break;
			/* to quit */
			case 5:
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

		if ((temp=scanf("%d",&(nextTimeMin))) <= 0) {
					printf("\tWARNING: Impossible to set this value, setting to default.\n\n");
					nextTimeMin = ALLOWED_TIME_BORDERS_MIN;
		}
		if (nextTimeMin < ALLOWED_TIME_BORDERS_MIN ||
				nextTimeMin > ALLOWED_TIME_BORDERS_MAX) {
			printf("\tWARNING: The given value is outside of the borders, setting to default.\n\n");
			nextTimeMin = ALLOWED_TIME_BORDERS_MIN;
		}

		printf("\tMaximum time step for randomity (without quotes): ");

		if ((temp=scanf("%d",&(nextTimeMax))) <= 0) {
					printf("\tWARNING: Impossible to set this value, setting to default.\n\n");
					nextTimeMin = ALLOWED_TIME_BORDERS_MAX;
		}
		if (nextTimeMax < ALLOWED_TIME_BORDERS_MIN ||
				nextTimeMax > ALLOWED_TIME_BORDERS_MAX) {
			printf("\tWARNING: The given value is outside of the borders, setting to default.\n\n");
			nextTimeMax = ALLOWED_TIME_BORDERS_MAX;
		}

		return 0;
}

/* for adding a new source file (words list) */
int mainMenu::addNewSourceList() {
		int temp = 0;
		int takenAttempts = 0;

		memset(openFile,0,MAX_LENGTH_FILE_PATH + 1);

		std::system("clear");
		printf("Define a source file to read (without quotes): ");

		while ((temp=scanf("%s",openFile)) <= 0 &&
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

/* set a working directory for the program */
int mainMenu::setWorkingDir() {
		int temp = 0;
		int takenAttempts = 0;

		memset(workingDir,0,MAX_LENGTH_WORK_DIR + 1);

		std::system("clear");
		printf("Define a working directory for the program: ");

		while ((temp=scanf("%s",workingDir)) <= 0 &&
			takenAttempts < MAIN_MENU_GIVEN_ATTEMPTS) {
				printf("\tWARNING: Impossible to set this value, try again.\n");
				printf("\tDefine a working directory for the program: ");
				takenAttempts++;
		}

		if (takenAttempts >= MAIN_MENU_GIVEN_ATTEMPTS) {
				printf("DEBUG: Sorry, max attempts have been reached. Terminating the program.\n");
				return -1;
		}

		return 0;
}

/* basic help the terminal based menu */
int mainMenu::provideHelp() {
		std::system("clear");

		const char * additional = "\nPress any key to continue.. \n";

		/* a way to implement 'press any key' using ncurses, not the best one */
		initscr();
		addstr( mainHelpInformation );
		addstr( additional );
		cbreak();    /* turn off line-buffering */
		noecho();    /* turn off character echo */
		getch();     /* read and discard a single character (caveats!) */
		echo();      /* turn echo back on */
		nocbreak();  /* turn line-buffering back on */
		endwin();

		return 0;
}

/* basic help for CLI */
void mainMenu::CLIprovideHelp() {
	printf("%s", mainHelpInformation);
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

/** using given arguments define the interaction mode
 *	0 - interaction only via cli arguments
 *	1 - interaction via usual cli menu
 *	2 - interaction via gui menu
 */
int mainMenu::defineTheMode() {
	int ret = 1; /* default - via cli menu */
	bool modeDefined = false;

	for (auto it = argvList.begin(); it != argvList.end(); ++it)
	{
		std::string tmp = *it;
		if(tmp.empty()) continue;

		/* interaction using cli arguments */
		if(tmp == ARG_CLI_MODE) {
			ret = 0;
			modeDefined = true;
			break;

		/* interaction using cli menu */
		} else if(tmp == ARG_CLI_MENU) {
			ret = 1;
			modeDefined = true;
			break;

		/* interaction using graphical interface */
		} else if(tmp == ARG_GUI_MENU) {
			ret = 2;
			modeDefined = true;
			break;
		}
	}

	/* no mode defined and arguments present, then act as the cli mode */
	if (!modeDefined && !argvListEmpty()) ret = 0;

	return ret;
}

/* check whether argv list is empty */
bool mainMenu::argvListEmpty() {
	for (auto it = argvList.begin(); it != argvList.end(); ++it)
	{
		std::string tmp = *it;
		if (!tmp.empty()) return false;
	}
	return true;
}

/** get a path to the source file
 *  0 - file was defined by user
 * -1 - something went wrong
 */
int mainMenu::CLIsetSourceFile() {
	int fileNameLength = 0;
	memset(openFile,0,MAX_LENGTH_FILE_PATH + 1);

	for (auto it = argvList.begin(); it != argvList.end(); ++it) {
		std::string tmp = *it;
		if(!tmp.empty() && tmp == ARG_OPEN_FILE) {
			++it; /* next after that must be path to a file */
			tmp = *it;
			fileNameLength = tmp.length();

			/* prevent cases when the next to a path argument is not
			 * a value, but right away another cli argument, or the value is empty */
			if (tmp.empty() || tmp.find("--") != std::string::npos) {
				printf("ERROR: %s argument given, but no value!\n", ARG_OPEN_FILE);
				printf("ERROR: Make sure to use it in a syntax: %s /path/file.txt\n", ARG_OPEN_FILE);
				return -1; /* value next to the argument was another argument */
			}

			memcpy(openFile, tmp.c_str(), fileNameLength);
			openFile[fileNameLength] = '\0';
			return 0;
		}
	}
	return -1;	/* argument has been not found */
}

/* set a working directory path */
void mainMenu::CLIsetWorkingDir() {
	std::string tmpWorkDir;
	int dirLength = 0;
	bool found = false;

	memset(workingDir,0,MAX_LENGTH_WORK_DIR + 1);

	for (auto it = argvList.begin(); it != argvList.end(); ++it) {
		std::string argument = *it;

		if(!argument.empty() && argument == ARG_SET_DIR) {
			++it; /* next after that must be a value of directory */
			argument = *it;
			dirLength = argument.length();
			found = true;

			/* value next to the argument was another argument or was just empty */
			if (argument.empty() || argument.find("--") != std::string::npos) {
					printf("WARNING: %s argument given, but no value!\n", ARG_SET_DIR);
					printf("WARNING: Make sure to use it in a syntax: %s /directory/to/use/\n", ARG_SET_DIR);
					printf("WARNING: The value has been set to the default one: %s\n", DEFAULT_WORK_DIR);

					tmpWorkDir = DEFAULT_WORK_DIR;
					dirLength = DEFAULT_WORK_DIR_LENGTH;
					break;
			}

			tmpWorkDir = argument;
			break;
		}
	}

	/* argument has been not found */
	if (!found) {
		printf("WARNING: argument %s has been not found!\n", ARG_SET_DIR);
		printf("WARNING: The value has been set to the default one: %s\n", DEFAULT_WORK_DIR);
		tmpWorkDir = DEFAULT_WORK_DIR;
		dirLength = DEFAULT_WORK_DIR_LENGTH;
	}

	/* add a slash at the end if not added by a user */
	if (tmpWorkDir.back() != '/') {
		tmpWorkDir = tmpWorkDir + "/";
		dirLength++;
	}

	memcpy(workingDir, tmpWorkDir.c_str(), dirLength);
	workingDir[dirLength] = '\0';
}

/** get values for nextTimeMin and nextTimeMax
 * 0 - at least one argument has been found and set
 * 1 - no arguments have been found
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
				nextTimeMin = atoi(tmp.c_str());
				if (nextTimeMin == 0) {
					printf("WARNING: Wrong value has been given for <%s>. Setting to default.\n",
								ARG_NEXT_TIME_MIN);
					nextTimeMin = NEXT_TIME_MIN;
				}
			}
		}

		if(tmp == ARG_NEXT_TIME_MAX) {
			++it; /* next after that must be a value */
			tmp = *it;
			if (!tmp.empty()) {
				nextTimeMax = atoi(tmp.c_str());
				if (nextTimeMax == 0) {
					printf("WARNING: Wrong value has been given for <%s>. Setting to default.\n",
								ARG_NEXT_TIME_MAX);
					nextTimeMax = NEXT_TIME_MAX;
				}
			}
		}

	}
}