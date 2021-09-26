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

#include <random>
#include <bits/stdc++.h>

#include <iostream>
#include <cstdio>
#include <stdio.h>

#include <algorithm>

/* own headers */
#include "dataKeeper.h"
#include "SDLgraphics.h"

#define ALLOWED_AMOUNT_ARGV			1				// alowed amount of arguments
#define ALLOWED_AMOUNT_LINES		50			// alowed amount of processable text lines
#define NEXT_TIME_MIN						5				// min amount of seconds before next appear
#define NEXT_TIME_MAX						10			// max amount of seconds before next appear
#define FOR_HOW_LONG						1800		// time in seconds the program will run
#define FOR_HOW_MAY_CYCLES			10			// cycles to be processed

/* button ids which I use to identify the pressed one */
#define NO_BUTTON								0
#define YES_BUTTON							1
#define CANCEL_BUTTON						2

/* other */
#define WORD_NOT_LEARNED				false
#define WORD_LEARNED						true
