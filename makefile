CFLAGS=-c -W -Wall -Wunused -Wcast-align -Werror -fstack-protector-all -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wcast-align -Wno-format -Wno-long-long -Wmissing-declarations
LDFLAGS=-W -Wall -Wunused -Wcast-align -Werror -fstack-protector-all -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wcast-align -Wno-format -Wno-long-long -Wmissing-declarations -lm
# You can add -Ofast flag for optimization, but che-to ne to schitaet

SOURCES=func.c gas_two.c setka.c gnuplot.c shema.c tabtex.c
CLEANTEX=$(filter-out ./report/report_files/images, $(wildcard ./report/report_files/*))
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=a.out
REPORT_FILENAME=theplot.pdf

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc $(OBJECTS) ./laspack/liblaspack.a -o $@ $(LDFLAGS)

.cpp.o:
	gcc $(CFLAGS) $<

clean:
	rm -rf *.o a.out leak.out output.txt

gnuplot.o: gnuplot.h

tabtex.o: tabtex.h

*.o: func.h


tex_smooth:
	pdflatex -shell-escape plot_smooth.tex
	pdflatex table_smooth.tex
	mv -t ./report/report_files *.tex *.eps *.log *.gnuplot *.pdf *.aux

tex_abrupt:
	pdflatex -shell-escape plot_abrupt.tex
	mv -t ./report/report_files *.tex *.eps *.log *.gnuplot *.pdf *.aux

pdf:
	pdflatex ./report/results.tex
	mv -t ./report/report_files *.log  *.aux
	mv  ./results.pdf ./report/


cleanall:
	rm -rf *.o a.out leak.out output.txt *.log theplot*.*  *.eps *.gnuplot *.pdf  $(CLEANTEX)


cleanpdf:
	rm -rf $(CLEANTEX)
