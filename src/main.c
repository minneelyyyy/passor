#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse_opts.h"
#include "fill_buff_rand.h"
#include "mode.h"


static char *program_name;


void help(int exit_status)
{
	fprintf(
		exit_status ? stderr : stdout,
		"Usage: %s [OPTION]... [LENGTH]\n"
		"generate a password / random string\n\n"

		"  -U, --no-upper        generate without uppercase characters\n"
		"  -L, --no-lower        generate without lowercase characters\n"
		"  -N, --no-numbers      generate without number characters\n"
		"  -S, --no-symbols      generate without other characters\n"
		"  --number              generate only a number\n"
		"  --alpha               generate only alphabetical characters\n"
		"  --alpha-num           the same as --no-symbols\n"
		"  --dont-allow <string> don't allow generation to use characters in this string\n"
		"  --base64              generate a base64 string\n"
		"  -s, --no-spaces       don't allow generation to use spaces\n\n",
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
			".lower                  = %d\n"
			".upper                  = %d\n"
			".numbers                = %d\n"
			".symbols                = %d\n"
			".characters_not_allowed = \"%s\"\n"
			".length                 = %d\n",
			options.lower, options.upper, options.numbers, options.symbols, options.characters_not_allowed, options.length
		);
	#endif

	if (options.length <= 0)
		return 0;

	#ifdef STACK_ONLY
		char output[options.length + 1];
		memset(output, 0x0, options.length + 1);
	#else
		char *output = calloc(options.length + 1, sizeof(char));
	#endif
	
	fill_buff_rand(output, options, random);

	puts(output);
}
