#OBJS specifies source files
OBJS = RaXaR.cpp View.cpp Shapes.cpp Illumination.cpp Colour.cpp GeomX.cpp TGAReader.cpp TGAWriter.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS
COMPILER_FLAGS = -pedantic -Wall -Werror -std=c++11 -O3

#OBJ_NAME
OBJ_NAME = raxar

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

