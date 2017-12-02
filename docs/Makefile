name := paper
viewer := mupdf

sections := $(wildcard sections/*.tex)

all: $(name).pdf
view: $(name).pdf
	$(viewer) $(name).pdf
$(name).pdf: $(name).tex $(sections) $(name).bib
	mkdir -p build
	pdflatex -output-directory build/ $(name)
	bibtex build/$(name)
	pdflatex -output-directory build/ $(name)
	pdflatex -output-directory build/ $(name)
	mv build/$(name).pdf .
clean:
	rm -rf build
distclean: clean
	rm -f $(name).pdf
.PHONY: all clean
