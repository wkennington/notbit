#! /usr/bin/env bash

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$(pwd)/src/.libs/"
make check
gdb --tui $1
