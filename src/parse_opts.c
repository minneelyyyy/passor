#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mode.h"
#include "hash.h"
#include "exit_codes.h"

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
#define BASE64_M   1995964004
#define BASE16_M   1995963841
#define DEBUG_M    4227798182


#define special_check_special()                                                                \
	{                                                                                      \
		if (m->special)                                                                \
		{                                                                              \
			fprintf(stderr, "passor: error: cannot set multiple special flags\n"); \
			exit(MULTIPLE_SPECIAL_FLAGS);                                          \
		}                                                                              \
	}

#define check_special(str)                                                                               \
	{                                                                                                \
		if (m->special)                                                                          \
		{                                                                                        \
			fprintf(stderr, "passor: error: cannot set %s when special flag is set\n", str); \
			exit(SPECIAL_FLAG_SET);                                                          \
		}                                                                                        \
	}

#define check_bool_set_twice(b, def, str)                                      \
	{                                                                      \
		if (b != def)                                                  \
		{                                                              \
			fprintf(stderr, "passor: error: %s set twice\n", str); \
			exit(ARGUMENT_SET_TWICE);                              \
		}                                                              \
	}

#define common_flag_check(b, def, str)             \
	{                                          \
		check_special(str);                \
		check_bool_set_twice(b, def, str); \
	}

/* check if a string is a zero, as in "0" or "000" */
static bool is_zero(char *str)
{
	for (int i = 0; i < strlen(str); i++)
		if (str[i] != '0')
			return false;

	return true;
}

static bool is_long_option(const char *option)
{
	return strlen(option) > 1 ? option[0] == '-' && option[1] == '-' : false;
}

static bool is_flag(const char *flags)
{
	return flags[0] == '-';
}

// returns amount of options you should skip
//   0 for options that dont have any arguments (--no-upper)
//   1 for options that have 1 argument (--dont-allow)
static int parse_long_option(struct mode *m, char **args, int args_len)
{
	int arg_skip_count = 0;         

	switch (hash(args[0]))
	{
		case NO_UPPER: common_flag_check(m->upper, true, "--no-upper")
			m->upper = false;
			break;

		case NO_LOWER: common_flag_check(m->lower, true, "--no-lower")
			m->lower = false;
			break;

		case NO_NUMBERS: common_flag_check(m->numbers, true, "--no-numbers")
			m->numbers = false;
			break;

		case NO_SYMBOLS: common_flag_check(m->symbols, true, "--no-symbols")
			m->symbols = false;
			break;

		case SPACES: common_flag_check(m->spaces, false, "--spaces")
			m->spaces = true;
			break;

		case NUMBER:
			check_special("--number")

			m->upper = false;
			m->lower = false;
			m->symbols = false;
			break;

		case ALPHA:
			check_special("--alpha")

			m->symbols = false;
			m->numbers = false;
			break;

		case ALPHA_NUM:
			check_special("--alpha-num")

			m->symbols = false;
			break;

		case DONT_ALLOW:
			check_special("--dont-allow")

			if (args_len == 1)
			{
				fprintf(stderr, "passor: error: no argument given for multi-argument parameter %s\n", args[0]);
				exit(LAST_ARGUMENT_REQUIRES_ARGUMENT);
			}

			arg_skip_count = 1;
			strncat(m->characters_not_allowed, args[1], SIZEOF_NA_CHARS - 1 - strlen(m->characters_not_allowed));
			break;

		case BASE64_M:
			special_check_special();

			m->special = BASE64;
			break;
		
		case BASE16_M:
			special_check_special();

			m->special = BASE16;
			break;
		
		#ifdef DEBUG
			case DEBUG_M: check_bool_set_twice(m->debug, false, "--debug")
				m->debug = true;
				break;
		#endif

		default:
		{
			fprintf(stderr, "passor: error: %s is not a valid argument\n", args[0]);
			exit(INVALID_ARGUMENT);
		}
	}

	return arg_skip_count;
}

static void parse_flags(struct mode *m, char *flags)
{
	for (int i = 0; i < strlen(flags); i++)
	{
		switch (flags[i])
		{
			case '-':
				// don't raise this error if it's the first character
				// or if the string is longer than 1 chararacter
				if (i > 0 || strlen(flags) == 1)
				{
					fprintf(stderr, "passor: error: %c is not a valid flag\n", flags[i]);
					exit(INVALID_ARGUMENT);
				}
				break;

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
				special_check_special();

				m->special = BASE64;
				break;

			case 'x':
				special_check_special();

				m->special = BASE16;
				break;

			default:
				fprintf(stderr, "passor: error: %c is not a valid flag\n", flags[i]);
				exit(INVALID_ARGUMENT);
		}
	}
}

void parse_opts(struct mode *m, int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		int int_value_of_argument = atoi(argv[i]);
		bool argument_is_int = (!!int_value_of_argument ? !!int_value_of_argument : is_zero(argv[i]));

		if (argument_is_int)
		{
			m->length = int_value_of_argument;
		}
		else if (is_long_option(argv[i]))
		{
			// add return to i because some arguments take a second argument, this function
			// returns the amount of arguments to skip
			i += parse_long_option(m, argv + i, argc - i);
		}
		else if (is_flag(argv[i]))
		{
			parse_flags(m, argv[i]);
		}
		else
		{
			fprintf(stderr, "passor: error: %s is not a valid argument\n", argv[i]);
			exit(INVALID_ARGUMENT);
		}
	}
}
