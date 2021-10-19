#ifndef MODE_H
#define MODE_H

#include <stdbool.h>

struct mode
{
	bool lower: 1;
	bool upper: 1;
	bool numbers: 1;
	bool symbols: 1;
	unsigned int length;
};

#endif /* MODE_H */