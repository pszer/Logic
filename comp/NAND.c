#define NAME "NAND"
#define IOFF "img/NAND.png"
#define ION  "img/NANDon.png"

//void render(component*);
//void click (component*, int state, int ms);

void update(component* c) {
	c->out[0].state = c->in[0].state && c->in[1].state;
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

	1, //char polarity;
	0, //int var;

	update,       //void (*update)(component*);
	NULL,         //void (*render)(component*);
	NULL          //void (*click) (component*, int, int);
};

component* __load__() {
	return &c_NAND;
}