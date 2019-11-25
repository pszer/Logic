#define NAME "NOT"
#define IOFF "img/NOT.png"
#define ION  "img/NOTon.png"

//void render(component*);
//void click (component*, int state, int ms);

void update(component* c) {
	c->out[0].state = !c->in[0].state;
	c->state = c->out[0].state;
}

component c_NOT = {
	NAME,
	0,         //int state
	0,0,24,24, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	1,1,       //in_count, out_count

	//0 x y NULL
	{{0, 0 , 12}}, //nodes in 
	{{0, 22, 12}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,  //void (*update)(component*);
	NULL,          //void (*render)(component*);
	NULL,          //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_NOT;
}
