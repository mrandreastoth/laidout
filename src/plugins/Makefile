############################################################
##############                               ###############
#############  Laidout src/plugins Makefile   ##############
##############                               ###############
############################################################


include ../../Makefile-toinclude

LAXIDIR=$(LAXDIR)/interfaces
LAXOBJDIR=$(LAXDIR)
LD=g++
LDFLAGS= $(EXTRA_LDFLAGS) -L/usr/X11R6/lib -lXi -lXext -lX11 -lm -lpng `imlib2-config --libs` `freetype-config --libs`\
		 `cups-config --libs` -lXft -L$(LAXIDIR) -L$(LAXDIR)
DEBUGFLAGS= -g -gdwarf-2
CPPFLAGS= $(EXTRA_CPPFLAGS) -fPIC -Wall $(DEBUGFLAGS) -I$(LAXDIR)/.. -I/usr/include/freetype2



objs= \
	plugin.o 

#
# If you want a plugin built by default, include in plugins here.
#
plugins= \
	geglnodes.so \
	#exampleplugin.so \
	#antiprism.so


all: lax laxinterface $(objs) $(plugins)

plugins: $(plugins)

lax:
	cd $(LAXDIR) && $(MAKE)

laxinterface:
	cd $(LAXIDIR) && $(MAKE)

depends:
	makedepend -fmakedepend -I.. -I$(LAXDIR)/.. -Y *.cc

include makedepend

test: lax laxinterface plugin.o test.o exampleplugin.so
	g++ -rdynamic $(CPPFLAGS) plugin.o test.o -llaxkit $(LDFLAGS) -ldl -o test


####------------------  Gegl  --------------------#####
GEGLCPPFLAGS= `pkg-config --cflags gegl-0.3`
GEGLLIBS= `pkg-config --libs gegl-0.3`

geglnodes.so: geglnodes.o svgnodes.o
#geglnodes.so: geglnodes.cc
	@echo "Compiling geglnodes with pkg-config path $(PKG_CONFIG_PATH)"
	@echo "gegl cflags: $(GEGLCPPFLAGS)"
	@echo "gegl libs  : $(GEGLLIBS)"
#	g++ -shared -fPIC $(CPPFLAGS) $(GEGLCPPFLAGS) -L$(LAXDIR) geglnodes.cc -llaxkit $(GEGLLIBS) -o geglnodes.so
	g++ -shared -fPIC $(CPPFLAGS) $(GEGLCPPFLAGS) -L$(LAXDIR) svgnodes.o geglnodes.o -llaxkit $(GEGLLIBS) -o geglnodes.so

geglnodes.o: geglnodes.cc
	g++ -shared -fPIC $(CPPFLAGS) $(GEGLCPPFLAGS) -L$(LAXDIR) geglnodes.cc -llaxkit $(GEGLLIBS) -c -o $@

svgnodes.o: svg/svgnodes.cc
	g++ -shared -fPIC $(CPPFLAGS) $(GEGLCPPFLAGS) -L$(LAXDIR) svg/svgnodes.cc -llaxkit $(GEGLLIBS) -c -o $@


####------------------  Python  --------------------#####
PYTHONCPPFLAGS= `python3.5-config --cflags`
PYTHONLIBS= `python3.5-config --ldflags`

python.so: python.cc
	@echo "Compiling geglnodes with pkg-config path $(PKG_CONFIG_PATH)"
	@echo "gegl cflags: $(GEGLCPPFLAGS)"
	@echo "gegl libs  : $(GEGLLIBS)"
	g++ -shared -fPIC $(CPPFLAGS) $(PYTHONCPPFLAGS) -L$(LAXDIR) python.cc -llaxkit $(PYTHONLIBS) -o python.so


####------------------  ExamplePlugin  --------------------#####
exampleplugin.so: lax laxinterface exampleplugin.o
	g++ $(CPPFLAGS) -shared -fPIC exampleplugin.o -L$(LAXDIR) -llaxkit -o exampleplugin.so
	#g++ $(CPPFLAGS) -shared -fPIC exampleplugin.cc -L$(LAXDIR) -o exampleplugin.so


hidegarbage:
	../hidegarbage *.cc

unhidegarbage:
	../hidegarbage -0 *.cc


.PHONY: clean hidegarbage unhidegarbage lax laxinterfaces all plugins depends
clean:
	rm -f *.o *.so test svg/*.o

