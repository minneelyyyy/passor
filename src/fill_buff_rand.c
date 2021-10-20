#include <stdio.h>
#include <string.h>
#include "mode.h"

static char get_rand_char(struct mode m, FILE *random)
{
	char c;

	for (;;)
	{
		c = getc(random) % 93 + 33;

		if (!m.upper && c <= 'Z' && c >= 'A')
			continue;
		
		if (!m.lower && c <= 'z' && c >= 'a')
			continue;
		
		if (!m.numbers && c >= '0' && c <= '9')
			continue;
		
		if (!m.symbols && !!strchr("`!@#$%^&*()-=_+[]{}\\|;:'\",<.>/?", c))
			continue;
		
		break;
	}

	return c;
}

void fill_buff_rand(char *str, struct mode m, FILE *random)
{
	const int len = m.length;

	for (int i = 0; i < len; i++)
	{
		char c = get_rand_char(m, random);
		str[i] = c;
	}
}