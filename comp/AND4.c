#define NAME "AND4"
#define IOFF "AND4.png"
#define ION  "AND4on.png"

//void render(component*, SDL_Renderer *);
//void click (component*, int state, int ms);

void update(component* c, int frame) {
	c->out[0].state = c->in[0].state && c->in[1].state &&
	                  c->in[2].state && c->in[3].state;

	c->state = c->out[0].state;
}

component c_AND4 = {
	NAME,
	0,         //int state
	0,0,24,48, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	4,1,       //in_count, out_count

	//0 x y NULL
	{{0,0,6}, {0,0,18}, {0,0,30}, {0,0,42}}, //nodes in 
	{{0, 24, 24}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*, int frame);
	NULL,         //void (*render)(component*);
	NULL,         //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_AND4;
}
