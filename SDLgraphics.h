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

/* the class responsible for visual part */
class SDLgraphics {
private:
		SDL_DisplayMode displayMode;
		int request;
		SDL_Window * myWindow;
		int x, y;							 // coordinates of the window
		int w, h;							 // a size of the window
		Uint32 flags;					 // window initialization flags
		//float opacity;

		int buttonid;
		SDL_MessageBoxData messageboxdata = {};
		SDL_MessageBoxButtonData buttons[3] = {};
		SDL_MessageBoxColorScheme colorScheme = {};

		SDL_MessageBoxButtonData buttonOneMessage[1] = {};

public:
		SDLgraphics();
		~SDLgraphics() {};

		/* create a new window */
		void createNewWindow(int width, int height);
		void createResultWindow(int width, int height);
		int showWindow(const char *myTitle, const char *myMessage);
		void showWindowOneButton(const char *myMessage);
};