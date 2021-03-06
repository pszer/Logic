#define NAME "NAND"
#define IOFF "NAND.png"
#define ION  "NANDon.png"

//void render(component*, SDL_Renderer*);
//void click (component*, int state, int ms);

void update(component* c, int frame) {
	c->out[0].state = !(c->in[0].state && c->in[1].state);
	c->state = c->out[0].state;
}

component c_NAND = {
	NAME,
	0,         //int state
	0,0,28,24, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	2,1,       //in_count, out_count

	//0 x y NULL
	{{0, 0 , 6}, {0, 0, 18} }, //nodes in 
	{{0, 24, 12}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*, int frame);
	NULL,         //void (*render)(component*);
	NULL,         //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_NAND;
}
