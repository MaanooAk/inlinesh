
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===

int handle(char* file);
int handle_once(char* file);
int command(char* line, size_t size);

char command_char = '#';

int error_option = 101;
int error_file = 102;

#define propagate(x) \
	int error = x; \
	if (error != 0) return error;

#define error(error, message, x) \
	fprintf(stderr, "inlinesh: " message "\n", x); \
	return error;

// ===

int handle(char* file) {

	FILE *f = file == 0 ? stdin : fopen(file, "r");
	if (f == 0) {
		error(error_file, "file not found: %s", file);
	}

	char *buffer; // this will be allocated by getline
	size_t size = 0, count = 0;

	while ((count = getline(&buffer, &size, f)) != -1) {

		if (buffer[0] == command_char) {
			fflush(stdout); // keep the order
			propagate(command(buffer, size));
		} else {
			printf("%s", buffer);
		}
	}

	fclose(f);
	if (buffer) free(buffer);

}

typedef struct strset {
	struct strset* next;
	char value[];
} strset;

strset* strset_new(const char* value, strset* next) {
	strset* set = malloc(sizeof(strset) + (strlen(value) + 1) * sizeof(char));
	set->next = next;
	strcpy(set->value, value);
	return set;
}

int strset_add(strset** set, char* element) {
	strset* head = *set;
	while (head != 0) {
		if (strcmp(head->value, element) == 0) return 0;
		head = head->next;
	}
	*set = strset_new(element, *set);
	return 1;
}

int handle_once(char *file) {

	static strset* set = 0;
	if (!strset_add(&set, file)) return 0;

	return handle(file);
}

int command(char* line, size_t size) {

	char origin[size]; // this will be altered by strsep
	strcpy(origin, line + 1); // skip the command_char

	char *tmp = origin;
	char *command = strsep(&tmp, " ");
	char *param = strsep(&tmp, "\n");

	if (strcmp(command, "import") == 0) {
		return handle(param);
	} else if (strcmp(command, "include") == 0) {
		return handle_once(param);
	} else {
		system(line + 1); // skip the command_char
	}
}

int option(int argc, char *argv[], int *index) {
	char *name = argv[*index];

	if (strcmp(name, "-f") == 0 || strcmp(name, "--force") == 0) {
		error_file = 0;

	} else if (strcmp(name, "-c") == 0 || strcmp(name, "--command") == 0) {
		if (*index + 1 >= argc) {
			error(error_option, "option missing argument: %s", name);
		}
		command_char = argv[++(*index)][0];

	} else {
		error(error_option, "option not supported: %s", name);
	}
}

int main(int argc, char *argv[]) {
	int files = 0;

	for (int i=1; i<argc; i++) {
		char* param = argv[i];

		if (param[0] == '-') {
			propagate(option(argc, argv, &i));

		} else {
			files++;
			propagate(handle(param));
		}
	}

	if (files == 0) {
		// no files where provided
		propagate(handle(0)); // stdin
	}

}

