#include "core.h"

int main(int argc, char ** argv) {
	int code = Core_Init(argc, argv);

	if (code == 0)
		Core_Main();

	Core_Quit();
	return code;
}
