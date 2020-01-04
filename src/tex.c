#include "tex.h"

int textures_loaded=0;
texture textures[MAX_TEXTURES];

tex_tree * tex_search_tree = NULL;

int String_LessThan(const char* a, const char* b) {
	const char * c = a, * d = b;
	while (1) {
		if (!*c &&  *d) return -1; // ab  < abc
		if ( *c && !*d) return  1; // abc > ab
		if (!*c && !*d) return  0; // abc = abc

		if (*c < *d) return -1; // a < b
		if (*c > *d) return  1; // b > a

		// *c==*d if no checks above happened

		// check next character
		++c;
		++d;
	}
}

tex_tree * Texture_CreateTree(texture * tex) {
	tex_tree * tree = malloc(sizeof(tex_tree));
	if (!tree) return NULL;

	tree->tex      = tex;
	tree->left     = NULL;
	tree->right    = NULL;

	return tree;
}

void Texture_AddTree (tex_tree * tree, texture * tex) {
	int comp = String_LessThan(tree->tex->name, tex->name);

	if (comp == -1) {
		if (tree->left)
			Texture_AddTree(tree->left, tex);
		else
			tree->left = Texture_CreateTree(tex);
	} else if (comp == 1) {
		if (tree->right)
			Texture_AddTree(tree->right, tex);
		else
			tree->right = Texture_CreateTree(tex);
	}
}

void Texture_FreeTree (tex_tree * tree) {
	if (tree->left) Texture_FreeTree(tree->left);
	if (tree->right) Texture_FreeTree(tree->right);

	free(tree);
}

texture * Texture_TreeFind(tex_tree * tree, char * name) {
	int comp = String_LessThan(tree->tex->name, name);

	if (comp == 0)
		return tree->tex;
	else if (comp == -1 && tree->left )
		return Texture_TreeFind(tree->left, name);
	else if (comp ==  1 && tree->right)
		return Texture_TreeFind(tree->right, name);

	return NULL;
}

int Texture_LoadFolder(SDL_Renderer * r, char * folder) {
	DIR * d;
	struct dirent * dir;

	d = opendir(folder);

	if (!d) return -1;

	int folder_length = strlen(folder);
	int count = 0;

	while ((dir = readdir(d)) != NULL) {
		if (dir->d_type == DT_REG) {
			int l = folder_length + 1 + strlen(dir->d_name);
			char * str = malloc((l+1) * sizeof(char));

			memcpy(str, folder, folder_length);
			str[folder_length] = '/';
			memcpy(str+folder_length+1, dir->d_name, strlen(dir->d_name)+1);

			int state = Texture_Load(r, str);
			//if (state)
			//	printf("Loaded %s\n", str);

			if (state) ++count;

			free(str);
		}
	}

	closedir(d);
	return count;
}

void Texture_FreeAll() {
	if (tex_search_tree) Texture_FreeTree(tex_search_tree);

	for (int i = 0; i < textures_loaded; ++i) {
		Texture_Free(textures + i);
	}

	textures_loaded = 0;
}

texture * Texture_Get(char * name) {
	for (int i = 0; i < textures_loaded; ++i) {
		// if name matches
		if (strcmp(name, textures[i].name) == 0)
			return textures + i;
	}

	return NULL;
}

texture * Texture_FastGet(char * name) {
	return Texture_TreeFind(tex_search_tree, name);	
}

int Texture_Load(SDL_Renderer * r, char * filename) {
	texture t;

	SDL_Surface * surface = IMG_Load(filename);
	if (surface == NULL) return 0;
	t.img = SDL_CreateTextureFromSurface(r, surface);
	SDL_FreeSurface(surface);
	if (t.img == NULL) return 0;

	SDL_QueryTexture(t.img, NULL, NULL, &t.w, &t.h);

	int fl = strlen(filename);
	int slash_index = fl-1; 
	for (; slash_index >= 0; --slash_index)
		if (filename[slash_index] == '/') break;

	int len = fl-slash_index;
	int s = sizeof(char) * (len+1);
	t.name = malloc(s);
	strcpy(t.name, filename+slash_index+1);

	textures[textures_loaded] = t;

	if (tex_search_tree)
		Texture_AddTree(tex_search_tree, textures + textures_loaded);
	else
		tex_search_tree = Texture_CreateTree(textures + textures_loaded);

	++textures_loaded;
	return 1;
}

void Texture_Free(texture * t) {
	if (t->name) free(t->name);
	if (t->img)  SDL_DestroyTexture(t->img);

	t->name = NULL;
	t->img  = NULL;
}
