#pragma once

#include <stdio.h>
#include <stdint.h>

#include "logic.h"

// string for errors
extern char SAVE_ERROR[64];

#define SAVE_SIG 0x76CE6F55

/* FILE SPECIFICATION
 * uint32 0x76CE6F55 file signature
 *
 * uint32 component count
 * uint32 wire count
 *
 * component {
 * uint16 component name length (no null terminator)
 * char* component name
 * int32 x
 * int32 y
 * } repeated for each component
 *
 * wire {
 * uint32 component 1 index
 * uint32 component 2 index
 * int32 component 1 node index
 * int32 component 2 node index
 * uint8  parity
 * } repeated for each wire
 *
 */

#define SAVE_PATH "saves/"
#define SAVE_TEMP "saves/__TEMPORARY__.save"

/* Saves or loads file
 * full path is SAVE_PATH/filename
 * -- returns 1 for success
 * -- returns 0 for error
 *
 * if an error occurs, doesn't delete
 * current logic components
 *
 * SaveFile firsts writes to a temp
 * file before replacing any currently written
 * file.
 */
int Save_LoadFile(char* fname);
int Save_SaveFile(char* fname);

// functions used by the above
int Save_WriteComponent(component*, FILE*);
int Save_WriteWire(wire*, FILE*);
int Save_LoadComponent(component*, FILE*);
int Save_LoadWire(wire*, FILE*, component* comps, int compc);
