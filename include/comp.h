#pragma once

#include <stdlib.h>

//#include "tex.h"
typedef struct texture texture;
typedef struct SDL_Texture SDL_Texture;
typedef struct SDL_Renderer SDL_Renderer;

#define MAX_IN  8
#define MAX_OUT 8

#define OFF_COLOR (SDL_Color){0xff,0xff,0xff,0xff}
#define ON_COLOR  (SDL_Color){0x51,0xd9,0x2b,0xff}

#define NODE_SIZE 5

typedef struct component component;

typedef struct node {
	int state;

	// these are treated as offsets
	// of the parent components position
	int x, y;
} node;

struct component {
	char * name;

	// whether its ON(1) or OFF(0)
	int state;

	int x,y,w,h;
	char *img_off, *img_on;

	// how many inputs/outputs this component has
	int in_count, out_count;
	node in [MAX_IN];
	node out[MAX_OUT];

	// quarter turns clockwise
	char rotation;

	// optional variable used in component
	// operation (e.g. on/off flag for a switch)
	// (e.g. pointer allocated outside of component)
	long long var;

	/* optional functions */
	void (*update)(component*, int);
	void (*render)(component*, SDL_Renderer*);
	//                        button ms
	void (*click) (component*, int, int);
	void (*destroy)(component*); // called when destroyed
};

node* Component_GetNode(component* c, int n);
void Component_GetNodePos (component* c, node* n, int* x, int* y);
void Component_GetNodeRect(component* c, node* n, int* x, int* y, int* w, int* h);

typedef struct wire {
	// whether its ON(1) or OFF(0)
	int state;

	// point1  point2
	int x1, y1, x2, y2;

	// pointer to wire terminal components
	component * c1, * c2;

	// index to node
	// >=  0 is an output node (index will be n)
	// <= -1 is an input node  (index will be -n-1)
	int n1, n2; 

	// wire shape
	//
	//  parity=0  |  parity=1 |  parity=2 |  parity=3
	//            |           |           |
	//      |--p2 |      p2   |       p2  |  -----p2
	//      |     |  ____|    |       |   |  |
	//      |     |  |        |       |   |  |
	//  p1--|     |  p1       |  p1---|   |  p1
	//
	char parity;

	//  0 - uncalculated
	//  1 - goes from p1 to p2
	// -1 - goes from p2 to p1
	int flow;
} wire;

/* returns component with 0/NULL in all members */
component def_null_component();

