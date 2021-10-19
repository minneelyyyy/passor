#!/usr/bin/env bash

CBUILDOPTS=(
    "-Wall" "-Werror"     # warnings
    "-O3" "-march=native" # performance
)

SOURCEFILES=(
    "src/main.c"
    "src/hash.c"
    "src/parse_opts.c"
)

CBUILDMODE=$1

[[ "$CBUILDMODE" = "" ]] && CBUILDMODE="passor"

[[ "$CBUILDMODE" = "passor" ]] &&
    gcc ${CBUILDOPTS[@]} -o "passor" ${SOURCEFILES[@]}

[[ "$CBUILDMODE" = "clean" ]] &&
    rm "passor"
