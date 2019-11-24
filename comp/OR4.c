#define NAME "OR4"
#define IOFF "img/OR4.png"
#define ION  "img/OR4on.png"

//void render(component*);
//void click (component*, int state, int ms);

void update(component* c) {
	c->out[0].state = c->in[0].state || c->in[1].state ||
	                  c->in[2].state || c->in[3].state;

	c->state = c->out[0].state;
}

component c_OR4 = {
	NAME,
	0,         //int state
	0,0,24,48, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	4,1,       //in_count, out_count

	//0 x y NULL
	{{0,2,6}, {0,4,18}, {0,4,30}, {0,2,42}}, //nodes in 
	{{0, 24, 24}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*);
	NULL,         //void (*render)(component*);
	NULL          //void (*click) (component*, int, int);
};

component* __load__() {
	return &c_OR4;
}
