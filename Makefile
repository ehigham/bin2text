CLFLAGS=-O3 -DNDEBUG -std=c11 $(INLINE_WARN) $(INLINE_LIMIT) -pthread
GPP=gcc
BASEDIR=.
OBJDIR=$(BASEDIR)/obj
BIN=$(BASEDIR)
SRCDIR=src
LIBCINC= -I.
LIBLINC= -L. 
LIBLINK= -lm #-lgsl -lgslcblas #-lgmp -lmpfr

CFLAGS=$(CLFLAGS)  -Wall  $(LIBCINC) 
LFLAGS=$(CLFLAGS)  -Wall  $(LIBLINC)

#############################################################################
#add non-executables .cpp files to OBJS 
#if .cpp file reside in a dir other than the current one, add .o rule below
OBJS=$(OBJDIR)/run.o 

#add .cpp files (including executables') to CPPFILES.
CPPFILES= \
$(SRCDIR)/run.c \
$(SRCDIR)/main.c

##############################################################################
#EXECUTABLES: add executables rules here

all: search 

search: $(OBJDIR)/search.oo
	cp $(OBJDIR)/search.oo $(BIN)/run

%.oo: %.o $(OBJS) $(LIBS)
	echo $<
	$(GPP) $(LFLAGS) -o $@ $(OBJS) $< $(LIBLINK)

$(OBJDIR)/search.o: $(SRCDIR)/main.c
	mkdir -p obj
	$(GPP) $(CFLAGS) -o $(OBJDIR)/search.o -c $(SRCDIR)/main.c


##############################################################################
# add non-executables .o rules here

$(OBJDIR)/run.o: $(SRCDIR)/run.c $(SRCDIR)/run.h
	$(GPP) $(CFLAGS) -o $(OBJDIR)/run.o -c $(SRCDIR)/run.c

###############################################################################

clean:
	@rm -f $(OBJDIR)/*.o $(OBJDIR)/*.oo $(SRCDIR)/*~ 
