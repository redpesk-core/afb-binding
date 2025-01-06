# Copyright (C) 2015-2025 IoT.bzh Company
# Author: José Bollo <jose.bollo@iot.bzh>
#
# SPDX-License-Identifier: LGPL-3.0-only

INSRC = $(shell test -e ../../afb/afb-export.map && echo true || echo false)

ifeq (${INSRC},true)
AFBINC = ../../afb
AFBFLAGS = -I../.. -Wl,--version-script=../../afb/afb-export.map
else
AFBINC = $(shell pkg-config --variable=incdir afb-binding)
AFBFLAGS = $(shell pkg-config --cflags --libs afb-binding) \
	-Wl,--version-script=$(shell pkg-config --variable=version_script afb-binding)
endif
AFBDEP = $(shell find ${AFBINC} -type f)
FLAGS = -g -fPIC -shared ${AFBFLAGS}

CFLAGS += ${FLAGS}
CXXFLAGS += ${FLAGS} -fpermissive

.PHONY: all clean

tutos = tuto-1.so tuto-2.so tuto-3.so tuto-4.so tuto-5.so tuto-6.so tuto-app1.so
samples = hello3.so

all: ${tutos} ${samples}
	$(MAKE) -C tic-tac-toe all

clean:
	rm *.so 2>/dev/null || true
	$(MAKE) -C tic-tac-toe clean

%.so: %.c  ${AFBDEP}
	$(CC) $(CFLAGS) -o $@ $<

%.so: %.cpp  ${AFBDEP}
	$(CXX) $(CXXFLAGS) -o $@ $<

