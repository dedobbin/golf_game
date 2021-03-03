GCC=g++
EMCC=emcc
COMPILER_FLAGS = -g -w
LINKER_FLAGS = -lSDL2 -lSDL2_ttf  -lSDL2_image -lSDL2_mixer -ldump_lib

all: 
	$(GCC) *.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS)

wasm:
	$(EMCC) *.cpp --debug -s USE_SDL=2 -s USE_SDL_IMAGE=2  -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='["png"]' -s DISABLE_EXCEPTION_CATCHING=0 \
	--preload-file assets -o index.js