#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mode.h"
#include "hash.h"

void parse_opts(struct mode *m, int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		switch (hash(argv[i]))
		{
			case 1462069125:
				m->numbers = false;
				break;

			case 224907216:
				m->upper = false;
				break;

			case 3138154514:
				m->lower = false;
				break;

			case 3780993938:
				m->symbols = false;
				break;
		
			default:
			{
				unsigned int count = atoi(argv[i]);

				m->length = count;
			}
		}
	}
}