PYTHON = python.exe

CC = gcc

CFLAGS = -Wall -pedantic

all: build

table.png: table.rst rst2img.py
	$(PYTHON) rst2img.py table.rst table.png

doc.html: doc.rst
	$(PYTHON) c:\python27\Scripts\rst2html.py --math-output=MathML doc.rst doc.html

show:
	$(PYTHON) show.py

build: float


.PHONY: all
