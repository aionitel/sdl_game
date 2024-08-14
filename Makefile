CC = clang
INCLUDE = -I./include
LIBS = -L./lib -lSDL2 -ldl
ENTRY = src/main.c

build:
	${CC} ${LIBS} ${INCLUDE} ${ENTRY} -o game
	./game
	rm -rf game
