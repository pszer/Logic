#include "display.h"

int TOOLBAR_SCROLL = 0;
int TOOLBAR_LHOVER = 0, TOOLBAR_RHOVER = 0;
int TOOLBAR_COMPHOVER=-1;
 
int CANVAS_COMPMOVE=-1;
int C_CMOVE_OX = 0, C_CMOVE_OY = 0;

// flag set to 1 if drawing a wire
int  CANVAS_WIREFLAG = 0;
wire CANVAS_WIREMAKE = {0,0,0,0,0,NULL,NULL,0,0,0,0};

int CANVAS_COMPADD=-1;
int CANVAS_COMPROT=0;

int CANVAS_WIREHOVER = -1;

SDL_Rect TITLE_SAVE   = { 59,0,12,12 };
SDL_Rect TITLE_LOAD   = { 46,0,12,12 };
SDL_Rect TITLE_DELETE = { 26,0,12,12 };
SDL_Rect TITLE_GRID   = { 13,0,12,12 };
SDL_Rect TITLE_PAUSE  = { 0, 0,12,12 };

int GRID_FLAG = 1;

char POPUP_BUFFER[POPUP_BUFFER_SIZE+1] = "";
int  POPUP_SAVING  = 0;
int  POPUP_LOADING = 0;

SDL_Rect POPUP_RECT = {0,0,400,100};
SDL_Rect TYPING_RECT = {20,42,360,16};

char NOTIF_BUFFER[NOTIF_TEXTBOX_SIZE+1] = "";
int NOTIF_TEXTBOX = 0;
 
int RectPointCollision(int x, int y, int rx, int ry, int rw, int rh) {
	int a,b;
	if (rw >= 0) {
		a = (x >= rx) && (x <= rx + rw);
	} else {
		a = (x <= rx) && (x >= rx + rw);
	}

	if (rh >= 0) {
		b = (y >= ry) && (y <= ry + rh);
	} else {
		b = (y <= ry) && (y >= ry + rh);
	}

	return a&&b;
}

int RectMouseCollision(int rx, int ry, int rw, int rh) {
	return RectPointCollision(MOUSEX, MOUSEY, rx, ry, rw, rh);
}

int WirePointCollision(int x, int y, wire * w) {
	//
	//  parity=0  |  parity=1 |  parity=2 |  parity=3
	//            |           |           |
	//      |--p2 |      p2   |       p2  |  -----p2
	//      |     |  ____|    |       |   |  |
	//      |     |  |        |       |   |  |
	//  p1--|     |  p1       |  p1---|   |  p1
	//
	//
	if (w->parity == 0) {
		int xm = (w->x1 + w->x2)/2;

		if (RectPointCollision(x,y, w->x1, w->y1-2, (xm-w->x1), 5)) return 1;
		if (RectPointCollision(x,y, xm   , w->y2-2, (xm-w->x1), 5)) return 1;

		if (RectPointCollision(x,y, xm-2 , w->y1, 5, w->y2-w->y1)) return 1;
	} else if (w->parity == 1) {	
		int ym = (w->y1 + w->y2)/2;

		if (RectPointCollision(x,y, w->x1-2, w->y1, 5, ym-w->y1)) return 1;
		if (RectPointCollision(x,y, w->x2-2,    ym, 5, ym-w->y1)) return 1;

		if (RectPointCollision(x,y, w->x1, ym-2, w->x2-w->x1, 5)) return 1;
	} else if (w->parity == 2) {
		int xw = w->x2-w->x1;
		int yw = w->y2-w->y1;

		if (RectPointCollision(x,y, w->x1, w->y1-2, xw, 5)) return 1;
		if (RectPointCollision(x,y, w->x2-2, w->y1, 5, yw)) return 1;
	} else {
		int xw = w->x2-w->x1;
		int yw = w->y2-w->y1;

		if (RectPointCollision(x,y, w->x1-2, w->y1, 5, yw)) return 1;
		if (RectPointCollision(x,y, w->x1, w->y2-2, xw, 5)) return 1;
	}

	return 0;
}

int WireMouseCollision(wire * w) {
	return WirePointCollision(MOUSEX, MOUSEY, w);
}

