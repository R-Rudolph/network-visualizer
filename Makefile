SFILES=main.cpp NetworkEntities.cpp MovingCube.cpp sniffer.cpp gframework/graphicsframework.cpp
LIBRARIES=-lGL -lGLEW -lglut -lSOIL -lpcap -lpthread
OUTPUT=network-visualizer

all:
	g++ $(SFILES) -o $(OUTPUT) $(LIBRARIES) -std=gnu++11
	sudo setcap cap_net_raw+ep network-visualizer
run:
	make
	./network-visualizer
