#include "comp.h"


component def_null_component() {
	component c;
	char * byte;
	for (byte=(char*)&c; byte<(char*)&c+sizeof(component); ++byte)
		*byte=0;
	return c;
}

node* Component_GetNode(component* c, int n) {
	if (!c) return NULL;
	if (n >= 0)
		return c->out + n;
	else
		return c->in - (n+1);
}

void Component_GetNodePos (component* c, node* n, int* x, int* y) {
	if (x) *x = c->x + n->x;
	if (y) *y = c->y + n->y;
}

void Component_GetNodeRect(component* c, node* n,
	int* x, int *y, int *w, int *h)
{
	if(x) *x = c->x + n->x - NODE_SIZE/2;
	if(y) *y = c->y + n->y - NODE_SIZE/2;
	if(w) *w = NODE_SIZE;
	if(h) *h = NODE_SIZE;  
}