void Display_Render() {
	Display_RenderToolbox();
	Display_RenderTitle();

	if (CANVAS_WIREFLAG) {
		Render_Wire(&CANVAS_WIREMAKE);
	}

	if (CANVAS_COMPADD != -1) {
		component * c = COMP_DEFS[CANVAS_COMPADD];

		SDL_Rect r = {MOUSEX-c->w/2, MOUSEY-c->h/2,
		  c->w, c->h};

		Render_Texture(c->img_off, NULL, &r, CANVAS_COMPROT*90.0);
	}

	if (CANVAS_WIREHOVER != -1) {
		wire * w = wires + CANVAS_WIREHOVER;

		SDL_Rect r;
		if (w->parity < 2) {
			int x = (w->x1+w->x2) / 2;
			int y = (w->y1+w->y2) / 2;
			r = (SDL_Rect){x-8,y-8,16,16};
		} else if (w->parity == 2) {
			r = (SDL_Rect){w->x2-8,w->y1-8,16,16};
		} else {
			r = (SDL_Rect){w->x1-8,w->y2-8,16,16};
		}
		Render_Texture("img/hover.png", NULL, &r, 0.0);
	}

	if (TOOLBAR_COMPHOVER != -1) {
		Display_DrawTextbox(COMP_DEFS[TOOLBAR_COMPHOVER]->name,
		  MOUSEX, MOUSEY);
	}

	if (POPUP_SAVING)
		Display_RenderSavePopup();
	if (POPUP_LOADING)
		Display_RenderLoadPopup();

	if (NOTIF_TEXTBOX) {
		Display_DrawTextbox(NOTIF_BUFFER, MOUSEX, MOUSEY);
	}
}

void Display_Update() {
	TOOLBAR_LHOVER=0;
	TOOLBAR_RHOVER=0;
	TOOLBAR_COMPHOVER=-1;

	CANVAS_WIREHOVER=-1;

	if (NOTIF_TEXTBOX) --NOTIF_TEXTBOX;

	if (POPUP_SAVING || POPUP_LOADING) {
		if (!RectMouseCollision(POPUP_RECT.x, POPUP_RECT.y,
		       POPUP_RECT.w, POPUP_RECT.h) && MOUSE1==MOUSE_DOWN)
		{
			Display_QuitPopup();
			return;
		}

		if (BACKSPACE)
			Display_BackspacePopup();
		if (ENTER)
			Display_EnterPopup();

		return;
	}

	Display_InputCheckCanvas();
	Display_InputCheckScrollButtons();
	Display_InputCheckToolbar();
	Display_InputCheckTitleBar();

	if (MOUSE1 == MOUSE_UP) {
		CANVAS_COMPMOVE = -1;

		if (CANVAS_WIREFLAG)
			__Display_FinishWireMake();

		if (CANVAS_COMPADD != -1) {
			__Display_FinishCompAdd();
		}
	} else if (MOUSE1 == MOUSE_DOWN && CANVAS_COMPMOVE == -1) {
		if (CANVAS_WIREHOVER != -1) {
			wire * w = wires + CANVAS_WIREHOVER;
			w->parity = (w->parity+1) % 4;
		}
	}

	if (MOUSE2 == MOUSE_DOWN) {
		Display_RightClick();
	}

	if (CANVAS_COMPMOVE != -1) {
		comps[CANVAS_COMPMOVE].x = MOUSEX - C_CMOVE_OX;
		comps[CANVAS_COMPMOVE].y = MOUSEY - C_CMOVE_OY;
		Logic_UpdateAllWirePos();
	}

	if (CANVAS_WIREFLAG) {
		CANVAS_WIREMAKE.x2 = MOUSEX;
		CANVAS_WIREMAKE.y2 = MOUSEY;
	}
}

void Display_RightClick() {
	if (CANVAS_COMPADD != -1) {
		CANVAS_COMPROT++;
		CANVAS_COMPROT %= 4;
		return;
	}

	if (CANVAS_COMPMOVE != -1) {
		component * c = comps + CANVAS_COMPMOVE;
		c->rotation++;
		c->rotation %= 4;
		return;
	}

	if (CANVAS_WIREFLAG) {
		CANVAS_WIREMAKE.parity = (CANVAS_WIREMAKE.parity+1)%4;
	}

	if (CANVAS_WIREHOVER != -1) {
		Logic_DeleteWire(CANVAS_WIREHOVER);
		return;
	}

	int i;
	for (i = 0; i < comp_count; ++i) {
		component * c = comps+i;
		if (RectMouseCollision(c->x,c->y,c->w,c->h)) {
			Logic_DeleteComponent(i);
			return;
		}
	}
}

