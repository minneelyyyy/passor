#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse_opts.h"
#include "fill_buff_rand.h"


static char *program_name;


void help(int exit_status)
{
	fprintf(
		exit_status ? stderr : stdout,
		"Usage: %s [OPTION]... [LENGTH]\n"
		"generate a password / random string\n\n"

		"  --no-upper          generate without uppercase characters\n"
		"  --no-lower          generate without lowercase characters\n"
		"  --no-numbers        generate without number characters\n"
		"  --no-symbols        generate without other characters\n\n",
		program_name
	);

	exit(exit_status);
}


int main(int argc, char *argv[])
{
	/* file handle for the random byte generator */
	FILE *random = fopen("/dev/urandom", "r");

	if (!random)
	{
		fprintf(stderr, "Failed to open /dev/urandom. exiting.\n");
		return 1;
	}

	program_name = argv[0];

	if (argc > 1 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")))
		help(0);

	/* options for string generation, these are the defaults */
	struct mode options = {
		.lower   = true,
		.upper   = true,
		.numbers = true,
		.symbols = true,
		.length  = 8
	};

	/* fill the options struct with correct, user given values instead of defaults */
	parse_opts(&options, argc, argv);

	#ifdef DEBUG
	printf(
		".lower   = %d\n"
		".upper   = %d\n"
		".numbers = %d\n"
		".symbols = %d\n"
		".length  = %d\n",
		options.lower, options.upper, options.numbers, options.symbols, options.length
	);
	#endif

	if (options.length <= 0)
		return 0;

	char output[options.length + 1];
	memset(output, 0x0, options.length + 1);

	fill_buff_rand(output, options, random);

	puts(output);
}
