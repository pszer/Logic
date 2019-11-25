#include "event.h"
#include "display.h"

SDL_Event EVENT;

int MOUSEX=0, MOUSEY=0, MOUSEDX=0, MOUSEDY=0;

char MOUSE1 = MOUSE_NULL, MOUSE2 = MOUSE_NULL;

int M1S,M2S; // start tick for mouse button down
int MOUSE1DUR=0, MOUSE2DUR=0;

char BACKSPACE=0, ENTER=0;

int WIN_W, WIN_H; // set at core init and win event

/* Event_Update
 * updates some event flags
 */
void Event_Update() {
	if (MOUSE1 == MOUSE_NULL) {
		MOUSE1DUR = 0;
	} else {
		MOUSE1DUR = SDL_GetTicks() - M1S;
		if (MOUSE1 == MOUSE_DOWN)    MOUSE1 = MOUSE_HELD;
		else if (MOUSE1 == MOUSE_UP) MOUSE1 = MOUSE_NULL;
	}

	if (MOUSE2 == MOUSE_NULL) {
		MOUSE2DUR = 0;
	} else {
		MOUSE2DUR = SDL_GetTicks() - M2S;
		if (MOUSE2 == MOUSE_DOWN)    MOUSE2 = MOUSE_HELD;
		else if (MOUSE2 == MOUSE_UP) MOUSE2 = MOUSE_NULL;
	}

	if (BACKSPACE) BACKSPACE = 0;
	if (ENTER) ENTER = 0;
}

// event update handlers
void Event_HandleWindow() {
	switch (EVENT.window.event) {
	case SDL_WINDOWEVENT_RESIZED:
		WIN_W = EVENT.window.data1;
		WIN_H = EVENT.window.data2;
		break;
	}
}

void Event_HandleMouse() {
	SDL_GetMouseState(&MOUSEX, &MOUSEY);
	SDL_GetRelativeMouseState(&MOUSEDX, &MOUSEDY);
}

void Event_HandleButtonDown() {
	if (EVENT.button.button == SDL_BUTTON_LEFT) {
		MOUSE1 = MOUSE_DOWN;
		M1S = SDL_GetTicks();
	} else if (EVENT.button.button == SDL_BUTTON_RIGHT) {
		MOUSE2 = MOUSE_DOWN;
		M2S = SDL_GetTicks();
	}
}

void Event_HandleButtonUp() {
	if (EVENT.button.button == SDL_BUTTON_LEFT)
		MOUSE1 = MOUSE_UP;
	else if (EVENT.button.button == SDL_BUTTON_RIGHT)
		MOUSE2 = MOUSE_UP;
}

void Event_HandleKeyDown() {
	switch (EVENT.key.keysym.sym) {
	case SDLK_BACKSPACE:
		BACKSPACE = 1;
		break;
	case SDLK_RETURN:
		ENTER = 1;
		break;
	}
}

void Event_HandleTextInput() {
	Display_TypePopup(EVENT.text.text);
}
