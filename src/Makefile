GCC=g++
EMCC=emcc
COMPILER_FLAGS = -g -w
LINKER_FLAGS = -lSDL2 -lSDL2_ttf  -lSDL2_image -lSDL2_mixer -ldump_lib

all: 
	$(GCC) *.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS)

wasm:
	$(EMCC) utils/*.cpp entity_properties/*.cpp entities/*.cpp visuals/*.cpp *.cpp \
	--debug -s USE_SDL=2 -s USE_SDL_IMAGE=2  -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='["png"]' -s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1 \
	--preload-file ../assets -o index.js; \
	 $(MAKE) copy-to-build-folder

copy-to-build-folder:
	mkdir -p ../build
	cp index.html ../build
	cp index.js ../build
	cp index.wasm ../build
	cp index.data ../build

clean:
	rm -f index.js index.wasm index.data