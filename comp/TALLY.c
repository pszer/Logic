#include <string.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include "render.h"

#define NAME "TALLY 7-BIT"
#define IOFF "tally.png"
#define ION  "tally.png"

#define NUM "DIG#.png"

void render(component* c, SDL_Renderer * R) {
	char str[] = NUM;

	SDL_Rect r = {c->x, c->y, c->w, c->h};
	Render_Texture(IOFF, NULL, &r, 0.0);

	r.x=c->x+17;
	r.y=c->y+5;
	r.w=11;
	r.h=21;

	if (c->var >= 10) {
		str[3]='0' + (c->var/10)%10;
		Render_Texture(str, NULL, &r, 0.0);
	}

	r.x = c->x+30;
	str[3] = '0' + (c->var%10);
	Render_Texture(str, NULL, &r, 0.0);
}

//void click (component*, int state, int ms);
//void destroy(component * c);

void update(component* c, int frame) {
	/*c->var = c->in[0].state*0x08 +
	         c->in[1].state*0x04 +
	         c->in[2].state*0x02 +
	         c->in[3].state*0x01;*/

	// if reset
	if (c->in[1].state) {
		c->var = 0;
		c->state = 1;
	} else {
		c->state = (c->state<<1) + c->in[0].state;
		//c->state = (c->state<<1) + c->state;

		if ((c->state&0x01) && !(c->state&0x02)) {
			++c->var;
			if (c->var > 99)
				c->var = 0;
		}
	}


	c->out[6].state = (c->var&0x01) ? 1 : 0;
	c->out[5].state = (c->var&0x02) ? 1 : 0;
	c->out[4].state = (c->var&0x04) ? 1 : 0;
	c->out[3].state = (c->var&0x08) ? 1 : 0;
	c->out[2].state = (c->var&0x10) ? 1 : 0;
	c->out[1].state = (c->var&0x20) ? 1 : 0;
	c->out[0].state = (c->var&0x40) ? 1 : 0;

	c->rotation=0;
}

component c_TALLY = {
	NAME,
	0,         //int state
	0,0,48,32, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	2,7,       //in_count, out_count

	//0 x y NULL
	{{0,0,7}, {0,0,21}}, // nodes in
	{{0,6,0}, {0,12,0}, {0,18,0}, {0,24,0}, {0,30,0},{0,36,0}, {0, 42,0}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*, int frame);
	render,       //void (*render)(component*);
	NULL,         //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_TALLY;
}
