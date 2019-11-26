#include "logic.h"

// globals
int COMP_DEF_COUNT = 0;
component * COMP_DEFS[MAX_COMP_DEFS];
void      * COMP_HANDLES[MAX_COMP_DEFS];

int comp_count = 0, comp_size = 0;
component* comps = NULL;

int wire_count = 0, wire_size = 0;
wire* wires = NULL;

int LOGIC_PAUSE = 0;
int LOGIC_FRAME = 0;

int __dfndone;
char* __dfnstr;

void* Logic_DefineComps(void* _v) {
	DIR * d;
	struct dirent * dir;

	//__dfndone = 0;

	d = opendir(COMP_PATH);

	if (!d) {
		__dfndone = 1;
		return NULL;
	}

	while ((dir = readdir(d)) != NULL) {
		if (dir->d_type == DT_REG) {
			__dfnstr = dir->d_name;
			Logic_DefineCompFile(dir->d_name);

			if (COMP_DEF_COUNT == MAX_COMP_DEFS) break;
		}
	}

	__dfndone = 1;
	closedir(d);
	return NULL;
}

// takes component filename (without folder prefix)
// and adds it to definitions
// returns 1 for successful load
// 0 for error
int Logic_DefineCompFile(const char * fname) {
	const int cp_len   = strlen(COMP_PATH);
	const int cpso_len = strlen(COMP_SO_PATH);
	const int fn_len   = strlen(fname);

	if (!(fname[fn_len-1] == 'c' || fname[fn_len-1] == 'C')) {
		return 0;
	}

	const int path_len = cp_len + fn_len + 2;
	char* path = malloc( sizeof(char) * path_len);

	// path = 'COMP_PATH/fname'
	strcpy(path, COMP_PATH);
	strcat(path, fname);
	//printf("loading component '%s'\n", path);

	const int output_len = cpso_len + fn_len + 5;
	char* output = malloc(sizeof(char) * output_len);

	// output = 'COMP_SO_PATH/fname.so'
	strcpy(output, COMP_SO_PATH);
	strcat(output, fname);
	strcat(output, ".so");

	remove(output);

	// compiler command
	char* cc_com = malloc(sizeof(char)*
	  (path_len+output_len+80));
	strcpy(cc_com, "cc -std=c99 -O0 \"");
	strcat(cc_com, path);
	strcat(cc_com, "\" -o \"");
	strcat(cc_com, output);
	strcat(cc_com, "\" -shared -fPIC -Iinclude -include include/comp.h -lSDL2");

	// compile component
	//printf("%s\n", cc_com);
	system(cc_com);

	// open compiled library
	void * handle = dlopen(output, RTLD_LAZY);
	// free strings used
	free(output);
	free(path);
	free(cc_com);

	if (!handle) return 0;

	dlerror(); // clears error state

	// functions
	component* (*__load__)();
	void (*temp)();
	char * error;

	// get the __load__ function
	*(void **) (&__load__) = dlsym(handle, "__load__");
	if ((error = dlerror()) != NULL)
		return 0;

	// add definitions
	COMP_DEFS   [COMP_DEF_COUNT] = (*__load__)();
	COMP_HANDLES[COMP_DEF_COUNT] = handle;

#define __LOAD(x) if (COMP_DEFS[COMP_DEF_COUNT]->x) {\
		*(void **) (&temp)=dlsym(handle,#x);COMP_DEFS[COMP_DEF_COUNT]->x=temp;}
	//if (COMP_DEFS[COMP_DEF_COUNT].update()) 
	__LOAD(update);
	__LOAD(render);
	__LOAD(click);
	__LOAD(destroy);

	++COMP_DEF_COUNT;

	return 1;
}

// if comps is already allocated, it reallocates it
void Logic_AllocateComponents(int size) {
	if (!comps) {
		comps = malloc(size * sizeof(component));
		comp_size = size;
	} else {
		component * old = comps;

		comps = realloc(comps, size * sizeof(component));
		comp_size = size;

		// adjust wire component pointers
		int i;
		for (i = 0; i < wire_count; ++i) {
			wire * w = wires + i;
			w->c1 = comps + (w->c1 - old);
			w->c2 = comps + (w->c2 - old);
		}
	}
}

void Logic_AllocateWires(int size) {
	if (!wires) {
		wires = malloc(size * sizeof(wire));
		wire_size = size;
	} else {
		wires = realloc(wires, size * sizeof(wire));
		wire_size = size;
	}
}

void Logic_FreeDefines() {
	int i;
	for (i = 0; i < COMP_DEF_COUNT; ++i) {
		dlclose(COMP_HANDLES[i]);
	}
}

void Logic_FreeComponents() {
	if (comps) {
		int i;
		for (i = comp_count-1; i >= 0; --i)
			Logic_DeleteComponent(i);
		free(comps);
	}
	comp_count = 0;
	comps = NULL;
}

void Logic_FreeWires() {
	if (wires) free(wires);
	wire_count = 0;
	wires = NULL;
}

void Logic_AddComponent(const component* comp_type, int x, int y, char rotation) {
	if (comp_count >= comp_size)
		Logic_AllocateComponents(comp_size*2+1);

	component * c = comps + comp_count;
	*c = *comp_type;
	c->x = x;
	c->y = y;
	c->rotation = rotation;

	++comp_count;
}

void Logic_DeleteComponent(int index) {
	if (index<0 || index>=comp_count) return;
	component * c = comps + index;

	if (c->destroy) {
		c->destroy(c);
	}

	// first, delete all wires
	// connected to the component
	int i;
	for (i = 0; i < wire_count;) {
		wire * w = wires+i;
		if (w->c1==c || w->c2==c) {
			Logic_DeleteWire(i);
		} else {
			if (w->c1-comps > index) --w->c1;
			if (w->c2-comps > index) --w->c2;

			++i;
		}
	}

	for (i = index; i < comp_count-1; ++i) {
		comps[i] = comps[i+1];
	}

	--comp_count;
}

void Logic_DeleteWire(int index) {
	if (index<0 || index>=wire_count) return;
	
	int i;
	for (i = index; i < wire_count-1; ++i) {
		wires[i] = wires[i+1];
	}

	--wire_count;
}

void Logic_AddWire(component* a, component* b, int n1, int n2, int parity) {
	if (!(a&&b)) return;

	// stop output->output or input->input wire
	if (n1>=0 && n2>= 0) return;
	if (n1<0 && n2< 0) return;

	int i;
	for (int i = 0; i < wire_count; ++i) {
		wire * w = wires + i;

		if (n1 < 0) {
			if (w->n1==n1 && w->c1==a) return;
			if (w->n2==n1 && w->c2==a) return;
		}
		if (n2 < 0) {
			if (w->n1==n2 && w->c1==b) return;
			if (w->n2==n2 && w->c2==b) return;
		}
	}

	if (wire_count == wire_size)
		Logic_AllocateWires(comp_size*2+1);

	wire * w = wires + wire_count;
	w->state = 0;
	w->parity = parity;
	w->flow = 0;

	w->c1 = a;
	w->c2 = b;
	w->n1 = n1;
	w->n2 = n2;

	Component_GetNodePos(a, Component_GetNode(a,n1), &w->x1, &w->y1);
	Component_GetNodePos(b, Component_GetNode(b,n2), &w->x2, &w->y2);

	++wire_count;
}

void Logic_DeleteAll() {
	Logic_FreeComponents();
	Logic_FreeWires();

	Logic_AllocateComponents(START_COMP_SIZE);
	Logic_AllocateWires(START_WIRE_SIZE);
}

void Logic_Update() {
	Logic_NullInputNodes();
	Logic_UpdateWires();

	if (LOGIC_PAUSE) return;

	Logic_UpdateComponents();

	++LOGIC_FRAME;
}

void Logic_UpdateWires() {
	for (int i = 0; i < wire_count; ++i) {
		wire * w = wires + i;

		node * N1 = Component_GetNode(w->c1,w->n1),
		     * N2 = Component_GetNode(w->c2,w->n2);

		// if output node
		if (w->n1>=0) w->state = N1->state;
		if (w->n2>=0) w->state = N2->state;

		N1->state = w->state;
		N2->state = w->state;
	}
}

void Logic_NullInputNodes() {
	int i,j;
	for (i = 0; i < comp_count; ++i) {
		component * c = comps + i;
		for (j = 0; j < c->in_count; ++j) {
			c->in[j].state = 0;
		}
	}
}

void Logic_UpdateComponents() {
	for (int i = 0; i < comp_count; ++i) {
		if (comps[i].update) {
			comps[i].update(comps+i, LOGIC_FRAME);
		}
	}
}

void Logic_UpdateAllWirePos() {
	for (int i = 0; i < wire_count; ++i) {
		Logic_UpdateWirePos(wires + i);
	}
}

void Logic_UpdateWirePos(wire * w) {
	Component_GetNodePos(w->c1, Component_GetNode(w->c1,w->n1), &w->x1, &w->y1);
	Component_GetNodePos(w->c2, Component_GetNode(w->c2,w->n2), &w->x2, &w->y2);
}
