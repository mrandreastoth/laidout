###############################################################
##############                                  ###############
#############  Laidout src/calculator Makefile   ##############
##############                                  ###############
###############################################################

include ../../Makefile-toinclude
LAXIDIR=$(LAXDIR)/interfaces
LAXOBJDIR=$(LAXDIR)
LD=g++
LDFLAGS= $(EXTRA_LDFLAGS) -L$(LAXDIR) -L$(LAXIDIR) -L/usr/local/lib -L/usr/X11R6/lib -rdynamic -lXi -lXext -lX11 -lm -lpng `imlib2-config --libs` `freetype-config --libs`\
		 `cups-config --libs` -ldl -lXft
DEBUGFLAGS= -g -gdwarf-2
CPPFLAGS= -Wall $(HIDEGARBAGE) $(DEBUGFLAGS) $(EXTRA_CPPFLAGS)  -I$(LAXDIR)/.. `freetype-config --cflags` -I$(POLYPTYCHBASEDIR)


objs= \
	values.o \
	interpreter.o \
	calculator.o \
	shortcuttodef.o



dataobjects: $(objs)

depends:
	makedepend -fmakedepend -I.. -I$(LAXDIR)/.. -Y *.cc

include makedepend

objectatt2code: values.o objectatt2code.cc
	g++ $(CPPFLAGS) $@.cc values.o ../fieldplace.o -llaxkit -llaxinterfaces $(LDFLAGS) -lharfbuzz -lcairo -lfontconfig -lsqlite3 -o $@

hidegarbage:
	../hidegarbage *.cc

unhidegarbage:
	../hidegarbage -0 *.cc


.PHONY: clean printing hidegarbage unhidegarbage
clean:
	rm -f *.o objectatt2code

