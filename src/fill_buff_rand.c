#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mode.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define inrange(c, a, z) a <= c && c <= z

static char get_rand_char(struct mode m)
{
	static const char *base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789/+";
	static const char *base16_chars = "0123456789ABCDEF";

	switch (m.spec)
	{
		case BASE64:
			return base64_chars[rand() % strlen(base64_chars)];

		case BASE16:
			return base16_chars[rand() % strlen(base16_chars)];
		
		case NONE:
			for (;;)
			{
				char c = rand() % ('~' - ' ' + 1) + ' ';

				if (!m.upper && inrange(c, 'A', 'Z'))
					continue;

				if (!m.lower && inrange(c, 'a', 'z'))
					continue;

				if (!m.numbers && inrange(c, '0', '9'))
					continue;

				if (!m.symbols && !!strchr("`~!@#$%^&*()-=_+[]{}\\|;:'\",<.>/?", c))
					continue;

				if (!m.spaces && c == ' ')
					continue;

				if (!!strchr(m.characters_not_allowed, c))
					continue;

				return c;
			}
		
		default:
			return 0;
	}
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

	for (int i = 0; i < m.length; i++)
		str[i] = get_rand_char(m);
}
