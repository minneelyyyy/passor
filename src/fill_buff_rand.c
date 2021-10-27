#include <stdio.h>
#include <string.h>
#include "mode.h"

#define inrange(c, a, z) a <= c && c <= z

static char get_rand_char(struct mode m, FILE *random)
{
	char c;

	for (;;)
	{
		c = getc(random) % ('~' - ' ' + 1) + ' ';

		if (!m.upper && inrange(c, 'A', 'Z'))
			continue;

		if (!m.lower && inrange(c, 'a', 'z'))
			continue;

		if (!m.numbers && inrange(c, '0', '9'))
			continue;

		if (!m.symbols && !!strchr("`~!@#$%^&*()-=_+[]{}\\|;:'\",<.>/?", c))
			continue;

		if (!!strchr(m.characters_not_allowed, c))
			continue;

		break;
	}

	return c;
}

void fill_buff_rand(char *str, struct mode m, FILE *random)
{
	for (int i = 0; i < m.length; i++)
		str[i] = get_rand_char(m, random);
}
