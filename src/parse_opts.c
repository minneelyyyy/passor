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
#define BASE64	        7569864675294820U
#define DONT_ALLOW  12143978086140558272U
#define SPACES          7569865357737886U


#define check_set(bool_set, string)                                               \
	{                                                                         \
		if (bool_set == true)                                             \
		{                                                                 \
			fprintf(stderr, "passor: error: %s set twice\n", string); \
			exit(1);                                                  \
		}                                                                 \
		bool_set = true;                                                  \
	}


/* check if a string is a zero, as in "0" or "000" */
static bool is_zero(char *str)
{
	const int len = strlen(str);

	for (int i = 0; i < len; i++)
		if (str[i] != '0')
			return false;

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

void parse_opts(struct mode *m, int argc, char *argv[])
{
	struct set_opts {
		bool no_upper   : 1;
		bool no_lower   : 1;
		bool no_numbers : 1;
		bool no_symbols : 1;
		bool number     : 1;
		bool alpha      : 1;
		bool alpha_num  : 1;
		bool base64     : 1;
		bool dont_allow : 1;
		bool spaces     : 1;
	} set = (struct set_opts) { 0 };

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] != '-')
		{
			for (int j = 1; j < strlen(argv[i]); j++)
			{
				switch (argv[i][j])
				{
					case 'U':
						check_set(set.no_upper, "flag U");

						m->upper = false;
						break;

					case 'L':
						check_set(set.no_lower, "flag L");

						m->lower = false;
						break;

					case 'N':
						check_set(set.no_numbers, "flag N");

						m->numbers = false;
						break;

					case 'S':
						check_set(set.no_symbols, "flag S");

						m->symbols = false;
						break;

					case 's':
						check_set(set.spaces, "flag s");

						str_chr_remove(m->characters_not_allowed, ' ');
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
					check_set(set.no_upper, "--no-upper");

					m->upper = false;
					break;

				case NO_LOWER:
					check_set(set.no_lower, "--no-lower");

					m->lower = false;
					break;

				case NO_NUMBERS:
					check_set(set.no_numbers, "--no-numbers");

					m->numbers = false;
					break;

				case NO_SYMBOLS:
					check_set(set.no_symbols, "--no-symbols");

					m->symbols = false;
					break;

				case NUMBER:
					check_set(set.number, "--number");

					m->symbols = false;
					m->upper = false;
					m->lower = false;
					break;

				case ALPHA:
					check_set(set.alpha, "--alpha");

					m->symbols = false;
					m->numbers = false;
					break;

				case ALPHA_NUM:
					check_set(set.alpha_num, "--alpha-num");

					m->symbols = false;
					break;

				case BASE64:
					check_set(set.base64, "--base64");

					// characters not part of base64
					strcat(m->characters_not_allowed, "`~!@#$%^&*()-=_[]{}\\|;:'\",<.>?");
					break;

				case DONT_ALLOW:
					check_set(set.dont_allow, "--dont-allow");

					i++;
					strcat(m->characters_not_allowed, argv[i]);
					break;

				case SPACES:
					check_set(set.spaces, "--spaces");

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

	#ifdef DEBUG
		printf(
			"===============\n"
			".no_upper   = %d\n"
			".no_lower   = %d\n"
			".no_numbers = %d\n"
			".no_symbols = %d\n"
			".number     = %d\n"
			".alpha      = %d\n"
			".alpha_num  = %d\n"
			".base64     = %d\n"
			".dont_allow = %d\n"
			".spaces     = %d\n",
			set.no_upper, set.no_lower, set.no_numbers, set.no_symbols, set.number, set.alpha, set.alpha_num, set.base64, set.dont_allow, set.spaces
		);
	#endif
}
