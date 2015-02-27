#!/bin/bash

cd "$(dirname "$0")"

name="${1%.h}"
header="$name.h"

if [ ! -f "$header" ]; then
    echo "Header file '$header' not found"
    exit 1
fi

cat <<BOARD > board.h
#include "$header"

BoardDescription makeNamedBoard() {
    BoardDescription b = makeBoard();
    b.name = "$name";
    return b;
}
BOARD

make
./casefactory
