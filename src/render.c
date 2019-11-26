#include "render.h"
#include "logic.h"
#include "display.h"

SDL_Renderer * RENDER = NULL;
TTF_Font * FONT = NULL;

SDL_Color BG = DEF_BG_COLOR;

/* sets rendering color */
void Render_SetColor(SDL_Color* C) {
	if (!C) return;
	SDL_SetRenderDrawColor(RENDER, C->r, C->g, C->b, C->a);
}

/* clears screen to BG */
void Render_Clear(void) {
	Render_SetColor(&BG);
	SDL_RenderClear(RENDER);
}

/* updates frame */
void Render_Update(void) {
	SDL_RenderPresent(RENDER);
}

/* loads and frees default font */
void Render_LoadFont(void) {
	FONT = TTF_OpenFont(DEF_FONT, 12);
}
void Render_FreeFont(void) {
	if (FONT) TTF_CloseFont(FONT);
}

/* renders aliased colored line */
void Render_Line(int x1, int y1, int x2, int y2, SDL_Color* C) {
	Render_SetColor(C);
	SDL_RenderDrawLine(RENDER, x1, y1, x2, y2);
}

/* renders colored rectangle */
void Render_Rect(int x, int y, int w, int h, SDL_Color* C) {
	Render_SetColor(C);
	SDL_Rect r = {x,y,w,h};
	SDL_RenderFillRect(RENDER, &r);
}

void Render_RectLine(int x, int y, int w, int h, SDL_Color* C) {
	Render_SetColor(C);
	SDL_Rect r = {x,y,w,h};
	SDL_RenderDrawRect(RENDER, &r);
}

void Render_Text(const char* text, int x, int y, int align, SDL_Color* c) {
	SDL_Color W = {0xff,0xff,0xff,0xff};
	if (!c) c = &W;

	SDL_Surface* s = TTF_RenderUTF8_Solid(FONT, text, *c);
	SDL_Texture* t = SDL_CreateTextureFromSurface(RENDER, s);

	int w,h;
	TTF_SizeText(FONT, text, &w, &h);

	SDL_Rect r = {x,y,w,h};

	if (align == ALIGN_RIGHT)
		r.x -= w;
	else if (align == ALIGN_MIDDLE)
		r.x -= w/2;

	SDL_RenderCopy(RENDER, t, NULL, &r);

	SDL_FreeSurface(s);
	SDL_DestroyTexture(t);
}

/* renders texture by texture name */
void Render_Texture(char * tname,
	SDL_Rect* src, SDL_Rect* dest, double rotation)
{
	texture * t = Texture_Get(tname);	
	if (!t) return;

	SDL_RenderCopyEx(RENDER, t->img,
	  src, dest, rotation, NULL, SDL_FLIP_NONE);
}

void Render_DrawTextbox(const char * text, int x, int y) {
	int w,h;

	TTF_SizeText(FONT, text, &w, &h);

	// if textbox would go off top edge of window
	if (y-h-2 < 0)
		y = h+2;
	// if textbox would go off right edge of window
	if (x+w+4 > WIN_W)
		x = WIN_W-w-4;

	SDL_Color BG = {0x00,0x00,0x00,0xff};
	SDL_Color FG = {0xff,0xff,0xff,0xff};

	Render_Rect(x, y-h-1, w+4, h+2, &BG);
	Render_Text(text, x+3, y-h-1, ALIGN_LEFT, &FG);
	Render_RectLine(x, y-h-1, w+4, h+2, &FG);
}

void Render_Component(component* c) {
	if (!c) return;

	int hovered = (c-comps) == CANVAS_NODECOMPHOVER;

	// render nodes
	SDL_Color color    = {0xff,0xff,0xff,0xff},
	          on_color = DEF_ON_COLOR;
	SDL_Color hover    = {0xff,0xff,0x00,0xff};

	int x,y,w,h;
	int i;
	for (i = 0; i < c->in_count; ++i) {
		Component_GetNodeRect(c, c->in+i, &x,&y,&w,&h);
		if (hovered && -i-1 == CANVAS_NODEHOVER) {
			SDL_Rect hover_rect = {x+w/2-8,y+h/2-8,16,16};
			Render_Rect(x,y,w,h, &hover);
			Render_Texture("img/hover.png", NULL, &hover_rect, 0.0);
		} else {
			Render_Rect(x,y,w,h, c->in[i].state ? &on_color : &color);
		}
	}

	for (i = 0; i < c->out_count; ++i) {
		Component_GetNodeRect(c, c->out+i, &x,&y,&w,&h);

		if (hovered && i == CANVAS_NODEHOVER) {
			SDL_Rect hover_rect = {x+w/2-8,y+h/2-8,16,16};
			Render_Rect(x,y,w,h, &hover);
			Render_Texture("img/hover.png", NULL, &hover_rect, 0.0);
		} else {
			Render_Rect(x,y,w,h, c->out[i].state ? &on_color : &color);
		}
	}

	// render component
	if (c->render) {
		c->render(c, RENDER, textures, textures_loaded);
		return;
	} else {
		SDL_Rect rect = {c->x, c->y, c->w, c->h};
		double rot = c->rotation * 90.0;

		if (!c->state && c->img_off)
			Render_Texture(c->img_off,NULL,&rect,rot);
		if (c->state && c->img_on)
			Render_Texture(c->img_on,NULL,&rect,rot);
	}
}

void Render_Wire(wire* w) {
	if (!w) return;

	SDL_Color C;
	if (w-wires == CANVAS_WIREHOVER) {
		C = (SDL_Color){0xff,0xff,0x00,0xff};
	} else {
		C = w->state ? ON_COLOR : OFF_COLOR;
	}

	//
	//  parity=0  |  parity=1 |  parity=2 |  parity=3
	//            |           |           |
	//      |--p2 |      p2   |       p2  |  -----p2
	//      |     |  ____|    |       |   |  |
	//      |     |  |        |       |   |  |
	//  p1--|     |  p1       |  p1---|   |  p1
	//
	if (w->parity == 0) {
		int xm = (w->x1 + w->x2 - 1)/2;

		Render_Line(w->x1, w->y1, xm   , w->y1, &C); // --
		Render_Line(xm   , w->y2, w->x2, w->y2, &C); // --

		Render_Line(xm   , w->y1, xm   , w->y2, &C); // |
	} else if (w->parity == 1) {
		int ym = (w->y1 + w->y2 - 1)/2;

		Render_Line(w->x1, w->y1, w->x1, ym   , &C); // |
		Render_Line(w->x2, ym   , w->x2, w->y2, &C); // |

		Render_Line(w->x1, ym   , w->x2, ym   , &C); // --
	} else if (w->parity == 2) {
		Render_Line(w->x1, w->y1, w->x2, w->y1, &C); // --
		Render_Line(w->x2, w->y1, w->x2, w->y2, &C); // |
	} else  {
		Render_Line(w->x1, w->y1, w->x1, w->y2, &C); // |
		Render_Line(w->x1, w->y2, w->x2, w->y2, &C); // --
	}
}
