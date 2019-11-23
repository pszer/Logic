#pragma once

#include <SDL2/SDL.h>

extern SDL_Event EVENT;

extern int MOUSEX, MOUSEY, MOUSEDX, MOUSEDY; // mouse xy pos and change

// mouse button state (1=left, 2=right)
enum { MOUSE_NULL , MOUSE_DOWN , MOUSE_HELD , MOUSE_UP };
extern char MOUSE1, MOUSE2;

// if mouse button is held, these are how long
// it has been held (in milliseconds)
extern int MOUSE1DUR, MOUSE2DUR;

extern int WIN_W, WIN_H; // set at core init and win event

/* Event_Update
 * updates some event flags
 */
void Event_Update();

// event update handlers
void Event_HandleWindow();
void Event_HandleMouse();
void Event_HandleButtonDown();
void Event_HandleButtonUp();
