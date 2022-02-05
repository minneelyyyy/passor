#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse_opts.h"
#include "fill_buff_rand.h"
#include "mode.h"
#include "exit_codes.h"

#ifndef MAX_LENGTH
#	define MAX_LENGTH 100000
#endif

// stores the first argument in argv
//    ./passor 24 :  program_name = "./passor"
//    passor 24   :  program_name = "passor"
static char *program_name;

// prints help message which describes how to use the program
// then exits with exit_status
void help(int exit_status)
{
	fprintf(
		// if program exit status is an error val then print to stderr,
		// otherwise print to stdout
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
		program_name // this is the only real use of program_name
	);

	exit(exit_status);
}

int main(int argc, char *argv[])
{
	// set program_name to the first argument, the name of the program
	// this is safe because argv will always have at least one argument, no exception
	program_name = argv[0];

	// check if the user asks for help. we need to verify the argument count
	// because if we try to check argv[>0] and there arent that many arguments
	// the program will crash
	if (argc > 1 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")))
		help(0);

	// options for string generation, these are the defaults
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

	// fill the options struct with correct, user given values instead of defaults
	parse_opts(&options, argc, argv);

	#ifdef DEBUG
		// activate this debug info with the '--debug' command line option (only for debug builds)
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

	// exit the program if the user specifies <=0 as the length
	if (options.length <= 0)
		return 0;
	
	if (options.length > MAX_LENGTH)
	{
		fprintf(stderr, "passor: error: length specified too long (maximum: %d)\n", MAX_LENGTH);
		exit(SPECIFIED_LENGTH_OVER_MAX);
	}

	#ifdef HEAP_BUFFER
		char *output = malloc(options.length + 1);
	#else
		char output[options.length + 1];
	#endif

	output[options.length] = '\0';
	
	fill_buff_rand(output, options);

	puts(output);
}
