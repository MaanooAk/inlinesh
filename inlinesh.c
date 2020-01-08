
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===

int handle(char* file);
int command(char* line, size_t size);

#define propagate(x) \
	int error = x; \
	if (error != 0) return error;

#define error(error, message, x) \
	fprintf(stderr, "inlinebash: " message "\n", x); \
	return error;

// ===

int handle(char* file) {

	FILE *f = file == 0 ? stdin : fopen(file, "r");
	if (f == 0) {
		error(0, "file not found: %s", file);
	}

	char *buffer; // TODO malloc
	size_t size = 0, count = 0;

	while ((count = getline(&buffer, &size, f)) != -1) {

		if (buffer[0] == '#') {
			fflush(stdout); // keep the order
			propagate(command(buffer, size));
		} else {
			printf(buffer);
		}
	}

	fclose(f);
	if (buffer) free(buffer);

}

int command(char* line, size_t size) {

	char origin[size]; // this will be mutatted
	strcpy(origin, line + 1); // skip the #

	char *tmp = origin;
	char *command = strsep(&tmp, " ");
	char *param = strsep(&tmp, "\n");

	if (strcmp(command, "import") == 0) {
		handle(param);
	} else {
		system(line + 1); // skip the #
	}
}

int main(int argc, char *argv[]) {

	if (argc == 1) {
		propagate(handle(0)); // stdin

	} else for (int i=1; i<argc; i++) {
		char* param = argv[i];

		if (param[0] == '-') {
			error(102, "option not supported: %s", param);

		} else {
			propagate(handle(param));
		}
	}

	return 0;
}

