#ifndef MODE_H
#define MODE_H

#include <stdbool.h>

struct mode
{
	// flags for basic generation
	union {
		unsigned basic : 4;
		struct {
			bool lower   : 1;
			bool upper   : 1;
			bool numbers : 1;
			bool symbols : 1;
			bool spaces  : 1;
		};
	};

	// special generation modes
	union {
		unsigned special : 2;
		struct {
			bool base64 : 1;
			bool base16 : 1;
		};
	};

	// other
	char characters_not_allowed[64];
	int length;
};

#endif /* MODE_H */