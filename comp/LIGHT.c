#define NAME "LIGHT"
#define IOFF "LIGHT.png"
#define ION  "LIGHTon.png"

//void render(component*, SDL_Renderer*);
//void click (component*, int state, int ms);

void update(component* c, int frame) {
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

	update,       //void (*update)(component*, int frame);
	NULL,         //void (*render)(component*);
	NULL,         //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_LIGHT;
}
