.PHONY: all native clean 

all: behavior.o collision.o enemy_behavior.o golf_mode.o graphic.o item_behavior.o

behavior.o: behavior.cpp behavior.hpp
	$(COMPILER) -g -c behavior.cpp

collision.o: collision.cpp collision.hpp
	$(COMPILER) -g -c collision.cpp

enemy_behavior.o: enemy_behavior.cpp enemy_behavior.hpp
	$(COMPILER) -g -c enemy_behavior.cpp

golf_mode.o: golf_mode.cpp golf_mode.hpp
	$(COMPILER) -g -c golf_mode.cpp

graphic.o: graphic.cpp graphic.hpp
	$(COMPILER) -g -c graphic.cpp

item_behavior.o: item_behavior.cpp item_behavior.hpp
	$(COMPILER) -g -c item_behavior.cpp

clean: 
	rm -f *.o