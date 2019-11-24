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
	*x = n->x - c->w/2;
	*y = n->y - c->h/2;

	int temp;
	switch (c->rotation % 4) {
	case 0:
		break;
	case 1:
		temp = *x;
		*x = *y;
		*y = temp;
		break;
	case 2:
		*x = -*x;
		*y = -*y;
		break;
	case 3:
		temp = *x;
		*x = *y;
		*y = -temp;
		break;
	}

	*x += c->x + c->w/2;
	*y += c->y + c->h/2;
}

void Component_GetNodeRect(component* c, node* n,
	int* x, int *y, int *w, int *h)
{
	if (!(x&&y&&w&&h)) return;

	Component_GetNodePos(c,n,x,y);

	*x = *x - NODE_SIZE/2;
	*y = *y - NODE_SIZE/2;
	*w = NODE_SIZE;
	*h = NODE_SIZE;

	/*if(x) *x = c->x + n->x - NODE_SIZE/2;
	if(y) *y = c->y + n->y - NODE_SIZE/2;
	if(w) *w = NODE_SIZE;
	if(h) *h = NODE_SIZE;*/
}
