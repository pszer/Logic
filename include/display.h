#pragma once

#include "logic.h"
#include "event.h"
#include "render.h"
#include "save.h"

#define TITLE_HEIGHT 12
#define TITLE_FG     {0x00,0x00,0x00,0xff}
#define TITLE_BG     {0xff,0xff,0xff,0xff}

#define TOOLBAR_HEIGHT 64
#define TOOLBAR_FG     {0xff,0xff,0xff,0xff}
#define TOOLBAR_BG     {0x13,0x13,0x13,0xff}

#define TOOLBAR_HOVER_C {0x8a,0xe8,0x0c,0xff}

#define TOOLBAR_SCROLL_W 10

#define GRID_COLOR {0x2A,0x2A,0x2A,0xff}
#define GRID_SIZE 60

extern int TOOLBAR_SCROLL;
extern int TOOLBAR_LHOVER, TOOLBAR_RHOVER;
extern int TOOLBAR_COMPHOVER;

// -1 if no moving
// otherwise its component index
extern int CANVAS_COMPMOVE;

// flag set to 1 if drawing a wire
extern int  CANVAS_WIREFLAG;
extern wire CANVAS_WIREMAKE;

// -1 if no component being added
// otherwise its the component type index
extern int CANVAS_COMPADD;

// indexes for hovered over objects
extern int CANVAS_WIREHOVER;
extern int CANVAS_NODECOMPHOVER;
extern int CANVAS_NODEHOVER;

#define POPUP_SAVING_TEXT  "Save to where (in saves/)"
#define POPUP_LOADING_TEXT "Load from where (in saves/)"
#define POPUP_ENTER_TEXT   "[press ENTER] [click outside popup to cancel]"

#define POPUP_BUFFER_SIZE 50
extern char POPUP_BUFFER[POPUP_BUFFER_SIZE+1];
extern int  POPUP_SAVING;
extern int  POPUP_LOADING;
extern SDL_Rect POPUP_RECT;

#define NOTIF_TEXTBOX_SIZE 64
extern char NOTIF_BUFFER[NOTIF_TEXTBOX_SIZE+1];
extern int NOTIF_TEXTBOX;

#define TITLE_DELETE_IMG  "img/ICON_DELETE.png"
#define TITLE_GRIDON_IMG  "img/ICON_GRID.png"
#define TITLE_GRIDOFF_IMG "img/ICON_GRIDOFF.png"
#define TITLE_PAUSE_IMG   "img/ICON_PAUSE.png"
#define TITLE_PLAY_IMG    "img/ICON_PLAY.png"
#define TITLE_SAVE_IMG    "img/ICON_SAVE.png"
#define TITLE_LOAD_IMG    "img/ICON_LOAD.png"
#define TITLE_HOME_IMG    "img/ICON_HOME.png"

// button rectangles
extern SDL_Rect TITLE_DELETE;
extern SDL_Rect TITLE_GRID;
extern SDL_Rect TITLE_PAUSE;
extern SDL_Rect TITLE_SAVE;
extern SDL_Rect TITLE_LOAD;

extern int TRANSFORM_X, TRANSFORM_Y;

extern int GRID_FLAG;

int RectPointCollision(int x, int y, int rx, int ry, int rw, int rh);
int RectMouseCollision(int rx, int ry, int rw, int rh);
int WirePointCollision(int x, int y, wire * w);
int WireMouseCollision(wire * w);

void Display_Render(void);
void Display_RenderTitle(void);
void Display_RenderToolbox(void);
void Display_RenderGrid(void);

void Display_RenderSavePopup(void);
void Display_RenderLoadPopup(void);

void Display_Update(void);
void Display_NullFlags(void);

// returns 1 if input detected
int Display_InputCheckTitleBar(void);
int Display_InputCheckScrollButtons(void);
int Display_InputCheckToolbar(void);
int Display_InputCheckCanvas(void);

void Display_LeftClick(void);
void Display_RightClick(void);

void Display_PopupUpdate(void);
void Display_BackspacePopup(void);
void Display_TypePopup(char * text);
void Display_QuitPopup(void);
void Display_EnterPopup(void);

void Display_TransformCamera(int x, int y);
void Display_ResetCamera(void);

int  __Display_InputCheckNode(component* c);
void __Display_FinishWireMake(void);
void __Display_FinishCompAdd(void);
