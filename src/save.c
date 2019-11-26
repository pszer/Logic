#include "save.h"
#include "display.h"

char SAVE_ERROR[64] = "";

int Save_LoadFile(char * fname) {

	char * full_path = malloc(
		sizeof(char)*(strlen(SAVE_PATH) + strlen(fname))+1);
	strcpy(full_path, SAVE_PATH);
	strcat(full_path, fname);

	FILE * f = fopen(full_path, "r");
	if (!f) {
		strcpy(SAVE_ERROR, "Cannot find file.");
		return 0;
	}

	uint32_t sig;
	fread(&sig, sizeof(sig), 1, f);

	if (sig != SAVE_SIG) {
		strcpy(SAVE_ERROR, "Wrong file signature.");
		return 0;
	}

	component * comps_temp = NULL;
	wire      * wires_temp = NULL;

	int i;
	uint32_t compc, wirec;
	fread(&compc, sizeof(compc), 1, f);
	fread(&wirec, sizeof(wirec), 1, f);

	if (ferror(f)) {
		strcpy(SAVE_ERROR, "Error reading file.");
		goto err;
	}

	comps_temp = malloc(sizeof(component) * compc * 2);
	wires_temp = malloc(sizeof(wire) * wirec * 2);

	if (!comps_temp || !wires_temp) {
		strcpy(SAVE_ERROR, "malloc error");
	}

	for (i = 0; i < compc; ++i) {
		if (Save_LoadComponent(comps_temp+i, f) == 0) {
			strcpy(SAVE_ERROR, "Error loading components.");
			goto err;
		}
	}

	for (i = 0; i < wirec; ++i) {
		if (Save_LoadWire(wires_temp+i, f, comps_temp, compc) == 0) {
			strcpy(SAVE_ERROR, "Error loading wiring.");
			goto err;
		}
	}

	TRANSFORM_X = 0;
	TRANSFORM_Y = 0;

	// delete current components
	Logic_FreeComponents();
	Logic_FreeWires();

	comps = comps_temp;
	wires = wires_temp;

	comp_count = compc;
	wire_count = wirec;

	comp_size = compc*2;
	wire_size = wirec*2;

	Logic_UpdateAllWirePos();

	fclose(f);
	free(full_path);
	return 1;
err:
	if (comps_temp) free(comps_temp);
	if (wires_temp) free(wires_temp);
	free(full_path);
	fclose(f);
	return 0;
}

int Save_SaveFile(char * fname) {
	char * full_path = malloc(
		sizeof(char)*(strlen(SAVE_PATH) + strlen(fname))+1);
	strcpy(full_path, SAVE_PATH);
	strcat(full_path, fname);
	
	int rewrite = 0;
	FILE * f;

	// first check if file exists
	f = fopen(full_path, "r");
	if (f) {
		rewrite = 1;
		fclose(f);
	}
	
	f = fopen(rewrite ? SAVE_TEMP : full_path, "w");
	if (!f) {
		strcpy(SAVE_ERROR, "Cannot open file for writing.");
		free(full_path);
		return 0;
	}

	// wire signature
	uint32_t sig = SAVE_SIG;
	fwrite(&sig, sizeof(sig), 1, f);

	// write counts
	uint32_t cc = comp_count, wc = wire_count;
	fwrite(&cc, sizeof(cc), 1, f);
	fwrite(&wc, sizeof(wc), 1, f);

	if (ferror(f)) goto err;

	int i;
	for (i = 0; i < comp_count; ++i) {
		if (Save_WriteComponent(comps + i, f) == 0) {
			goto err;
		}
	}

	for (i = 0; i < wire_count; ++i) {
		if (Save_WriteWire(wires + i, f) == 0) {
			goto err;
		}
	}

	if (ferror(f)) goto err;

	fclose(f);
	if (rewrite) {
		remove(full_path);
		rename(SAVE_TEMP, full_path);
	}

	free(full_path);
	return 1;
err:
	strcpy(SAVE_ERROR, "I/O error.");
	free(full_path);
	fclose(f);
	return 0;
}

int Save_WriteComponent(component* c, FILE* f) {
	uint16_t len = strlen(c->name);

	// write string length (not including \0)
	fwrite(&len, sizeof(len), 1, f);

	// write string
	fwrite(c->name, sizeof(char), len, f);

	uint32_t x = c->x - TRANSFORM_X, y = c->y - TRANSFORM_Y;
	// write position
	fwrite(&x, sizeof(x), 1, f);
	fwrite(&y, sizeof(x), 1, f);

	if (ferror(f)) return 0;
	return 1;
}

int Save_WriteWire(wire* w, FILE* f) {
	uint32_t c1i = w->c1 - comps,
	         c2i = w->c2 - comps;
	int32_t  n1  = w->n1,
	         n2  = w->n2;
	uint8_t  parity = w->parity;

	fwrite(&c1i, sizeof(c1i), 1, f);
	fwrite(&c2i, sizeof(c2i), 1, f);
	fwrite(&n1,  sizeof(n1), 1, f);
	fwrite(&n2,  sizeof(n2), 1, f);
	fwrite(&parity, sizeof(parity), 1, f);

	if (ferror(f)) return 0;
	return 1;
}

int Save_LoadComponent(component* c, FILE* f) {
	uint16_t namel;
	// read comp name length
	fread(&namel, sizeof(namel), 1, f);

	char * str = malloc((namel+1) * sizeof(char));
	if (!str) return 0;
	fread(str, sizeof(char), namel, f);
	str[namel] = '\0';

	int i;
	for (i = 0; i < COMP_DEF_COUNT; ++i) {
		if (strcmp(COMP_DEFS[i]->name, str) == 0) {
			*c = *COMP_DEFS[i];
			break;
		}
	}

	// if no component found
	if (i == COMP_DEF_COUNT) {
		char name[33];
		char* c;
		for (c = str; *c && c-str<32; ++c) {
			name[c-str]=*c;
		}
		name[c-str]='\0';

		strcpy(SAVE_ERROR, "Unknown component ");
		strcat(SAVE_ERROR, name);
		return 0;
	}

	uint32_t x,y;
	fread(&x, sizeof(x), 1, f);
	fread(&y, sizeof(y), 1, f);
	c->x = x;
	c->y = y;

	free(str);

	if (ferror(f)) return 0;
	return 1;
}

int Save_LoadWire(wire* w, FILE* f, component* comps, int compc) {
	uint32_t c1i, c2i;
	int32_t n1, n2;
	uint8_t parity;

	fread(&c1i, sizeof(c1i), 1, f);
	fread(&c2i, sizeof(c2i), 1, f);
	fread(&n1, sizeof(n1), 1, f);
	fread(&n2, sizeof(n2), 1, f);
	fread(&parity, sizeof(parity), 1, f);

	if (c1i >= compc || c2i >= compc) {
		return 0;
	}

	if ((n1 >= MAX_IN || n1 < -MAX_IN) ||
	    (n2 >= MAX_IN || n2 < -MAX_IN))
	{
		return 0;
	}

	w->c1 = comps + c1i;
	w->c2 = comps + c2i;
	w->n1 = n1;
	w->n2 = n2;
	w->parity = parity%4;

	if (ferror(f)) return 0;
	return 1;
}
