#define NAME "SWITCH"
#define IOFF "img/switch.png"
#define ION  "img/switchon.png"

//void render(component*);

void click (component* c, int state, int ms) {
	if (state == 1 && ms < 300)
		c->var = !c->var;
}

void update(component* c, int frame) {
	c->state = c->var;
	c->out[0].state = c->state;
}

component c_SWITCH = {
	NAME,
	0,         //int state
	0,0,32,32, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	0,1,       //in_count, out_count

	//0 x y NULL
	{}, //nodes in 
	{{0, 32, 16}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*, int frame);
	NULL,         //void (*render)(component*);
	click,        //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_SWITCH;
}
