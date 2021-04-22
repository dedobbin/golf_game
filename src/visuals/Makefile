.PHONY: all native clean 

all: camera.o visuals.o

camera.o: camera.cpp camera.hpp
	$(COMPILER) -g -c camera.cpp

visuals.o: visuals.cpp visuals.hpp
	$(COMPILER) -g -c visuals.cpp

clean: 
	rm -f *.o