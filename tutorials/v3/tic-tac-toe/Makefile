.PHONY: run clean all

FLAGS = -g \
	-fPIC \
	-shared \
	$(shell pkg-config --cflags --libs afb-binding) \
	-Wl,--version-script=$(shell pkg-config --variable=version_script afb-binding)

all: tic-tac-toe.so

clean:
	rm -f tic-tac-toe.so

tic-tac-toe.so: tic-tac-toe.c
	$(CC) $(FLAGS) -o $@ $<

PORT = 4444

run: tic-tac-toe.so
	@echo appli: http://localhost:$(PORT)/tic-tac-toe.html
	@echo monitor: http://localhost:$(PORT)/monitoring
	afb-binder -vv -b ./tic-tac-toe.so -p $(PORT) --roothttp=. -M --tracereq=common --traceevt=all

