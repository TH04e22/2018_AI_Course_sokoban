.PHONY: all clean

OBJ = main.o
SRC = main.cpp
all: main

main: clean ${OBJ} sokoban.o agent.o
	g++ -o main ${OBJ} sokoban.o agent.o

sokoban.o: sokoban.cpp
	g++ -c sokoban.cpp

agent.o: agent.cpp
	g++ -c agent.cpp

${OBJ}: ${SRC}
	g++ -c ${SRC}

clean:
	rm -f main *.o
