
#ifndef EXIT_CODES_H
#define EXIT_CODES_H

enum exit_codes
{
    /* this is unused */
    NORMAL_EXIT = 0,

    /* def: this error is raised if the user specifies a length greater than the maximum
     *   the default maximum is 100,000, but can be changed through compiler options with -DMAX_LENGTH={num}
     *
     * error message: "passor: error: length specified too long (maximum: {MAX_LENGTH})"
     * raised in: main.c, main()
     ***********************************************************************************************************/

    SPECIFIED_LENGTH_OVER_MAX,


    /* def: this error is raised if the user tries to use multiple special flags in the same call
     *   special flags are flags that use a preset string to generate random characters
     *   we do not let the user use multiple of these modes at the same time
     *
     * error message: "passor: error: cannot set multiple special flags"
     * raised in: parse_opts.c, MACRO special_check_special()
     ***********************************************************************************************************/

    MULTIPLE_SPECIAL_FLAGS,


    /* def: this error is raised if the user tries to use a normal flag while a special flag is set
     *   special flags are flags that use a preset string to generate random characters
     *   we do not let the user manipulate this string (maybe in a future update)
     *
     * error message: "passor: error: cannot set {argument} when special flag is set"
     * raised in: parse_opts.c, MACRO check_special()
     ***********************************************************************************************************/

    SPECIAL_FLAG_SET,


    /* def: this error is raised if the user tries to pass the same argument twice
     *   this is just to enforce proper usage of the program, there is no reason to pass an argument twice
     *
     * error message: "passor: error: {argument} set twice"
     * raised in: parse_opts.c, MACRO check_bool_set_twice()
     ***********************************************************************************************************/
    
    ARGUMENT_SET_TWICE,


    /* def: this error is raised if the user tries to pass an argument that takes arguments as the last argument
     *   there is only one argument (--dont-allow) that this effects
     *
     * example: passor 16 --dont-allow
     *
     * error message: "passor: error: no argument given for multi-argument parameter {argument}"
     * raised in: parse_opts.c, parse_long_option()
     ***********************************************************************************************************/

    LAST_ARGUMENT_REQUIRES_ARGUMENT,


    /* def: this error is raised if the user tries to pass an argument that the program doesn't recognize
     *
     * error message: "passor: error: {argument} is not a valid argument"
     *                or "passor: error: {flag} is not a valid flag"
     * raised in: parse_opts.c, parse_long_option(), parse_flags(), parse_opts()
     ***********************************************************************************************************/

    INVALID_ARGUMENT,


    /* def: this error is raised if the user makes it so that there are no
     *   characters left for the program to generate
     *
     * example: passor -ULSN
     *
     * error message: "passor: error: no printable characters"
     * raised in: fill_buff_rand.c, fill_buff_rand()
     ***********************************************************************************************************/

    NO_AVAILABLE_CHARACTERS
};

#endif /* EXIT_CODES_H */