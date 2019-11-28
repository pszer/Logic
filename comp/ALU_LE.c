#define NAME "ALU LESS"
#define IOFF "ALU_LE.png"
#define ION  "ALU_LE.png"

//void click (component*, int state, int ms);

void update(component* c, int frame) {
	c->var =(c->in[3].state*0x01 +
	         c->in[2].state*0x02 +
	         c->in[1].state*0x04 +
	         c->in[0].state*0x08)

			<

		(c->in[7].state*0x01 +
	         c->in[6].state*0x02 +
	         c->in[5].state*0x04 +
	         c->in[4].state*0x08);

	c->out[0].state = c->var?1:0;
}

component c_ALU_LE = {
	NAME,
	0,         //int state
	0,0,32,64, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	8,1,       //in_count, out_count

	//0 x y NULL
	{{0,0,7} ,{0,0,13},{0,0,19},{0,0,25},
	 {0,0,39},{0,0,45},{0,0,51},{0,0,57}}, //nodes in
	{{0,32,32}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*, int frame);
	NULL,       //void (*render)(component*);
	NULL,         //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_ALU_LE;
}
