#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "tex.h"
#include "comp.h"

#define DEF_BG_COLOR { 0x1f,0x1f,0x1f,0xff }

#define DEF_ON_COLOR { 0x8e,0xe8,0x0c,0xff }

#define DEF_FONT "alphbeta.ttf" 

extern SDL_Renderer * RENDER;
extern TTF_Font * FONT;

extern SDL_Color BG;

/* loads and frees default font */
void Render_LoadFont();
void Render_FreeFont();

/* sets rendering color */
void Render_SetColor(SDL_Color* C);

/* clears screen to BG */
void Render_Clear();

/* updates frame */
void Render_Update();

/* renders aliased colored line */
void Render_Line(int x1, int y1, int x2, int y2, SDL_Color* C);

/* renders colored rectangle */
void Render_Rect(int x, int y, int w, int h, SDL_Color* C);
void Render_RectLine(int x, int y, int w, int h, SDL_Color* C);

enum {ALIGN_LEFT, ALIGN_MIDDLE, ALIGN_RIGHT};

/* render text */
void Render_Text(const char* text, int x, int y, int align, SDL_Color* c);

/* renders texture by texture name */
void Render_Texture(char * tname,
	SDL_Rect* src, SDL_Rect* dest, double rotation);

void Render_Component(component* c);
void Render_Wire(wire* w);
