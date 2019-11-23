#pragma once

#define MAX_TEXTURES 128

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <dirent.h>
#include <stdlib.h>
#include <string.h>

typedef struct texture {
	char * name;
	SDL_Texture * img;

	int w,h;
} texture;

extern int textures_loaded;
extern texture textures[MAX_TEXTURES];

/* loads all textures in a folder
 * -- returns how many textures were loaded
 * -- -1 if error loading folder
 */
int Texture_LoadFolder(SDL_Renderer *, char * folder);
void Texture_FreeAll();

/* gets loaded texture by filename
 * -- returns pointer to texture
 * -- returns NULL if texture not found
 */
texture * Texture_Get(char * name);

/* loads a texture by filename
 * -- returns 1 if success, 0 for error
 */
int  Texture_Load(SDL_Renderer *, char * filename);
void Texture_Free(texture * t);
