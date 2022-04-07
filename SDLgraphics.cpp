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

#include "SDLgraphics.h"

/* constructor */
SDLgraphics::SDLgraphics() {
		SDL_Init(SDL_INIT_EVERYTHING);/* initialize SDL
																	   SDL_INIT_EVERYTHING - means initialization of all systems */
		/* get monitor preferences, if everything ok, than 0 is returned */
		int request = SDL_GetDesktopDisplayMode(0,&displayMode); // arg1 = 0 - is general monitor
		x = 1600;
		y = 900;
		w = 200;
		h = 90;
		flags = SDL_WINDOW_SHOWN;
		//opacity = 0.5;

		buttons[0] = { 0, 0, "no" };
		buttons[1] = { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" };
		buttons[2] = { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "stop learning" };

		buttonGotcha[0] = { 0, 0, "Gotcha!"};
		buttonOk[0] = { 0, 0, "Okay"};
}

/* create a new window */
void SDLgraphics::createNewWindow(int width, int height, int window_type) {
		if (width != 0)	w = width;
		if (height != 0) h = height;
		myWindow = SDL_CreateWindow("NaN", x, y, w, h, flags);
		if (myWindow == nullptr) {
				printf("DEBUG: SDL_CreateWindow Error: <%s> \n", SDL_GetError());
		}
		/* set windows opacity - returns 0 on success or a negative error code on failure */
		//SDL_SetWindowOpacity(myWindow, opacity);

		messageboxdata.window = myWindow;
		messageboxdata.flags = SDL_MESSAGEBOX_INFORMATION;
		switch (window_type) {
			case WINDOW_TYPE_QUIZ:
				messageboxdata.numbuttons = SDL_arraysize(buttons);
				messageboxdata.buttons = buttons;
				break;
			case WINDOW_TYPE_RESULT:
				messageboxdata.numbuttons = SDL_arraysize(buttonGotcha);
				messageboxdata.buttons = buttonGotcha;
				break;
			case WINDOW_TYPE_NO_ANSWER:
				messageboxdata.numbuttons = SDL_arraysize(buttonOk);
				messageboxdata.buttons = buttonOk;
				break;
      default:
				printf("WARNING: no window type given, cannot prepare a window!\n");
		}
}

int SDLgraphics::showWindow(const char *myTitle, const char *myMessage) {
		messageboxdata.title = myTitle;
		messageboxdata.message = myMessage;
		SDL_ShowMessageBox(&messageboxdata, &buttonid);
		SDL_DestroyWindow(myWindow);
		return buttonid;
}