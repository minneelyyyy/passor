#!/usr/bin/env bash

PROGRAMNAME="randstr"

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

[[ "$CBUILDMODE" = "" ]] && CBUILDMODE="$PROGRAMNAME"

[[ "$CBUILDMODE" = "$PROGRAMNAME" ]] &&
    gcc ${CBUILDOPTS[@]} -o "$PROGRAMNAME" ${SOURCEFILES[@]}

[[ "$CBUILDMODE" = "debug" ]] &&
    gcc ${CBUILDOPTS[@]} -DDEBUG -o "$PROGRAMNAME" ${SOURCEFILES[@]}

[[ "$CBUILDMODE" = "install" ]] &&
    mv "./$PROGRAMNAME" "/usr/bin/$PROGRAMNAME"

[[ "$CBUILDMODE" = "clean" ]] &&
    rm "$PROGRAMNAME"
