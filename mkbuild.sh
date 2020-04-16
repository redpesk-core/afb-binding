#/bin/bash

h="$(dirname $0)"
f=false
arch=
bd=
: ${PREFIX:=}
eval set -- $(getopt -o b:fp: -l buildir:,force,--prefix: -- "$@") || exit
while :; do
	case "$1" in
	-b|--buildir) bd="$2"; shift;;
	-f|--force) force=true;;
	-p|--prefix) PREFIX="$2"; shift;;
	--) shift; break;;
	esac
	shift
done

: ${bd:=build}

mkdir -p "$h/$bd" || exit
cd "$h/$bd" || exit
$force && rm -r * 2>/dev/null || rm CMakeCache.txt 2>/dev/null

cmake \
	-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX:=${PREFIX:=$HOME/.local}} \
	..

make -j "$@"
