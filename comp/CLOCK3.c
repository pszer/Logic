#include <SDL2/SDL_timer.h>

#define NAME "CLOCK (1/3 hz)"
#define IOFF "CLOCK3.png"
#define ION  "CLOCK3.png"

//void render(component*);
//void click (component*, int state, int ms);
//void destroy(component * c);

void update(component* c, int frame) {
	if (c->var == 0) c->var = frame;

	if (!(c->var%3)) c->state = 1;
	else c->state = 0;
	++c->var;

	c->out[0].state = c->state;
}

component c_CLOCK = {
	NAME,
	0,         //int state
	0,0,32,32, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	0,1,       //in_count, out_count

	//0 x y NULL
	{{0, 0, 0}}, //nodes in 
	{{0, 32, 16}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*, int frame);
	NULL,         //void (*render)(component*);
	NULL,         //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_CLOCK;
}
