#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mode.h"
#include "hash.h"


/* very funny magic numbers */
#define NO_UPPER     8243583158179576629U
#define NO_LOWER     8243583158168875026U
#define NO_NUMBERS  12144439425868588933U
#define NO_SYMBOLS  12144439432482481042U
#define NUMBER          7569865168420680U
#define ALPHA            229389837851557U
#define ALPHA_NUM   13783808862824347298U
#define BASE64		    7569864675294820U
#define DONT_ALLOW  12143978086140558272U
#define SPACES          7569865357737886U


#define CHECK_SET(bool_set, string)                                   \
	{                                                                 \
		if (bool_set == true)                                         \
		{                                                             \
			fprintf(stderr, "passor: error: %s set twice\n", string); \
			exit(1);                                                  \
		}                                                             \
		bool_set = true;                                              \
	}


/* check if a string is a zero, as in "0" or "000" */
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

static void str_chr_remove(char *str, char c)
{
	while (*str != c && *str != '\0')
		str++;

	while (*str != '\0')
	{
		*str = *(str + 1);
		str++;
	}
}

struct options_set {
	bool no_upper: 1;
	bool no_lower: 1;
	bool no_numbers: 1;
	bool no_symbols: 1;
	bool number: 1;
	bool alpha: 1;
	bool alpha_num: 1;
	bool base64: 1;
	bool dont_allow: 1;
	bool spaces: 1;
};

void parse_opts(struct mode *m, int argc, char *argv[])
{
	struct options_set set = {
		.no_upper   = false,
		.no_lower   = false,
		.no_numbers = false,
		.no_symbols = false,
		.number     = false,
		.alpha      = false,
		.alpha_num  = false,
		.base64     = false,
		.dont_allow = false,
		.spaces     = false,
	};

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] != '-')
		{
			for (int j = 0; j < strlen(argv[i]); j++)
			{
				switch (argv[i][j])
				{
					case 'U':
						CHECK_SET(set.no_upper, "flag U");

						m->upper = false;
						break;

					case 'L':
						CHECK_SET(set.no_lower, "flag L");

						m->lower = false;
						break;

					case 'N':
						CHECK_SET(set.no_numbers, "flag N");

						m->numbers = false;
						break;

					case 'S':
						CHECK_SET(set.no_symbols, "flag S");

						m->symbols = false;
						break;

					case 's':
						CHECK_SET(set.spaces, "flag s");

						str_chr_remove(m->characters_not_allowed, ' ');
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
				case NO_UPPER:
					CHECK_SET(set.no_upper, "--no-upper");

					m->upper = false;
					break;

				case NO_LOWER:
					CHECK_SET(set.no_lower, "--no-lower");

					m->lower = false;
					break;

				case NO_NUMBERS:
					CHECK_SET(set.no_numbers, "--no-numbers");

					m->numbers = false;
					break;

				case NO_SYMBOLS:
					CHECK_SET(set.no_symbols, "--no-symbols");

					m->symbols = false;
					break;

				case NUMBER:
					CHECK_SET(set.number, "--number");

					m->symbols = false;
					m->upper = false;
					m->lower = false;
					break;

				case ALPHA:
					CHECK_SET(set.alpha, "--alpha");

					m->symbols = false;
					m->numbers = false;
					break;

				case ALPHA_NUM:
					CHECK_SET(set.alpha_num, "--alpha-num");

					m->symbols = false;
					break;

				case BASE64:
					CHECK_SET(set.base64, "--base64");

					// characters not part of base64
					strcat(m->characters_not_allowed, "`~!@#$%^&*()-=_[]{}\\|;:'\",<.>?");
					break;

				case DONT_ALLOW:
					CHECK_SET(set.dont_allow, "--dont-allow");

					i++;
					strcat(m->characters_not_allowed, argv[i]);
					break;

				case SPACES:
					CHECK_SET(set.spaces, "--spaces");

					// remove space
					str_chr_remove(m->characters_not_allowed, ' ');
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
