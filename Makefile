PYTHON = python.exe

CC = gcc

CFLAGS = -Wall -pedantic

all: build doc.html

formulas.png: formulas.tex
	$(PYTHON) texmath2img.py formulas.tex formulas.png


prec_table.rst: float
	float prec > prec_table.rst

bench_table.rst: float
	float bench > bench_table.rst

bench_table.png: bench_table.rst
	$(PYTHON) rst2img.py bench_table.rst bench_table.png

doc.html: doc.rst
	$(PYTHON) c:\python27\Scripts\rst2html.py --math-output=MathML doc.rst doc.html

doc.rst: bench_table.png bench_table.rst prec_table.rst formulas.png

show:
	$(PYTHON) show.py

build: float


.PHONY: all
