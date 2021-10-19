#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mode.h"
#include "hash.h"


/* very funny magic numbers */
#define NO_NUMBERS  12144439425868588933U
#define NO_UPPER    249805550247202768U
#define NO_LOWER    8243583158168875026U
#define NO_SYMBOLS  12144439432482481042U


void parse_opts(struct mode *m, int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		switch (hash(argv[i]))
		{
			case NO_NUMBERS:
				m->numbers = false;
				break;

			case NO_UPPER:
				m->upper = false;
				break;

			case NO_LOWER:
				m->lower = false;
				break;

			case NO_SYMBOLS:
				m->symbols = false;
				break;
		
			default:
				m->length = atoi(argv[i]);
		}
	}
}