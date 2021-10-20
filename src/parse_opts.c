#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mode.h"
#include "hash.h"


/* very funny magic numbers */
#define NO_NUMBERS  12144439425868588933U
#define NO_UPPER    8243583158179576629U
#define NO_LOWER    8243583158168875026U
#define NO_SYMBOLS  12144439432482481042U
#define NUMBER		7569865168420680U
#define ALPHA       229389837851557U
#define ALPHA_NUM   13783808862824347298U


/* check if a string is an actual zero, as in "0" or "000" */
static bool is_zero(char *str)
{
	const int len = strlen(str);

	for (int i = 0; i < len; i++)
	{
		if (str[i] != '0')
			return false;
	}

	return true;
}


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

			case NO_SYMBOLS: case ALPHA_NUM:
				m->symbols = false;
				break;
			
			case NUMBER:
				m->symbols = false;
				m->upper = false;
				m->lower = false;
				break;
			
			case ALPHA:
				m->symbols = false;
				m->numbers = false;
				break;
		
			default:
			{
				int arg = atoi(argv[i]);

				if (arg == 0 && !is_zero(argv[i]))
				{
					fprintf(stderr, "passor: error: %s is not a valid argument\n", argv[i]);
					exit(1);
				}
				else
				{
					m->length = arg;
				}
			}
		}
	}
}