int Display_InputCheckTitleBar() {
	if (MOUSEY > TITLE_HEIGHT) return 0;
	if (MOUSE1 != MOUSE_DOWN) return 0;

	#define UNRECT(r) r.x, r.y, r.w, r.h

	if (RectMouseCollision( UNRECT(TITLE_PAUSE) )) {
		LOGIC_PAUSE = !LOGIC_PAUSE;
	} else if (RectMouseCollision( UNRECT(TITLE_GRID) )) {
		GRID_FLAG = !GRID_FLAG;
	} else if (RectMouseCollision( UNRECT(TITLE_DELETE) )) {
		Logic_DeleteAll();
	} else if (RectMouseCollision( UNRECT(TITLE_SAVE ))) {
		Display_QuitPopup();
		POPUP_SAVING = 1;
		SDL_StartTextInput();
	} else if (RectMouseCollision( UNRECT(TITLE_LOAD ))) {
		Display_QuitPopup();
		POPUP_LOADING = 1;
		SDL_StartTextInput();
	}

	#undef UNRECT

	return 1;
}

int Display_InputCheckScrollButtons() {
	if (RectMouseCollision(0,TITLE_HEIGHT,TOOLBAR_SCROLL_W,TOOLBAR_HEIGHT)) {
		TOOLBAR_LHOVER=1;

		if (MOUSE1 != MOUSE_DOWN) return 0;

		--TOOLBAR_SCROLL;
		if (TOOLBAR_SCROLL<0)
			TOOLBAR_SCROLL=0;
	} else if (RectMouseCollision(WIN_W-TOOLBAR_SCROLL_W,TITLE_HEIGHT,
	                              TOOLBAR_SCROLL_W,TOOLBAR_HEIGHT))
	{
		TOOLBAR_RHOVER=1;

		if (MOUSE1 != MOUSE_DOWN) return 0;

		++TOOLBAR_SCROLL;
		if (TOOLBAR_SCROLL>=COMP_DEF_COUNT-1)
			TOOLBAR_SCROLL=COMP_DEF_COUNT-1;
	} else return 0;

	return 1;
}

int Display_InputCheckToolbar() {
	if (MOUSEY < TITLE_HEIGHT || MOUSEY > TITLE_HEIGHT + TOOLBAR_HEIGHT)
		return 0;

	if (MOUSEX < TOOLBAR_SCROLL_W || MOUSEX > WIN_W - TOOLBAR_SCROLL_W)
		return 0;

	int n = (MOUSEX - TOOLBAR_SCROLL_W) / (TOOLBAR_HEIGHT-2) + TOOLBAR_SCROLL;
	TOOLBAR_COMPHOVER = n;

	if (n >= 0 && n < COMP_DEF_COUNT) {
		if (MOUSE1 == MOUSE_DOWN) {
			CANVAS_COMPADD = n;
			CANVAS_COMPROT = 0;
		}
	} else {
		TOOLBAR_COMPHOVER = -1;
	}

	return 1;
}

int Display_InputCheckCanvas() {
	if (MOUSEY < TITLE_HEIGHT + TOOLBAR_HEIGHT) return 0;

	int i;
	for (i = 0; i < comp_count; ++i) {
		component * comp = comps + i;

		if (__Display_InputCheckNode(comp)) return 1;

		if (RectMouseCollision(comp->x, comp->y, comp->w, comp->h)) {
			if (comp->click && MOUSE1 != MOUSE_NULL) {
				comp->click(comp, MOUSE1-2, MOUSE1DUR);
			}

			if (MOUSE1 == MOUSE_DOWN) {
				CANVAS_COMPMOVE = i;
				C_CMOVE_OX = MOUSEX - comp->x;
				C_CMOVE_OY = MOUSEY - comp->y;
				return 1;
			} else if (MOUSE2 == MOUSE_DOWN) {
				if (CANVAS_WIREFLAG) continue;
			}
		}
	}

	for (i = 0; i < wire_count; ++i) {
		if (WireMouseCollision(wires + i)) {
			CANVAS_WIREHOVER = i;
			break;
		}
	}

	return 1;
}

