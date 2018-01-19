config="debug"

all: FORCE
	premake5 gmake
	make -j8 -C build config=${config}
run: FORCE
	premake5 gmake
	make -j8 -C build osgw config=${config}
	bin/osgw ${args}

docs: FORCE
	make -C docs
tags: FORCE
	ctags -R src/osgw include/osgw
	cscope -Rb -s src/osgw -s include/osgw

clean: FORCE
	rm -rf build
	rm -rf docs/build
distclean: clean
	rm -rf bin
	rm -f docs/paper.pdf
FORCE:

# Clarifies gmake that these aren't deps.
.PHONY: all run docs tags clean distclean
