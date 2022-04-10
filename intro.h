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

#define ALLOWED_AMOUNT_ARGV       10      // allowed amount of arguments
#define MAIN_MENU_GIVEN_ATTEMPTS  3
#define MAIN_MENU_CHOICE_AMOUNT   5       // possible values for the main menu
#define NEXT_TIME_MIN             5       // min amount of seconds before next appear
#define NEXT_TIME_MAX             10      // max amount of seconds before next appear
#define ALLOWED_TIME_BORDERS_MIN  1       // minimum allowed value for nextTimeMin / nextTimeMax
#define ALLOWED_TIME_BORDERS_MAX  60      // maximum allowed value for nextTimeMin / nextTimeMax
#define MAX_LENGTH_FILE_PATH      100     // maximum allowed length for a file name including a path
#define MAX_LENGTH_WORK_DIR       100     // maximum allowed length for a path to a work dir

#define DEFAULT_WORK_DIR          "/tmp/"
#define DEFAULT_WORK_DIR_LENGTH   6

/* arguments list */
#define ARG_CLI_MENU      "--cli-menu"    // interactive cli based menu
#define ARG_GUI_MENU      "--gui-menu"    // GUI menu
#define ARG_CLI_MODE      "--cli-mode"    // no menus at all, just cli arguments

#define ARG_OPEN_FILE     "--open-file"   // followed by the path to a file
#define ARG_SET_DIR       "--set-dir"     // followed by the directory path
#define ARG_NEXT_TIME_MIN "--next-time-min" // followed by min time (sec) before next appear
#define ARG_NEXT_TIME_MAX "--next-time-max" // followed by max time (sec) before next appear

class mainMenu {
private:
		const char * mainInteractionWindow;
		const char * mainHelpInformation;

		char * ch;       /* give a possibility to enter 2digit*/
		int nextTimeMin;
		int nextTimeMax;

		int argvAmount;

public:
		std::vector<std::string> argvList;
		char * openFile;
		char * workingDir;
public:
		mainMenu();

		/* interaction via cli based interactive menu */
		int launchInteractionWindow();
		int defineTimeRanges();
		int addNewSourceList();
		int setWorkingDir();
		int provideHelp();

		/* interaction via cli arguments */
		int CLIappendToArgvList(std::string & arg);
		int CLIsetSourceFile();
		void CLIsetWorkingDir();
		void CLIdefineTimeRanges();

		/* other */
		int defineTheMode();

		/* search for an argument in arguments list */
		bool inArguments(const std::vector<std::string> & array, const std::string & value) {
				return std::find(array.begin(), array.end(), value) != array.end() ? true : false;
		}

};