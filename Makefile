EMCC=emcc

all:
	$(EMCC) src/utils/*.cpp src/entity_properties/*.cpp src/entities/*.cpp src/visuals/*.cpp src/*.cpp \
	--debug -s USE_SDL=2 -s USE_SDL_IMAGE=2  -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='["png"]' -s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1 \
	--preload-file resources -o index.js; \
	 $(MAKE) copy-wasm-to-build-folder

native:
	g++ src/entities/*.cpp src/entity_properties/*.cpp src/utils/*.cpp src/visuals/*.cpp src/*.cpp \
	-g -w -lSDL2 -lSDL2_ttf  -lSDL2_image -lSDL2_mixer -ldump_lib; \
	 $(MAKE) copy-native-to-build-folder

copy-wasm-to-build-folder:
	mkdir -p build/web
	cp index.html build/web
	cp index.js build/web
	cp index.wasm build/web
	cp index.data build/web

copy-native-to-build-folder:
	mkdir -p build/native
	cp a.out build/native
	cp -r resources build/native

clean:
	rm -f src/index.js src/index.wasm src/index.data src/*.out