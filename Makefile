.PHONY: all native build export-native export-wasm export-wasm-with-editor clean 

native:
	$(MAKE) build COMPILER=g++

web:
	$(MAKE) build COMPILER=emcc

build: entity_factory.o game.o import_level.o world.o
	$(MAKE) COMPILER=$(COMPILER) -C src/entities
	$(MAKE) COMPILER=$(COMPILER) -C src/entity_properties
	$(MAKE) COMPILER=$(COMPILER) -C src/utils
	$(MAKE) COMPILER=$(COMPILER) -C src/visuals
	
	@# This will now create build artifacts in root folder, while other put it in their src folder... oh well
ifeq ($(COMPILER),g++)
	$(COMPILER) src/main.cpp *.o src/entities/*.o src/entity_properties/*.o src/utils/*o src/visuals/*.o \
	-g -w -lSDL2 -lSDL2_ttf  -lSDL2_image -lSDL2_mixer -ldump_lib;
	$(MAKE) export-native
endif

ifeq ($(COMPILER),emcc)
	$(COMPILER) src/main.cpp *.o src/entities/*.o src/entity_properties/*.o src/utils/*o src/visuals/*.o \
 	-s USE_SDL=2 -s USE_SDL_IMAGE=2  -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='["png"]' \
	-s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1 \
	-s EXPORTED_RUNTIME_METHODS=["cwrap","setValue"] \
	--preload-file resources -o wasm.js;
	$(MAKE) export-wasm-with-editor
endif
	
entity_factory.o: src/entity_factory.cpp src/entity_factory.hpp
	$(COMPILER) -g -c src/entity_factory.cpp

game.o: src/game.cpp src/game.hpp
	$(COMPILER) -g -c src/game.cpp

world.o: src/world.cpp src/world.hpp
	$(COMPILER) -g -c src/world.cpp

import_level.o: src/import_level.cpp src/import_level.hpp
	$(COMPILER) -g -c src/import_level.cpp

export-native:
	rm -fr build/native
	mkdir build/native
	cp a.out build/native
	cp -r resources build/native

export-wasm-with-editor:
	rm -fr build/web
	mkdir build/web
	cp -r src/web-frontend/level_editor/* build/web
	cp wasm.js build/web/script #(WASM-glue)
	cp wasm.wasm build/web/script
	cp wasm.data build/web
	@# Spritesheets are also embedded in the WASM module, but needs to be duplicated here so level editor JS can also use them
	mkdir -p build/web/resources/spritesheets
	cp resources/spritesheets/* build/web/resources/spritesheets

clean:
	$(MAKE) clean -C src/entities
	$(MAKE) clean -C src/entity_properties
	$(MAKE) clean -C src/utils
	$(MAKE) clean -C src/visuals
	rm -f wasm.js wasm.wasm wasm.data *.a *.so *.o *.out