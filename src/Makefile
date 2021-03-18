EMCC=emcc

all:
	$(EMCC) utils/*.cpp entity_properties/*.cpp entities/*.cpp visuals/*.cpp *.cpp \
	--debug -s USE_SDL=2 -s USE_SDL_IMAGE=2  -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='["png"]' -s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1 \
	--preload-file ../resources -o index.js; \
	 $(MAKE) copy-wasm-to-build-folder

native:
	g++ entities/*.cpp entity_properties/*.cpp utils/*.cpp visuals/*.cpp *.cpp \
	-g -w -lSDL2 -lSDL2_ttf  -lSDL2_image -lSDL2_mixer -ldump_lib; \
	 $(MAKE) copy-native-to-build-folder

copy-wasm-to-build-folder:
	mkdir -p ../build/web
	cp index.html ../build/web
	cp index.js ../build/web
	cp index.wasm ../build/web
	cp index.data ../build/web

copy-native-to-build-folder:
	mkdir -p ../build/native
	cp a.out ../build/native


test-import:
	g++ import_level.cpp world.cpp entities/*.cpp entity_properties/*.cpp utils/utils.cpp visuals/*.cpp \
	-g -w -lSDL2 -lSDL2_ttf  -lSDL2_image -lSDL2_mixer -ldump_lib 

clean:
	rm -f index.js index.wasm index.data *.out