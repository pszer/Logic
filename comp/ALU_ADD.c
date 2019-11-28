#define NAME "ALU ADD"
#define IOFF "ALU_ADD.png"
#define ION  "ALU_ADD.png"

//void click (component*, int state, int ms);

void update(component* c, int frame) {
	c->var = c->in[3].state*0x01 +
	         c->in[2].state*0x02 +
	         c->in[1].state*0x04 +
	         c->in[0].state*0x08 +

		 c->in[7].state*0x01 +
	         c->in[6].state*0x02 +
	         c->in[5].state*0x04 +
	         c->in[4].state*0x08;

	c->out[4].state = (c->var&0x10)?1:0;
	c->out[0].state = (c->var&0x08)?1:0;
	c->out[1].state = (c->var&0x04)?1:0;
	c->out[2].state = (c->var&0x02)?1:0;
	c->out[3].state = (c->var&0x01)?1:0;
}

component c_ALU_ADD = {
	NAME,
	0,         //int state
	0,0,32,64, //int x,y,w,h

	//img_off, img_on
	  IOFF   , ION,  

	8,5,       //in_count, out_count

	//0 x y NULL
	{{0,0,7} ,{0,0,13},{0,0,19},{0,0,25},
	 {0,0,39},{0,0,45},{0,0,51},{0,0,57}}, //nodes in
	{{0,32,23},{0,32,29},{0,32,35},{0,32,41},{0,32,13}}, //nodes out

	0, //int rotation;
	0, //int var;

	update,       //void (*update)(component*, int frame);
	NULL,       //void (*render)(component*);
	NULL,         //void (*click) (component*, int, int);
	NULL          //void (*destroy)(component*);
};

component* __load__() {
	return &c_ALU_ADD;
}
