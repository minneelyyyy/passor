#!/usr/bin/env bash

PROGRAMNAME="passor"

CBUILDOPTS=(
    "-Wall" "-Werror" "-Wno-unused-function" # warnings
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

function use_heap {
    gcc ${CBUILDOPTS[@]} -DHEAP_BUFFER -o "$PROGRAMNAME" ${SOURCEFILES[@]}
}

function windows {
   x86_64-w64-mingw32-gcc ${CBUILDOPTS[@]} -o "$PROGRAMNAME.exe" ${SOURCEFILES[@]}
}

function windows_heap {
   x86_64-w64-mingw32-gcc ${CBUILDOPTS[@]} -DHEAP_BUFFER -o "$PROGRAMNAME.exe" ${SOURCEFILES[@]}
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
