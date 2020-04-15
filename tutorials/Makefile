# Copyright (C) 2015-2020 IoT.bzh Company
# Author: José Bollo <jose.bollo@iot.bzh>
#
# SPDX-License-Identifier: LGPL-3.0-only

FLAGS = \
	-fPIC \
	$(shell pkg-config --cflags --libs afb-binding) \
	-Wl,--version-script=$(shell pkg-config --variable=version_script afb-binding)

CFLAGS += ${FLAGS}
CXXFLAGS += ${FLAGS}

.PHONY: all clean

all: tuto-1.so tuto-2.so tuto-3.so tuto-4.so tuto-5.so tuto-app1.so

clean:
	rm *.so 2>/dev/null || true

%.so: %.c
	$(CC) $(CFLAGS) -shared -o $@ $<

%.so: %.cpp
	$(CXX) $(CXXFLAGS) -shared -o $@ $<
