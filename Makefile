.PHONY: all native build copy-wasm-to-build-folder copy-native-to-build-folder clean 

native:
	$(MAKE) build COMPILER=g++

web:
	$(MAKE) build COMPILER=emcc

build: entity_factory.o game.o import_level.o world.o
	$(MAKE) COMPILER=$(COMPILER) -C src/entities
	$(MAKE) COMPILER=$(COMPILER) -C src/entity_properties
	$(MAKE) COMPILER=$(COMPILER) -C src/utils
	$(MAKE) COMPILER=$(COMPILER) -C src/visuals
	
	# This will now create build artifacts in root folder, while other put it in their src folder... oh well
ifeq ($(COMPILER),g++)
	$(COMPILER) src/main.cpp *.o src/entities/*.o src/entity_properties/*.o src/utils/*o src/visuals/*.o \
	-g -w -lSDL2 -lSDL2_ttf  -lSDL2_image -lSDL2_mixer -ldump_lib;
	$(MAKE) copy-native-to-build-folder
endif

ifeq ($(COMPILER),emcc)
	$(COMPILER) src/main.cpp *.o src/entities/*.o src/entity_properties/*.o src/utils/*o src/visuals/*.o \
 	-s USE_SDL=2 -s USE_SDL_IMAGE=2  -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='["png"]' -s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1 -s \
	--preload-file resources -o index.js;
	$(MAKE) copy-wasm-to-build-folder
endif
	
entity_factory.o: src/entity_factory.cpp src/entity_factory.hpp
	$(COMPILER) -c src/entity_factory.cpp

game.o: src/game.cpp src/game.hpp
	$(COMPILER) -c src/game.cpp

import_level.o: src/import_level.cpp src/import_level.hpp
	$(COMPILER) -c src/import_level.cpp

entity_factory.o: src/world.cpp src/world.hpp
	$(COMPILER) -c src/world.cpp

copy-native-to-build-folder:
	mkdir -p build/native
	cp a.out build/native
	cp -r resources build/native

copy-wasm-to-build-folder:
	mkdir -p build/web
	cp src/index.html build/web
	cp index.js build/web
	cp index.wasm build/web
	cp index.data build/web

clean:
	$(MAKE) clean -C src/entities
	$(MAKE) clean -C src/entity_properties
	$(MAKE) clean -C src/utils
	$(MAKE) clean -C src/visuals
	rm -f index.js index.wasm index.data *.a *.so *.o *.out