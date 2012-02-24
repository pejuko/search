#!/bin/sh

function run() {
	echo -ne "$1..."
	if $*; then
		echo "ok";
	else
		echo "bad";
	fi
}

# copy macros
run aclocal

# create config.h.in
run autoheader

# only for libraries
run libtoolize --automake -c

# create Makefile.in
run automake --foreign -a -c

# create Makefile and config.h
run autoconf


# configure for build
./configure $*

