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

typedef struct tex_tree tex_tree;
// texture finding binary tree
struct tex_tree {
	texture * tex;

	tex_tree * left;
	tex_tree * right;
};

extern int textures_loaded;
extern texture textures[MAX_TEXTURES];

extern tex_tree * tex_search_tree;

tex_tree * Texture_CreateTree(texture * tex);
void Texture_AddTree (tex_tree * base, texture * tex);
void Texture_FreeTree(tex_tree * tree);

texture * Texture_TreeFind(tex_tree * tree, char * name);

/* -1 for less than
 *  0 for equal
 *  1 for greater than
 */
int String_LessThan(const char* a, const char* b);

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

/* gets loaded texture by filename with
 * a binary search tree
 * -- returns pointer to texture
 * -- returns NULL if texture not found
 */
texture * Texture_FastGet(char * name);

/* loads a texture by filename
 * -- returns 1 if success, 0 for error
 */
int  Texture_Load(SDL_Renderer *, char * filename);

void Texture_Free(texture * t);
