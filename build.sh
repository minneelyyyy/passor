#!/usr/bin/env bash

CBUILDOPTS=(
    "-Wall" "-Werror"     # warnings
    "-O3" "-march=native" # performance
)

SOURCEFILES=(
    "src/main.c"
    "src/hash.c"
    "src/parse_opts.c"
    "src/fill_buff_rand.c"
)

CBUILDMODE=$1

[[ "$CBUILDMODE" = "" ]] && CBUILDMODE="passor"

[[ "$CBUILDMODE" = "passor" ]] &&
    gcc ${CBUILDOPTS[@]} -o "passor" ${SOURCEFILES[@]}

[[ "$CBUILDMODE" = "debug" ]] &&
    gcc ${CBUILDOPTS[@]} -DDEBUG -o "passor" ${SOURCEFILES[@]}

[[ "$CBUILDMODE" = "install" ]] &&
    mv "./passor" "/usr/bin/passor"

[[ "$CBUILDMODE" = "clean" ]] &&
    rm "passor"
