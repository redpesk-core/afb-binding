# Copyright (C) 2015-2026 IoT.bzh Company
# Author: José Bollo <jose.bollo@iot.bzh>
#
# SPDX-License-Identifier: LGPL-3.0-only

base = ../..
INSRC = $(shell test -e $(base)/afb/afb-export.map && echo true || echo false)

ifeq (${INSRC},true)
AFBINC = $(base)/afb
AFBFLAGS = -I$(base) -Wl,--version-script=$(base)/afb/afb-export.map
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

tutos = tuto-1.so
samples = hello4.so empty.so

all: ${tutos} ${samples}

clean:
	rm *.so 2>/dev/null || true

%.so: %.c ${AFBDEP}
	$(CC) $(CFLAGS) -o $@ $<

%.so: %.cpp ${AFBDEP}
	$(CXX) $(CXXFLAGS) -o $@ $<
