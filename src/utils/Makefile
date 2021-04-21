.PHONY: all native clean 

all: utils.o sdl_utils.o

utils.o: utils.cpp utils.hpp
	$(COMPILER) -c utils.cpp

sdl_utils.o: sdl_utils.cpp sdl_utils.hpp
	$(COMPILER) -c sdl_utils.cpp

clean: 
	rm -f *.o