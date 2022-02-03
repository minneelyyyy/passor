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
		"generate a password / random string\n"
		"\n"
		"  -U, --no-upper            generate without uppercase characters\n"
		"  -L, --no-lower            generate without lowercase characters\n"
		"  -N, --no-numbers          generate without number characters\n"
		"  -S, --no-symbols          generate without other characters\n"
		"  -s, --spaces              allows generation to use spaces\n"
		"  --number                  generate only a number\n"
		"  --alpha                   generate only alphabetical characters\n"
		"  --alpha-num               the same as --no-symbols\n"
		"  --dont-allow <string>     don't allow generation to use characters in this string\n"
		"  -X, --base64              generate a base64 string\n"
		"  -x, --base16              generate a base16 string\n"
		"\n",
		program_name
	);

	exit(exit_status);
}

int main(int argc, char *argv[])
{
	program_name = argv[0];

	if (argc > 1 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")))
		help(0);

	/* options for string generation, these are the defaults */
	struct mode options = {
		.lower   = true,
		.upper   = true,
		.numbers = true,
		.symbols = true,
		.spaces  = false,
		.special = NONE,
		.debug   = false,
		.characters_not_allowed = "",
		.length  = 8
	};

	/* fill the options struct with correct, user given values instead of defaults */
	parse_opts(&options, argc, argv);

	#ifdef DEBUG
		if (options.debug)
		{
			printf(
				"===========================\n"
				".upper                  = %d\n"
				".lower                  = %d\n"
				".numbers                = %d\n"
				".symbols                = %d\n"
				".spaces                 = %d\n"
				".special                = %d\n"
				".debug                  = %d\n"
				".characters_not_allowed = \"%s\"\n"
				".length                 = %d\n",
				options.lower, options.upper, options.numbers, options.symbols, options.spaces, options.special, options.debug, options.characters_not_allowed, options.length
			);
		}
	#endif

	if (options.length <= 0)
		return 0;

	#ifdef HEAP_BUFFER
		char *output = malloc(options.length + 1);
	#else
		char output[options.length + 1];
		output[options.length] = '\0';
	#endif
	
	fill_buff_rand(output, options);

	puts(output);
}
