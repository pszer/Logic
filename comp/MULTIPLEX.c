#define NAME "MULTIPLEX"
#define IOFF "MULTIPLEX.png"
#define ION  "MULTIPLEX.png"

//void render(component*, SDL_Renderer*);
//void click (component*, int state, int ms);
//void destroy(component * c);

void update(component* c, int frame) {
	c->var = (c->in[4].state<<1) + c->in[5].state;

	switch (c->var) {
	case 0: c->out[0].state = c->in[0].state; break;
	case 1: c->out[0].state = c->in[1].state; break;
	case 2: c->out[0].state = c->in[2].state; break;
	case 3: c->out[0].state = c->in[3].state; break;
	}
}

component c_MULTIPLEX = {
	NAME,
	0,         //int state
	0,0,20,40, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	6,1,       //in_count, out_count

	//0 x y NULL
	{{0,0,5}, {0,0,15}, {0,0,25}, {0,0,35}, {0,6,37}, {0,14,35}}, //nodes in 
	{{0, 20, 20}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*, int frame);
	NULL,         //void (*render)(component*);
	NULL,         //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_MULTIPLEX;
}
