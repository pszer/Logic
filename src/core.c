#include "core.h"

SDL_Window   * WINDOW = NULL;
SDL_Event      EVENT;

char QUIT_FLAG = 0;

/* Core_Init
 * initialises SDL2, window and rendering contex
 * -- returns 0 for no error
 * -- returns 1 for error
 */

#define ERR() {printf("SDL: %s\n",SDL_GetError());return 1;}
int Core_Init(int argc, char ** argv) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ERR();

	if (TTF_Init() == -1) {
		printf("IMG: %s\n", TTF_GetError());
		return 1;
	}

	// init SDL2 img
	int flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if ((IMG_Init(flags)&flags) != flags) {
		printf("IMG: %s\n", IMG_GetError());
		return 1;
	}

	// window creation
	WIN_W = DEF_WIN_W;
	WIN_H = DEF_WIN_H;
	WINDOW = SDL_CreateWindow("logic",
	  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	  WIN_W, WIN_H, SDL_WINDOW_RESIZABLE);
	if (!WINDOW)
		ERR();

	// create hardware accelerated rendering context
	RENDER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
	if (!RENDER) {
		printf("Falling back to software rendering\n");
		RENDER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_SOFTWARE);
		if (!RENDER) ERR();
	}

	int count = Texture_LoadFolder(RENDER, "img");
	if (count == -1)
		return 1;
	else
		printf("Loaded %i textures\n", count);

	Render_LoadFont();

	pthread_t t;
	// make in seperate thread, otherwise launch in same
	// thread
	__dfndone = 0;
	if (pthread_create(&t, NULL, Logic_DefineComps, NULL)) {
		Logic_DefineComps(NULL);
		Render_Clear();
		Render_Text("Compiling...",WIN_W/2,WIN_H/2,ALIGN_MIDDLE, NULL);
		Render_Update();
	} else {
		while (!__dfndone) {
			int w,h;
			while (SDL_PollEvent(&EVENT));
			SDL_GetWindowSize(WINDOW, &w, &h);
			Render_Clear();
			//Render_Text("Compiling...",w/2,h/2,ALIGN_MIDDLE, NULL);
			Render_Text(__dfnstr,w/2,h/2,ALIGN_MIDDLE, NULL);
			Render_Update();
			SDL_Delay(15);
		}
		pthread_join(t, NULL);
	}

	Logic_AllocateComponents(START_COMP_SIZE);
	Logic_AllocateWires(START_WIRE_SIZE);

	return 0;
}
#undef ERR

/* Core_Quit
 * quits sdl and frees memory
 */

#define FREE(f,x) if(x)f(x);
void Core_Quit() {
	Logic_FreeDefines();
	Logic_FreeComponents();
	Logic_FreeWires();

	Render_FreeFont();

	Texture_FreeAll();

	FREE(SDL_DestroyRenderer, RENDER);
	FREE(SDL_DestroyWindow, WINDOW);

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
#undef FREE

/* Core_Main
 * main loop
 * loops until QUIT_FLAG is set
 */
void Core_Main() {
	while (!QUIT_FLAG) {
		Event_Update();
		Core_HandleEvents();
		Display_Update();

		Logic_Update();
		Core_Render();

		SDL_Delay(15);
	}
}

/* Core_Render
 * render graphics call
 */
void Core_Render() {
	Render_Clear();

	Display_RenderGrid();

	int i;
	for (i = 0; i < wire_count; ++i)
		Render_Wire(wires + i);
	for (i = 0; i < comp_count; ++i)
		Render_Component(comps + i);

	Display_Render();
	
	Render_Update();
}

/* Core_HandleEvents
 * handles all events in the queue
 * called by Core_Main every frame
 */
void Core_HandleEvents() {
	while (SDL_PollEvent(&EVENT)) {
		switch (EVENT.type) {
		case SDL_QUIT:
			QUIT_FLAG = 1;
			return;

		case SDL_WINDOWEVENT:
			Event_HandleWindow();
			break;
		case SDL_MOUSEMOTION:
			Event_HandleMouse();
			break;
		case SDL_MOUSEBUTTONDOWN:
			Event_HandleButtonDown();
			break;
		case SDL_MOUSEBUTTONUP:
			Event_HandleButtonUp();
			break;
		case SDL_KEYDOWN:
			Event_HandleKeyDown();
			break;
		case SDL_TEXTINPUT:
			Event_HandleTextInput();
			break;
		}
	}
}
