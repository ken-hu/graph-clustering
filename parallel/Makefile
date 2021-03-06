CXX 		= mpic++
#CXX 		= vtcxx -vt:c++ mpic++ -DVTRACE -DVT_
#VTINC		= -I /usr/local/include/vampirtrace
INCPATH		= include
LIBDIR		= -L/usr/local/lib
LDLIBS		= -lboost_mpi -lboost_serialization -lboost_program_options
#MEDIANFLAG	= -DMedian_
CXXFLAGS	= -I $(INCPATH) $(VTINC) -Wall -std=c++11 -O3 -finline-functions -ffast-math -fomit-frame-pointer -funroll-loops $(MEDIANFLAG)

SRCDIR		= src
BUILDDIR	= build

CXXFILES	= $(shell find $(SRCDIR) -name '*.cc')		
OBJECTS 	= $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(CXXFILES:cc=o))
TARGET 		= ../main_parallel

all: $(TARGET)

$(TARGET): $(filter-out $(BUILDDIR)/test.o, $(OBJECTS))
	@echo "Linking..."
	$(CXX) -o $@ $^ $(LIBDIR) $(LDLIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Explicit dependencies required for headers
$(OBJECTS):	$(INCPATH)/graph.h
$(BUILDDIR)/test.o $(BUILDDIR)/partition.o:	$(INCPATH)/*.h $(SRCDIR)/lanczos.cc

define OBJECT_DEPENDS_ON_CORRESPONDING_HEADER
   $(1) : $(patsubst $(BUILDDIR)/%, $(INCPATH)/%, $(1:o=h)) 
endef

$(foreach object_file,$(OBJECTS),$(eval $(call OBJECT_DEPENDS_ON_CORRESPONDING_HEADER, $(filter-out $(BUILDDIR)/main.o, $(object_file)))))

# Phony target to get around problem of having a file called 'clean'
.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(TARGET)

tester_parallel: $(filter-out $(BUILDDIR)/main.o, $(OBJECTS))
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBDIR) $(LDLIBS)
	mv tester_parallel ../
