# Copyright (C) 2015-2022 IoT.bzh Company
# Author: José Bollo <jose.bollo@iot.bzh>
#
# SPDX-License-Identifier: LGPL-3.0-only

FLAGS = -g \
	-fPIC \
	$(shell pkg-config --cflags --libs afb-binding) \
	-Wl,--version-script=$(shell pkg-config --variable=version_script afb-binding)

CFLAGS += ${FLAGS}
CXXFLAGS += ${FLAGS} -fpermissive

.PHONY: all clean

tutos = tuto-1.so tuto-2.so tuto-3.so tuto-4.so tuto-5.so tuto-app1.so
samples = hello3.so

all: ${tutos} ${samples}
	$(MAKE) -C tic-tac-toe all

clean:
	rm *.so 2>/dev/null || true
	$(MAKE) -C tic-tac-toe clean

%.so: %.c
	$(CC) $(CFLAGS) -shared -o $@ $<

%.so: %.cpp
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

