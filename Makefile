SFILES=main.cpp NetworkEntities.cpp MovingCube.cpp sniffer.cpp gframework/graphicsframework.cpp
LIBRARIES=-lGL -lGLEW -lglut -lSOIL -lpcap -lpthread
OUTPUT=network-visualizer

all:
	g++ $(SFILES) -o $(OUTPUT) $(LIBRARIES) -std=gnu++11
run:
	make
	./network-visualizer
