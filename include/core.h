#pragma once

#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "event.h"
#include "render.h"
#include "logic.h"
#include "display.h"

#define DEF_WIN_W 800
#define DEF_WIN_H 600

extern SDL_Window   * WINDOW;

extern char QUIT_FLAG; // =1 to quit

/* Core_Init
 * initialises sdl, window and rendering contex
 * -- returns 0 for no error
 * -- returns 1 for error
 */
int Core_Init(int argc, char ** argv);

/* Core_Quit
 * quits sdl and frees memory
 */
void Core_Quit(void);

/* Core_Main
 * main loop
 * loops until QUIT_FLAG is set
 */
void Core_Main(void);

/* Core_Render
 * render graphics call
 */
void Core_Render(void);

/* Core_HandleEvents
 * polls all events in the queue
 * calls handle functions defined in event.h
 */
void Core_HandleEvents(void);