int __Display_InputCheckNode(component* c) {
	if (MOUSE1 != MOUSE_DOWN) return 0;

	int i;
	for (i = 0; i < c->in_count; ++i) {
		int x,y,w,h;
		Component_GetNodeRect(c, c->in+i, &x,&y,&w,&h);
		if (RectMouseCollision(x,y,w,h)) {
			CANVAS_WIREFLAG = 1;
			CANVAS_WIREMAKE.c1 = c;
			CANVAS_WIREMAKE.n1 = -i-1;
			Component_GetNodePos(c, c->in+i,
			  &CANVAS_WIREMAKE.x1, &CANVAS_WIREMAKE.y1);
			CANVAS_WIREMAKE.x2 = CANVAS_WIREMAKE.x1;
			CANVAS_WIREMAKE.y2 = CANVAS_WIREMAKE.y1;
			return 1;
		}
	}

	for (i = 0; i < c->out_count; ++i) {
		int x,y,w,h;
		Component_GetNodeRect(c, c->out+i, &x,&y,&w,&h);
		if (RectMouseCollision(x,y,w,h)) {
			CANVAS_WIREFLAG = 1;
			CANVAS_WIREMAKE.c1 = c;
			CANVAS_WIREMAKE.n1 = i;
			Component_GetNodePos(c, c->out+i,
			  &CANVAS_WIREMAKE.x1, &CANVAS_WIREMAKE.y1);
			CANVAS_WIREMAKE.x2 = CANVAS_WIREMAKE.x1;
			CANVAS_WIREMAKE.y2 = CANVAS_WIREMAKE.y1;
			return 1;
		}
	}

	return 0;
}

void __Display_AddWire(component* c, int n) {
	CANVAS_WIREMAKE.c2 = c;
	CANVAS_WIREMAKE.n2 = n;

	Logic_AddWire(CANVAS_WIREMAKE.c1,CANVAS_WIREMAKE.c2,
	              CANVAS_WIREMAKE.n1,CANVAS_WIREMAKE.n2,
	              CANVAS_WIREMAKE.parity);
}

void __Display_FinishWireMake() {
	CANVAS_WIREFLAG = 0;
	for (component* c = comps; c != comps+comp_count; ++c) {
		int i;
		for (i = 0; i < c->in_count; ++i) {
			int x,y,w,h;
			Component_GetNodeRect(c, c->in+i, &x,&y,&w,&h);	
			if (RectMouseCollision(x,y,w,h)) {
				__Display_AddWire(c, -i-1);
				return;
			}
		}

		for (i = 0; i < c->out_count; ++i) {
			int x,y,w,h;
			Component_GetNodeRect(c, c->out+i, &x,&y,&w,&h);	
			if (RectMouseCollision(x,y,w,h)) {
				__Display_AddWire(c, i);
				return;
			}
		}
	}
}


void __Display_FinishCompAdd() {
	if (MOUSEY > TITLE_HEIGHT + TOOLBAR_HEIGHT) {
		component * c = COMP_DEFS[CANVAS_COMPADD];
		Logic_AddComponent(c, MOUSEX-c->w/2, MOUSEY-c->h/2, CANVAS_COMPROT);
	}

	CANVAS_COMPADD = -1;
}

void Display_BackspacePopup() {
	int len = strlen(POPUP_BUFFER);
	if (len == 0) return;
	POPUP_BUFFER[len-1] = '\0';
}

void Display_TypePopup(char * text) {
	int i = strlen(POPUP_BUFFER);
	char * c = text;

	while (i < POPUP_BUFFER_SIZE) {
		if (*c) POPUP_BUFFER[i] = *c;
		else break;
		++c;
		++i;
	}

	POPUP_BUFFER[i] = '\0';
}

void Display_QuitPopup() {
	POPUP_SAVING = 0;
	POPUP_LOADING = 0;
	strcpy(POPUP_BUFFER, "");
	SDL_StopTextInput();
}

void Display_EnterPopup() {
	// if string not empty
	if (POPUP_BUFFER[0]) {
		int result = 0;
		if (POPUP_SAVING) result=Save_SaveFile(POPUP_BUFFER);
		else if (POPUP_LOADING) result=Save_LoadFile(POPUP_BUFFER);

		if (!result) {
			NOTIF_TEXTBOX = 240;
			strcpy(NOTIF_BUFFER, SAVE_ERROR);
		}
	}
	Display_QuitPopup();
}

void Display_DrawTextbox(const char * text, int x, int y) {
	int w,h;

	TTF_SizeText(FONT, text, &w, &h);

	SDL_Color BG = {0x00,0x00,0x00,0xff};
	SDL_Color FG = {0xff,0xff,0xff,0xff};

	Render_Rect(x, y-h-1, w+4, h+2, &BG);
	Render_Text(text, x+3, y-h-1, ALIGN_LEFT, &FG);
	Render_RectLine(x, y-h-1, w+4, h+2, &FG);
}

