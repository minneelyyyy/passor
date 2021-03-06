#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "parse_opts.h"
#include "fill_buff_rand.h"
#include "mode.h"
#include "exit_codes.h"

#ifndef MAX_LENGTH
#    define MAX_LENGTH 100000
#endif

/* stores the first argument in argv */
static char *program_name;

/* prints help message which describes how to use the program
 * then exits with exit_status */
void help(int exit_status)
{
    FILE *out = exit_status ? stderr : stdout;
    
    fprintf(
        out,
        "Usage: %s [OPTION]... [LENGTH]\n"
        "generate a password / random string\n"
        "\n",
        program_name
    );

    fprintf(
        out,
        "  -U, --no-upper            generate without uppercase characters\n"
        "  -L, --no-lower            generate without lowercase characters\n"
        "  -N, --no-numbers          generate without number characters\n"
        "  -S, --no-symbols          generate without other characters\n"
        "  -s, --spaces              allows generation to use spaces\n"
    );

    fprintf(
        out,
        "  --number                  generate only a number\n"
        "  --alpha                   generate only alphabetical characters\n"
        "  --alpha-num               the same as --no-symbols\n"
        "  --dont-allow <string>     don't allow generation to use characters in this string\n"
        "  -X, --base64              generate a base64 string\n"
        "  -x, --base16              generate a base16 string\n"
        "\n"
    );

    exit(exit_status);
}

int main(int argc, char *argv[])
{
    char *output;

    struct mode options = {
        /* .lower   = */ true,
        /* .upper   = */ true,
        /* .numbers = */ true,
        /* .symbols = */ true,
        /* .spaces  = */ false,
        /* .special = */ NONE,
        /* .debug   = */ false,
        /* .characters_not_allowed = */ "",
        /* .length  = */ 8
    };
    
    program_name = argv[0];

    if (argc > 1 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")))
        help(0);

    /* fill the options struct with correct, user given values instead of defaults */
    parse_opts(&options, argc, argv);

    #ifdef DEBUG
        /* activate this debug info with the '--debug' command line option (only for debug builds) */
        if (options.debug)
        {
            printf(
                "===========================\n"
                ".upper                  = %d\n"
                ".lower                  = %d\n"
                ".numbers                = %d\n"
                ".symbols                = %d\n"
                ".spaces                 = %d\n"
                ".special                = %d\n"
                ".debug                  = %d\n"
                ".characters_not_allowed = \"%s\"\n"
                ".length                 = %d\n",
                options.lower, options.upper, options.numbers, options.symbols,
                options.spaces, options.special, options.debug,
                options.characters_not_allowed, options.length
            );
        }
    #endif

    /* exit the program if the user specifies <=0 as the length
     * this program defines <=0 as valid inputs that make the program return nothing */
    if (options.length <= 0)
        return 0;

    if (options.length > MAX_LENGTH)
    {
        fprintf(stderr, "passor: error: length specified too long (maximum: %d)\n", MAX_LENGTH);
        exit(SPECIFIED_LENGTH_OVER_MAX);
    }

    output = malloc(options.length + 1);
    output[options.length] = '\0';

    fill_buff_rand(output, options);

    puts(output);

    return 0;
}
