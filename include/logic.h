#pragma once

#include <dirent.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

#include "comp.h"

#define COMP_PATH    "comp/"
#define COMP_SO_PATH "comp/so/"

#define MAX_COMP_DEFS 64

#define START_COMP_SIZE 32
#define START_WIRE_SIZE 32

// standard components used in
// construction
extern int COMP_DEF_COUNT;
extern component * COMP_DEFS[MAX_COMP_DEFS];
extern void      * COMP_HANDLES[MAX_COMP_DEFS];

extern int LOGIC_PAUSE;
extern int LOGIC_FRAME;

// set by Logic_DefineComps to signal finished thread
extern int __dfndone;
extern char* __dfnstr;

// compiles and loads all components on comp
// (calls DefineCompFile for all files in a folder)
void* Logic_DefineComps(void*);
void Logic_FreeDefines();
void Logic_SortDefines();

/* takes component filename (without folder prefix)
 * and adds it to definitions
 * -- returns 1 for success
 * -- returns 0 for error
 */
int Logic_DefineCompFile(const char * fname);

// components on the board
// comp_count is how many components there are
// comp_size is size of *comps array
extern int comp_count, comp_size;
extern component* comps;

extern int wire_count, wire_size;
extern wire* wires;

// if comps is already allocated, it reallocates
// it
void Logic_AllocateComponents(int size);
void Logic_AllocateWires(int size);
void Logic_FreeComponents();
void Logic_FreeWires();

void Logic_AddComponent(const component* comp_type, int x, int y, char rotation);
void Logic_AddWire(component* a, component* b, int n1, int n2, int parity);

void Logic_DeleteComponent(int index);
void Logic_DeleteWire(int index);

void Logic_DeleteAll();

void Logic_Update();
void Logic_NullInputNodes();
void Logic_UpdateWires();
void Logic_UpdateComponents();

void Logic_UpdateAllWirePos();
void Logic_UpdateWirePos(wire * w);
