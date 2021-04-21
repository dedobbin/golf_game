.PHONY: all native clean 

all: behavior.o collision.o enemy_behavior.o golf_mode.o graphic.o item_behavior.o

behavior.o: behavior.cpp behavior.hpp
	$(COMPILER) -c behavior.cpp

collision.o: collision.cpp collision.hpp
	$(COMPILER) -c collision.cpp

enemy_behavior.o: enemy_behavior.cpp enemy_behavior.hpp
	$(COMPILER) -c enemy_behavior.cpp

golf_mode.o: golf_mode.cpp golf_mode.hpp
	$(COMPILER) -c golf_mode.cpp

graphic.o: graphic.cpp graphic.hpp
	$(COMPILER) -c graphic.cpp

item_behavior.o: item_behavior.cpp item_behavior.hpp
	$(COMPILER) -c item_behavior.cpp

clean: 
	rm -f *.o