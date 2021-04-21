.PHONY: all native build copy-wasm-to-build-folder copy-native-to-build-folder clean 

native:
	$(MAKE) build COMPILER=g++

build: entity_factory.o game.o import_level.o world.o
	$(MAKE) COMPILER=$(COMPILER) -C src/entities
	$(MAKE) COMPILER=$(COMPILER) -C src/entity_properties
	$(MAKE) COMPILER=$(COMPILER) -C src/utils
	$(MAKE) COMPILER=$(COMPILER) -C src/visuals
	$(COMPILER) src/main.cpp *.o src/entities/*.o src/entity_properties/*.o src/utils/*o src/visuals/*.o \
	-g -w -lSDL2 -lSDL2_ttf  -lSDL2_image -lSDL2_mixer -ldump_lib;
	
entity_factory.o: src/entity_factory.cpp src/entity_factory.hpp
	$(COMPILER) -c src/entity_factory.cpp

game.o: src/game.cpp src/game.hpp
	$(COMPILER) -c src/game.cpp

import_level.o: src/import_level.cpp src/import_level.hpp
	$(COMPILER) -c src/import_level.cpp

entity_factory.o: src/world.cpp src/world.hpp
	$(COMPILER) -c src/world.cpp

clean:
	$(MAKE) clean -C src/entities
	$(MAKE) clean -C src/entity_properties
	$(MAKE) clean -C src/utils
	$(MAKE) clean -C src/visuals
	rm -f index.js index.wasm index.data *.a *.so *.o *.out