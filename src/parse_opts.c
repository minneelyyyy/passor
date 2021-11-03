#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mode.h"
#include "hash.h"


/* very funny magic numbers */
#define NO_UPPER   3148856117
#define NO_LOWER   3138154514
#define NO_NUMBERS 1462069125
#define NO_SYMBOLS 3780993938
#define SPACES     2678407070
#define NUMBER     2489089864
#define ALPHA      4224506789
#define ALPHA_NUM  2290529954
#define DONT_ALLOW 1351171008
#define BASE64     1995964004
#define BASE16     1995963841


#define special_check_special()                                                                \
	{                                                                                      \
		if (m->special)                                                                \
		{                                                                              \
			fprintf(stderr, "passor: error: cannot set multiple special flags\n"); \
			exit(1);                                                               \
		}                                                                              \
	}

#define check_special(str)                                                                               \
	{                                                                                                \
		if (m->special)                                                                          \
		{                                                                                        \
			fprintf(stderr, "passor: error: cannot set %s when special flag is set\n", str); \
			exit(1);                                                                         \
		}                                                                                        \
	}

#define check_bool_set_twice(b, default, str)                                  \
	{                                                                      \
		if (b != default)                                              \
		{                                                              \
			fprintf(stderr, "passor: error: %s set twice\n", str); \
			exit(1);                                               \
		}                                                              \
	}

#define common_flag_check(b, default, str)             \
	{                                              \
		check_special(str);                    \
		check_bool_set_twice(b, default, str); \
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
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] != '-')
		{
			for (int j = 1; j < strlen(argv[i]); j++)
			{
				switch (argv[i][j])
				{
					case 'U': common_flag_check(m->upper, true, "flag U")
						m->upper = false;
						break;

					case 'L': common_flag_check(m->lower, true, "flag L")
						m->lower = false;
						break;

					case 'N': common_flag_check(m->numbers, true, "flag N")
						m->numbers = false;
						break;

					case 'S': common_flag_check(m->symbols, true, "flag S")
						m->symbols = false;
						break;

					case 's': common_flag_check(m->spaces, false, "flag s")
						m->spaces = true;
						break;

					case 'X':
						check_bool_set_twice(m->base64, false, "flag X");
						special_check_special();

						m->basic = false;
						m->base64 = true;
						break;

					case 'x':
						check_bool_set_twice(m->base16, false, "flag x");
						special_check_special();

						m->basic = false;
						m->base16 = true;
						break;

					default:
						fprintf(stderr, "passor: error: %c is not a valid flag\n", argv[i][j]);
						exit(1);
				}
			}
		}
		else
		{
			switch (hash(argv[i]))
			{
				case NO_UPPER: common_flag_check(m->upper, true, "--no-upper")
					m->upper = false;
					break;

				case NO_LOWER: common_flag_check(m->upper, true, "--no-lower")
					m->upper = false;
					break;

				case NO_NUMBERS: common_flag_check(m->upper, true, "--no-numbers")
					m->upper = false;
					break;

				case NO_SYMBOLS: common_flag_check(m->upper, true, "--no-symbols")
					m->upper = false;
					break;

				case SPACES: common_flag_check(m->spaces, false, "--spaces")
					m->spaces = true;
					break;

				case NUMBER:
					check_special("--number");

					m->upper = false;
					m->lower = false;
					m->symbols = false;
					break;

				case ALPHA:
					check_special("--alpha");

					m->symbols = false;
					m->numbers = false;
					break;

				case ALPHA_NUM:
					check_special("--alpha-num");

					m->symbols = false;
					break;

				case DONT_ALLOW:
					check_special("--dont-allow")

					i++;
					strcat(m->characters_not_allowed, argv[i]);
					break;

				case BASE64:
					check_bool_set_twice(m->base64, false, "--base64");
					special_check_special();

					m->basic = false;
					m->base64 = true;
					break;
				
				case BASE16:
					check_bool_set_twice(m->base16, false, "--base64");
					special_check_special();

					m->basic = false;
					m->base16 = true;
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
