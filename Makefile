PYTHON = c:\python26\python.exe

show:
	$(PYTHON) show.py

build:
	$(PYTHON) c:\python26\Scripts\rst2html.py --math-output=MathML doc.rst doc.html

rst:
	$(PYTHON) c:\python26\Scripts\rst2odt.py doc.rst doc.odt


all: build show 
	

.PHONY: all
	
