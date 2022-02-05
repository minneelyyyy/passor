
#ifndef EXIT_CODES_H
#define EXIT_CODES_H

enum exit_codes
{
    NORMAL_EXIT = 0,
    SPECIFIED_LENGTH_OVER_MAX,
    MULTIPLE_SPECIAL_FLAGS,
    SPECIAL_FLAG_SET,
    ARGUMENT_SET_TWICE,
    LAST_ARGUMENT_REQUIRES_ARGUMENT,
    INVALID_ARGUMENT,
    NO_AVAILABLE_CHARACTERS,
};

#endif /* EXIT_CODES_H */