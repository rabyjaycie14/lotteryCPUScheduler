/**
 *
 */

#include "types.h"
#include "user.h"

int main(int argc, char* argv[]) {
	if(argc < 3) {
		/* 1 is for standard output, 0 for console output */
		printf(1, "Usage:\n hello <first name> <last name>\n");
		exit();
	} else {
		printf(1, "Hello, my name is %s %s\n", argv[1], argv[2]);
		exit();
	}
}
