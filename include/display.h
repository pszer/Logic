#pragma once

#include "logic.h"
#include "event.h"
#include "render.h"

#define TITLE_HEIGHT 12
#define TITLE_FG     {0x00,0x00,0x00,0xff}
#define TITLE_BG     {0xff,0xff,0xff,0xff}

#define TOOLBAR_HEIGHT 64
#define TOOLBAR_FG     {0xff,0xff,0xff,0xff}
#define TOOLBAR_BG     {0x13,0x13,0x13,0xff}

#define TOOLBAR_HOVER_C {0x8a,0xe8,0x0c,0xff}

#define TOOLBAR_SCROLL_W 10

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

extern int CANVAS_WIREHOVER;


int RectPointCollision(int x, int y, int rx, int ry, int rw, int rh);
int RectMouseCollision(int rx, int ry, int rw, int rh);
int WirePointCollision(int x, int y, wire * w);
int WireMouseCollision(wire * w);

void Display_Render();
void Display_RenderTitle();
void Display_RenderToolbox();

void Display_Update();

// returns 1 if input detected
int Display_InputCheckScrollButtons();
int Display_InputCheckToolbar();
int Display_InputCheckCanvas();

void Display_RightClick();

int  __Display_InputCheckNode(component* c);
void __Display_FinishWireMake();
void __Display_FinishCompAdd();
