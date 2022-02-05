#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mode.h"
#include "exit_codes.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

static const char *all_chars = "!\"#$&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
static const char *uppercase_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char *lowercase_chars = "abcdefghijklmnopqrstuvwxyz";
static const char *symbol_chars = "!\"#$&'()*+,-./:;<=>?@[\\]^_`{|}~";
static const char *number_chars = "0123456789";

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

static void remove_chars(char *str, const char *chars)
{
	for (int i = 0; i < strlen(chars); i++)
	{
		str_chr_remove(str, chars[i]);
	}
}

static void get_usable_chars(char *str, struct mode m)
{
	strcpy(str, all_chars);

	static const char *base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789/+";
	static const char *base16_chars = "0123456789ABCDEF";

	switch (m.special)
	{
		case NONE:
			if (!m.upper)
				remove_chars(str, uppercase_chars);
			if (!m.lower)
				remove_chars(str, lowercase_chars);
			if (!m.symbols)
				remove_chars(str, symbol_chars);
			if (!m.numbers)
				remove_chars(str, number_chars);
			if (m.spaces)
				strcat(str, " ");
			break;
		
		case BASE64:
			strcpy(str, base64_chars);
			break;
		
		case BASE16:
			strcpy(str, base16_chars);
			break;
	}
	
	remove_chars(str, m.characters_not_allowed);
}

static char get_rand_char(char *str)
{
	return str[rand() % strlen(str)];
}

void fill_buff_rand(char *str, struct mode m)
{
	// seed random number generator
	#ifdef _WIN32
		SYSTEMTIME st;
		GetSystemTime(&st);

		srand(st.wMilliseconds);
	#else
		struct timeval tv;
		gettimeofday(&tv, NULL);

		srand(tv.tv_usec);
	#endif

	char usable_chars[256] = "";
	get_usable_chars(usable_chars, m);

	#ifdef DEBUG
		if (m.debug)
			fprintf(stderr, "\"%s\"\n", usable_chars);
	#endif

	if (!strcmp(usable_chars, ""))
	{
		fprintf(stderr, "passor: error: no printable characters\n");
		exit(NO_AVAILABLE_CHARACTERS);
	}

	for (int i = 0; i < m.length; i++)
		str[i] = get_rand_char(usable_chars);
}
