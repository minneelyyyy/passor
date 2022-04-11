#!/usr/bin/env bash

PROGRAMNAME="passor"

CBUILDOPTS=(
    "-Wall" "-Werror" # warnings
    "-std=c89" "-pedantic" # language
)

SOURCEFILES=(
    "src/main.c"
    "src/hash.c"
    "src/parse_opts.c"
    "src/fill_buff_rand.c"
)

CBUILDMODE=$1

[[ "$CBUILDMODE" = "" ]] && CBUILDMODE="$PROGRAMNAME"

# disables optimization for debug builds
if [[ ! "$CBUILDMODE" = *"debug"* ]]
then
    CBUILDOPTS+=( "-O3" "-march=native" )
else
    CBUILDOPTS+=( "-O0" "-g" )
fi

# build modes

## linux
function build {
    gcc ${CBUILDOPTS[@]} -o "$PROGRAMNAME" ${SOURCEFILES[@]}
}

function debug {
    gcc ${CBUILDOPTS[@]} -DDEBUG -o "$PROGRAMNAME" ${SOURCEFILES[@]}
}

## windows
function windows {
   x86_64-w64-mingw32-gcc ${CBUILDOPTS[@]} -o "$PROGRAMNAME.exe" ${SOURCEFILES[@]}
}

function windows_debug {
    x86_64-w64-mingw32-gcc ${CBUILDOPTS[@]} -DDEBUG -o "$PROGRAMNAME.exe" ${SOURCEFILES[@]}
}

# other
function install {
    mv "./$PROGRAMNAME" "/usr/bin/$PROGRAMNAME"
}

function uninstall {
    rm -f "/usr/bin/$PROGRAMNAME"
}

function clean {
    rm -f "$PROGRAMNAME" "$PROGRAMNAME.exe"
}

if [[ "$CBUILDMODE" = "$PROGRAMNAME" ]]
then
    build
else
    $CBUILDMODE
fi