void Display_RenderTitle() {
	SDL_Color BG = TITLE_BG;
	SDL_Color FG = TITLE_FG;
	Render_Rect(0, 0, WIN_W, TITLE_HEIGHT, &BG);

	static int count = 0;
	if (!LOGIC_PAUSE)
		++count;

	SDL_Rect VP = {80,0,WIN_W,TITLE_HEIGHT};
	SDL_RenderSetViewport(RENDER, &VP);
	Render_Text("Logic", (count) % (WIN_W+40)-40,-1, ALIGN_LEFT, &FG);
	SDL_RenderSetViewport(RENDER, NULL);

	// render buttons
	// pause/play button
	Render_Texture(LOGIC_PAUSE ? TITLE_PAUSE_IMG : TITLE_PLAY_IMG,
		NULL, &TITLE_PAUSE, 0.0);
	// grid on/off button
	Render_Texture(GRID_FLAG ? TITLE_GRIDON_IMG : TITLE_GRIDOFF_IMG,
		NULL, &TITLE_GRID, 0.0);
	// delete button
	Render_Texture(TITLE_DELETE_IMG, NULL, &TITLE_DELETE, 0.0);

	// save button
	Render_Texture(TITLE_SAVE_IMG, NULL, &TITLE_SAVE, 0.0);
	// load button
	Render_Texture(TITLE_LOAD_IMG, NULL, &TITLE_LOAD, 0.0);
}

void Display_RenderSavePopup() {
	POPUP_RECT.x = WIN_W/2 - POPUP_RECT.w/2;
	POPUP_RECT.y = WIN_H/2 - POPUP_RECT.h/2;

	SDL_Color BG = {0xff,0xff,0xff,0xff},
	          FG = {0x00,0x00,0x00,0xff};

	Render_Rect(POPUP_RECT.x, POPUP_RECT.y, POPUP_RECT.w, POPUP_RECT.h, &BG);
	Render_RectLine(POPUP_RECT.x, POPUP_RECT.y, POPUP_RECT.w, POPUP_RECT.h, &FG);
	Render_RectLine(POPUP_RECT.x+1, POPUP_RECT.y+1, POPUP_RECT.w-2, POPUP_RECT.h-2, &FG);

	SDL_Rect newtyper = TYPING_RECT;
	newtyper.x += POPUP_RECT.x;
	newtyper.y += POPUP_RECT.y;

	Render_RectLine(newtyper.x, newtyper.y, newtyper.w, newtyper.h, &FG);
	Render_RectLine(newtyper.x+1, newtyper.y+1, newtyper.w-2, newtyper.h-2, &FG);

	Render_Text(POPUP_SAVING_TEXT, POPUP_RECT.x+POPUP_RECT.w/2, POPUP_RECT.y+25,
	  ALIGN_MIDDLE, &FG);
	Render_Text(POPUP_BUFFER, newtyper.x+4, newtyper.y+2, ALIGN_LEFT, &FG);
	Render_Text(POPUP_ENTER_TEXT, POPUP_RECT.x+POPUP_RECT.w/2, POPUP_RECT.y+POPUP_RECT.h-25,
	  ALIGN_MIDDLE, &FG);
}

void Display_RenderLoadPopup() {
	POPUP_RECT.x = WIN_W/2 - POPUP_RECT.w/2;
	POPUP_RECT.y = WIN_H/2 - POPUP_RECT.h/2;

	SDL_Color BG = {0xff,0xff,0xff,0xff},
	          FG = {0x00,0x00,0x00,0xff};

	Render_Rect(POPUP_RECT.x, POPUP_RECT.y, POPUP_RECT.w, POPUP_RECT.h, &BG);
	Render_RectLine(POPUP_RECT.x, POPUP_RECT.y, POPUP_RECT.w, POPUP_RECT.h, &FG);
	Render_RectLine(POPUP_RECT.x+1, POPUP_RECT.y+1, POPUP_RECT.w-2, POPUP_RECT.h-2, &FG);

	SDL_Rect newtyper = TYPING_RECT;
	newtyper.x += POPUP_RECT.x;
	newtyper.y += POPUP_RECT.y;

	Render_RectLine(newtyper.x, newtyper.y, newtyper.w, newtyper.h, &FG);
	Render_RectLine(newtyper.x+1, newtyper.y+1, newtyper.w-2, newtyper.h-2, &FG);

	Render_Text(POPUP_LOADING_TEXT, POPUP_RECT.x+POPUP_RECT.w/2, POPUP_RECT.y+25,
	  ALIGN_MIDDLE, &FG);
	Render_Text(POPUP_BUFFER, newtyper.x+4, newtyper.y+2, ALIGN_LEFT, &FG);
	Render_Text(POPUP_ENTER_TEXT, POPUP_RECT.x+POPUP_RECT.w/2, POPUP_RECT.y+POPUP_RECT.h-25,
	  ALIGN_MIDDLE, &FG);
}

