all: \
	data-life-cycle.svg \
	predef-types-converts.svg \


%.svg: %.dot
	dot -Tsvg $< > $@

