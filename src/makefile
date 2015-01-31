
GLLIBS = -lGL -lGLU -lglut -lGLEW -lSOIL
#IMGLIBS = -lSOIL
#ALLIBS = -lopenal -lalut

PROGRAMS = game

.SUFFIXES: .cpp .c .h .hpp .cc

all: $(PROGRAMS)

clean: 
	rm -f $(PROGRAMS)
	rm -f *.o
	rm -f *.out

game: track01.cpp
	g++ -g -Wall main.cpp tiny_obj_loader.cc -o $@ $(GLLIBS) 
