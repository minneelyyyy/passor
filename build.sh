#!/usr/bin/env bash

PROGRAMNAME="passor"

CBUILDOPTS=(
    "-Wall" "-Werror"     # warnings
    "-O3" "-march=native" # optimization
)

SOURCEFILES=(
    "src/main.c"
    "src/hash.c"
    "src/parse_opts.c"
    "src/fill_buff_rand.c"
)

CBUILDMODE=$1

[[ "$CBUILDMODE" = "" ]] && CBUILDMODE="$PROGRAMNAME"

function build {
    gcc ${CBUILDOPTS[@]} -o "$PROGRAMNAME" ${SOURCEFILES[@]}
}

function debug {
    gcc ${CBUILDOPTS[@]} -DDEBUG -o "$PROGRAMNAME" ${SOURCEFILES[@]}
}

function install {
    mv "./$PROGRAMNAME" "/usr/bin/$PROGRAMNAME"
}

function clean {
    rm "$PROGRAMNAME"
}

if [[ "$CBUILDMODE" = "$PROGRAMNAME" ]]
then
	build
else
	$CBUILDMODE
fi
