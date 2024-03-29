#/bin/bash

h="$(dirname $0)"
f=false
: ${bd:=build}
: ${PREFIX:=$HOME/.local}
eval set -- $(getopt -o b:fp: -l buildir:,force,prefix: -- "$@") || exit
while :; do
	case "$1" in
	-b|--buildir) bd="$2"; shift;;
	-f|--force) force=true;;
	-p|--prefix) PREFIX="$2"; shift;;
	--) shift; break;;
	esac
	shift
done

cd "$h"
h=$(pwd)
mkdir -p "$bd" || exit
cd "$bd" || exit
$force && rm -r * 2>/dev/null || rm CMakeCache.txt 2>/dev/null

cmake \
	-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX:=$PREFIX} \
	"$h"

make -j "$@"
