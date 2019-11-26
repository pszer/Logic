#define NAME "RESISTOR (4 DELAY)"
#define IOFF "img/RESIST.png"
#define ION  "img/RESIST.png"

//void render(component*);
//void click (component*, int state, int ms);
//void destroy(component * c);

void update(component* c, int frame) {
	c->var <<= 1;
	c->var += c->in[0].state;

	c->out[0].state = (c->var&0x08) ? 1 : 0;
	c->state = c->out[0].state;
}

component c_RESISTOR = {
	NAME,
	0,         //int state
	0,0,25,13, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	1,1,       //in_count, out_count

	//0 x y NULL
	{{0, 0 , 6}}, //nodes in 
	{{0, 25, 6}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*, int frame);
	NULL,         //void (*render)(component*);
	NULL,         //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_RESISTOR;
}
