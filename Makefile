config="debug"

all: FORCE
	premake5 gmake
	make -j8 -C build config=${config}
run: FORCE
	premake5 gmake
	make -j8 -C build lwss config=${config}
	bin/lwss ${args}

docs: FORCE
	make -C docs
tags: FORCE
	ctags -R src/lwss include/lwss
	cscope -Rb -s src/lwss -s include/lwss

clean: FORCE
	rm -rf build
	rm -rf docs/build
distclean: clean
	rm -rf bin
	rm -f docs/paper.pdf
FORCE:

# Clarifies gmake that these aren't deps.
.PHONY: all run docs tags clean distclean
