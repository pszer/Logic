#include "tex.h"

int textures_loaded=0;
texture textures[MAX_TEXTURES];

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
	memcpy(t.name, filename+slash_index+1, s);

	textures[textures_loaded] = t;

	++textures_loaded;
	return 1;
}

void Texture_Free(texture * t) {
	if (t->name) free(t->name);
	if (t->img)  SDL_DestroyTexture(t->img);

	t->name = NULL;
	t->img  = NULL;
}
