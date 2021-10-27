#include <stdio.h>
#include <string.h>
#include "mode.h"

#define BASE64 0b01
#define BASE16 0b10

#define inrange(c, a, z) a <= c && c <= z

static char get_rand_char(struct mode m, FILE *random)
{
	static const char *base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789/+";
	static const char *base16_chars = "0123456789ABCDEF";

	if (m.special)
	{
		switch (m.special)
		{
			case BASE64:
				return base64_chars[getc(random) % strlen(base64_chars)];

			case BASE16:
				return base16_chars[getc(random) % strlen(base16_chars)];
		}
	}
	else
	{
		for (;;)
		{
			char c = getc(random) % ('~' - ' ' + 1) + ' ';

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
	}

	return 0;
}

void fill_buff_rand(char *str, struct mode m, FILE *random)
{
	for (int i = 0; i < m.length; i++)
		str[i] = get_rand_char(m, random);
}
