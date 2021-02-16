GCC=g++
EMCC=emcc
COMPILER_FLAGS = -g -w
LINKER_FLAGS = -lSDL2 -lSDL2_ttf  -lSDL2_image -lSDL2_mixer -ldump_lib

all: 
	$(GCC) *.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS)

wasm:
	$(EMCC) *.cpp -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' \
	--preload-file assets -o index.js