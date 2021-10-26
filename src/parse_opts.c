#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mode.h"
#include "hash.h"


#define CHECK_BASE64_ENABLED \
	{ \
		if (base64_enabled == true) \
		{ \
			fprintf(stderr, "passor: error: base64 cannot be used with other arguments\n"); \
			exit(1); \
		} \
	}

/* very funny magic numbers */
#define NO_NUMBERS  12144439425868588933U
#define NO_UPPER     8243583158179576629U
#define NO_LOWER     8243583158168875026U
#define NO_SYMBOLS  12144439432482481042U
#define NUMBER          7569865168420680U
#define ALPHA            229389837851557U
#define ALPHA_NUM   13783808862824347298U
#define BASE64		    7569864675294820U
#define DONT_ALLOW  12143978086140558272U
#define NO_SPACES   13783827187913493800U


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
	bool base64_enabled = false;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] != '-')
		{
			for (int j = 0; j < strlen(argv[i]); j++)
			{
				switch (argv[i][j])
				{
					case 'N': CHECK_BASE64_ENABLED
						m->numbers = false;
						break;

					case 'U': CHECK_BASE64_ENABLED
						m->upper = false;
						break;

					case 'L': CHECK_BASE64_ENABLED
						m->lower = false;
						break;

					case 'S': CHECK_BASE64_ENABLED
						m->symbols = false;
						break;

					case '-':
						break;

					default:
						fprintf(stderr, "passor: error: %c is not a valid argument\n", argv[i][j]);
						exit(1);
				}
			}
		}
		else
		{
			switch (hash(argv[i]))
			{
				case NO_NUMBERS: CHECK_BASE64_ENABLED
					m->numbers = false;
					break;

				case NO_UPPER: CHECK_BASE64_ENABLED
					m->upper = false;
					break;

				case NO_LOWER: CHECK_BASE64_ENABLED
					m->lower = false;
					break;

				case NO_SYMBOLS: case ALPHA_NUM: CHECK_BASE64_ENABLED
					m->symbols = false;
					break;

				case NUMBER: CHECK_BASE64_ENABLED
					m->symbols = false;
					m->upper = false;
					m->lower = false;
					break;

				case ALPHA: CHECK_BASE64_ENABLED
					m->symbols = false;
					m->numbers = false;
					break;

				case DONT_ALLOW: CHECK_BASE64_ENABLED
					i++;
					strcat(m->characters_not_allowed, argv[i]);
					break;
				
				case NO_SPACES: CHECK_BASE64_ENABLED
					strcat(m->characters_not_allowed, " ");
				
				case 

				case BASE64:
					if (base64_enabled == true)
					{
						fprintf(stderr, "passor: error: base64 already set");
						exit(1);
					}

					base64_enabled = true;

					// characters not part of base64
					strcat(m->characters_not_allowed, "`~!@#$%^&*()_=-\\[]{}|;:'\",<.>? ");
					break;

				default:
				{
					int arg = atoi(argv[i]);

					// check if atoi() returned 0 and if the string is not a valid integer
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
}
