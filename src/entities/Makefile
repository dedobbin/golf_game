.PHONY: all native clean 

all: entity.o item.o living_entity.o

entity.o: entity.cpp entity.hpp
	$(COMPILER) -c entity.cpp

item.o: item.cpp item.hpp
	$(COMPILER) -c item.cpp

living_entity.o: living_entity.cpp living_entity.hpp
	$(COMPILER) -c living_entity.cpp

clean: 
	rm -f *.o