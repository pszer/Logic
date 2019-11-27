#include <string.h>

#include <SDL2/SDL.h>
#include "tex.h"

#define NAME "COUNTER 4-BIT"
#define IOFF "counter4.png"
#define ION  "counter4.png"

#define NUM "DIG#.png"

SDL_Texture * find_tex(texture* texs, int tc, char * name) {
	int i;
	for (i = 0; i < tc; ++i) {
		if (!strcmp(texs[i].name, name)) return texs[i].img;
	}
	return NULL;
}

void render(component* c, SDL_Renderer * R, texture* texs, int tc) {
	char str[] = NUM;

	SDL_Rect r = {c->x, c->y, c->w, c->h};
	SDL_RenderCopy(R, find_tex(texs,tc,IOFF), NULL, &r);

	r.x=c->x+17;
	r.y=c->y+5;
	r.w=11;
	r.h=21;

	if (c->var >= 10) {
		str[3]='0' + (c->var/10)%10;
		SDL_RenderCopy(R, find_tex(texs,tc,str), NULL, &r);
	}

	r.x = c->x+30;
	str[3] = '0' + (c->var%10);
	SDL_RenderCopy(R, find_tex(texs,tc,str), NULL, &r);
}

//void click (component*, int state, int ms);

void update(component* c, int frame) {
	c->var = c->in[0].state*0x08 +
	         c->in[1].state*0x04 +
	         c->in[2].state*0x02 +
	         c->in[3].state*0x01;

	// fix rotation to 0 or 180 degrees
	if (c->rotation==1)
		c->rotation=2;
	if (c->rotation==3)
		c->rotation=0;
}

component c_COUNTER = {
	NAME,
	0,         //int state
	0,0,48,32, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	4,0,       //in_count, out_count

	//0 x y NULL
	{{0,6,0}, {0,18,0}, {0,30,0}, {0,42,0}}, //nodes in
	{}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*, int frame);
	render,       //void (*render)(component*);
	NULL,         //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_COUNTER;
}
