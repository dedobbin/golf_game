GCC=g++
COMPILER_FLAGS = -g -w
LINKER_FLAGS = -lSDL2 -lSDL2_ttf  -lSDL2_image -lSDL2_mixer -ldump_lib

all: 
	$(GCC) *.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS)