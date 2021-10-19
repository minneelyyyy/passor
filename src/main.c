#include <stdio.h>
#include <stdbool.h>
#include "parse_opts.h"
#include "hash.h"

static char *program_name;
static const char *official_name = "passor";

void help(int exit_status)
{
	fprintf(
		exit_status ? stderr : stdout,
		"%s - password / random string generator\n"
		"USAGE: %s\n"
		"or:    %s [options...] [count]\n",
		official_name,
		program_name, program_name
	);

	exit(exit_status);
}

int main(int argc, char *argv[])
{
	/* file handle for the random byte generator */
	FILE *random = fopen("/dev/urandom", "r");

	// if /dev/urandom cannot be opened
	if (!random)
	{
		fprintf(stderr, "Failed to open /dev/urandom. exiting.\n");
		return 1;
	}

	program_name = argv[0];

	/* print help and exit if help is called */
	if (argc > 1 && !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))
		help(0);

	/* options for string generation, these are the defaults */
	struct mode options = {
		.lower   = true,
		.upper   = true,
		.numbers = true,
		.symbols = true,
		.length  = 8
	};

	// fill the options struct with correct, user given values instead of defaults
	parse_opts(&options, arc, argv);
}