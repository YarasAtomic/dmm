#
#	makefile for c++ projects for Linux and Windows specialiced in openGL
#
#	modes:
#		make: 			default build for the current OS (only compiles modified files)
#		make remake: 	compile everything again (removes every object file and compiles every file)
#		make clean:		remove bin/executable and object files
#		make run:		default build and run
#		make cleanrun	compile everything again and run
#		make buildlib: 	compile only the library
#		make fromlib: 	build using the library
#

KER = $(shell uname)
OSNAME= NONE
LIBS= NONE
LIBSDIR= NONE
BIN= NONE
INCLUDEDIR= NONE

CXX= g++

MODULES= $(wildcard src/*.cpp)
OBJDIR= obj
BUILDEDLIB= lib
INCLUDE= include
SOURCE= src
DEBUG= -g

OUT= Default build

LIBFILE = libgumEngine.a

ifeq ($(OS),Windows_NT) #OS is a Windows specific enviroment variable
	OUT= Windows build ...
	OSNAME = WINDOWS
	BIN = dmm.exe
#	LIBS = -lopengl32 -lfreeglut -lglu32 -llibportaudio
	LIBS = -lraylibdll
	LIBSDIR = -L"G:\Archivos de programa x86\raylib\lib"
	INCLUDEDIR = -I"G:\Archivos de programa x86\raylib\include"
else
	ifeq ($(KER),Linux)
		OUT= Linux build ...
		OSNAME = LINUX
		BIN = dmm
		LIBS = -lglut -lGLU -lGL -lc -lm -lX11 -lportaudio -lsndfile
		LIBSDIR = -L/usr/lib
		INCLUDEDIR = -I/usr/include
		
	endif
endif

$(info $(OUT))

DEF= -D$(OSNAME)

LDFLAGS= $(LIBSDIR)
CXXFLAGS= -c -I$(INCLUDE) $(INCLUDEDIR) $(DEBUG)

OBJ_FILES = $(MODULES:$(SOURCE)/%.cpp=$(OBJDIR)/%.o)
MAIN_OBJ = $(OBJDIR)/main.o

$(info $(OBJ_FILES))

build: $(BIN)

cleanrun: clean run

remake: clean build

run: build
	./$(BIN)

clean:
ifeq ($(OSNAME),LINUX)
	@rm $(OBJDIR)/*.o
	@rm $(BUILDEDLIB)/*.a
	@rm -r -f $(BIN)
else
	@del $(OBJDIR)\\*.o
	@del $(BUILDEDLIB)\\*.a
	@del $(BIN)
endif

buildlib: $(LIBFILE)

fromlib: $(LIBFILE) $(MAIN_OBJ)
	$(CXX) -std=c++17 -o $(BIN) $(OBJDIR)/main.o $(LDFLAGS) -L./lib -lgumEngine $(LIBS) 

.PHONY: build clean run remake cleanrun buildlib fromlib

#build the library
$(LIBFILE) : $(OBJ_FILES) $(BUILDEDLIB)
	ar rvs $(BUILDEDLIB)/$(LIBFILE) $(OBJ_FILES)

#link object files
$(BIN) : $(MAIN_OBJ) $(OBJ_FILES)
#	$(CXX) $(pkg-config --libs sndfile) -std=c++17 -o $(BIN) $(OBJDIR)/*.o $(LDFLAGS) $(LIBS)
	$(CXX) -std=c++17 -o $(BIN) $(OBJDIR)/*.o $(LDFLAGS) $(LIBS)

#compile every .cpp in SOURCE
$(OBJ_FILES) : $(OBJDIR)/%.o: $(SOURCE)/%.cpp
	$(CXX) -std=c++17 $(DEF) $(CXXFLAGS) $< -o $@

#compile main.cpp
$(MAIN_OBJ) : main.cpp $(OBJDIR)
	$(CXX) -std=c++17 $(DEF) $(CXXFLAGS) $< -o $(OBJDIR)/main.o 

#make object directory if it doesnt exists
$(OBJDIR) :
	mkdir $(OBJDIR)

#make lib directory if it doesnt exists
$(BUILDEDLIB) :
	mkdir $(BUILDEDLIB)
