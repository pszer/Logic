#define NAME "TOGGLE"
#define IOFF "TOGGLE.png"
#define ION  "TOGGLEon.png"

//void render(component* c, SDL_Renderer * R) {
//void click (component*, int state, int ms);
//void destroy(component * c);

void update(component* c, int frame) {
	//c->var += c->in[0].state;
	c->var = (c->var<<1) + c->in[0].state;

	if ((c->var&0x01) && !(c->var&0x02)) {
		c->state = !c->state;
	}

	c->out[0].state = c->state;
}

component c_TOGGLE = {
	NAME,
	0,         //int state
	0,0,16,16, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	1,1,       //in_count, out_count

	//0 x y NULL
	{{0,0,8}}, // nodes in
	{{0,16,8}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*, int frame);
	NULL,         //void (*render)(component*);
	NULL,         //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_TOGGLE;
}
