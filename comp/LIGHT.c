#define NAME "LIGHT"
#define IOFF "img/LIGHT.png"
#define ION  "img/LIGHTon.png"

//void render(component*);
//void click (component*, int state, int ms);

void update(component* c) {
	c->state = c->in[0].state;
}

component c_LIGHT = {
	NAME,
	0,         //int state
	0,0,32,32, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	1,0,       //in_count, out_count

	//0 x y NULL
	{{0, 0 , 16}}, //nodes in 
	{}, //nodes out

	0, //int rotation;
	0, //int var;

	update, //void (*update)(component*);
	NULL,         //void (*render)(component*);
	NULL          //void (*click) (component*, int, int);
};

component* __load__() {
	return &c_LIGHT;
}
