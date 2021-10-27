.PHONY: run clean all

all: tic-tac-toe.so

clean:
	rm -f tic-tac-toe.so

tic-tac-toe.so: tic-tac-toe.c
	$(CC) -o $@ -fPIC -shared $< -ljson-c

PORT = 4444

run: tic-tac-toe.so
	@echo appli: http://localhost:$(PORT)/tic-tac-toe.html
	@echo monitor: http://localhost:$(PORT)/monitoring
	afb-binder -vv -b ./tic-tac-toe.so -p $(PORT) --roothttp=. -M --tracereq=common --traceevt=all