void Display_RenderToolbox() {
	SDL_Color BG = TOOLBAR_BG;

	SDL_Color c = TOOLBAR_FG,
	          h = TOOLBAR_HOVER_C;


	// render background
	Render_Rect(0, TITLE_HEIGHT, WIN_W, TOOLBAR_HEIGHT, &BG);

	// render component icons
	int i = TOOLBAR_SCROLL,
	    end = (WIN_W-TOOLBAR_SCROLL_W)/TOOLBAR_HEIGHT + TOOLBAR_SCROLL;
	for (; i <= end && i < COMP_DEF_COUNT; ++i) {
		int offset = (i-TOOLBAR_SCROLL)*(TOOLBAR_HEIGHT-2)
		  + TOOLBAR_SCROLL_W;

		SDL_Rect r = {offset, TITLE_HEIGHT-2, TOOLBAR_HEIGHT, TOOLBAR_HEIGHT+4};

		Render_RectLine(r.x  ,r.y  ,r.w  ,r.h  , &c);
		Render_RectLine(r.x+1,r.y+1,r.w-2,r.h-2, &c);

		r.x+=2;
		r.y+=2;
		r.w-=4;
		r.h-=4;

		SDL_RenderSetViewport(RENDER, &r);

		SDL_Rect tr = {0,0, COMP_DEFS[i]->w, COMP_DEFS[i]->h};

		tr.x += (r.w - tr.w)/2;
		tr.y += (r.h - tr.h)/2;

		if (TOOLBAR_COMPHOVER != i)
			Render_Texture(COMP_DEFS[i]->img_off, NULL, &tr, 0.0);
		else
			Render_Texture(COMP_DEFS[i]->img_on, NULL, &tr, 0.0);

		SDL_RenderSetViewport(RENDER, NULL);
	}

	// render borders and buttons
	
	// LEFT BUTTON
	Render_Rect(0, TITLE_HEIGHT,
	            TOOLBAR_SCROLL_W, TOOLBAR_HEIGHT, &BG);
	Render_Rect(2, TITLE_HEIGHT+2,
	            TOOLBAR_SCROLL_W-4, TOOLBAR_HEIGHT-4, TOOLBAR_LHOVER?&h:&c);

	// RIGHT BUTTON
	Render_Rect(WIN_W, TITLE_HEIGHT,
	            -TOOLBAR_SCROLL_W, TOOLBAR_HEIGHT, &BG);
	Render_Rect(WIN_W-2, TITLE_HEIGHT+2,
	            -TOOLBAR_SCROLL_W+4, TOOLBAR_HEIGHT-4,TOOLBAR_RHOVER?&h:&c);

	// RIGHT BUTTON BLACK BUFFER
	Render_Rect(WIN_W-TOOLBAR_SCROLL_W-2, TITLE_HEIGHT, -2, TOOLBAR_HEIGHT, &BG);

	Render_Rect(WIN_W-TOOLBAR_SCROLL_W-2,TITLE_HEIGHT,2,TOOLBAR_HEIGHT,&c);

	Render_Line(0, TITLE_HEIGHT+TOOLBAR_HEIGHT,
	  WIN_W, TITLE_HEIGHT+TOOLBAR_HEIGHT, &c);
	Render_Line(0, TITLE_HEIGHT+TOOLBAR_HEIGHT+1,
	  WIN_W, TITLE_HEIGHT+TOOLBAR_HEIGHT+1, &c);
}

void Display_RenderGrid() {
	if (!GRID_FLAG) return;

	int x,y;
	SDL_Color GC = GRID_COLOR;

	for (y = GRID_SIZE+TOOLBAR_HEIGHT+TITLE_HEIGHT; y < WIN_H; y += GRID_SIZE) {
		for (x = GRID_SIZE; x < WIN_W; x += GRID_SIZE) {
			Render_Line(x,0,x,WIN_H, &GC);
			Render_Line(0,y,WIN_W,y, &GC);
		}
	}
}
