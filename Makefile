CLFLAGS=-O3 -DNDEBUG -std=c11 $(INLINE_WARN) $(INLINE_LIMIT) -pthread
GPP=gcc
BASEDIR=.
OBJDIR=$(BASEDIR)/obj
BIN=$(BASEDIR)
SRCDIR=src
RESDIR=$(SRCDIR)/resource
LIBCINC= -I.
LIBLINC= -L. 
LIBLINK= -lm #-lgsl -lgslcblas #-lgmp -lmpfr

CFLAGS=$(CLFLAGS)  -Wall  $(LIBCINC) 
LFLAGS=$(CLFLAGS)  -Wall  $(LIBLINC)

#############################################################################
#add non-executables .c files to OBJS 
#if .c file reside in a dir other than the current one, add .o rule below
OBJS=$(OBJDIR)/strings.o \
	 $(OBJDIR)/run.o 


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

$(OBJDIR)/strings.o: $(RESDIR)/strings.c
	$(GPP) $(CFLAGS) $ -c $ $^ $ $ -o $ $@


$(OBJDIR)/run.o: $(SRCDIR)/run.c
	$(GPP) $(CFLAGS) $ -c $ $^ $ $ -o $ $@

###############################################################################

clean:
	@rm -f $(OBJDIR)/*.o $(OBJDIR)/*.oo $(SRCDIR)/*~ 
