R = R
LATEX = pdflatex

RFILE = protocol.Rnw
LFILE = $(RFILE:.Rnw=.tex)

all:
	$(R) CMD Sweave $(RFILE)
	$(LATEX) $(LFILE)
	$(LATEX) $(LFILE)

clean:
	rm -f *.aux *.bbl *.blg *.log *.dvi \
		*.toc *.pdf *.ps *.out

