#!/bin/bash

# Elliott Darfink
# https://stackoverflow.com/questions/60654834/using-mprotect-to-make-text-segment-writable-on-macos
# https://gist.github.com/darfink/a756b88e999631d75cbbeefd3eee7e2d


if test `uname` == 'Darwin'; then
    printf '\x07' | dd of=$1 bs=1 seek=160 count=1 conv=notrunc ;
fi

# objcopy --writable-text a.out
