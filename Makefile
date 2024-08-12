CC = clang
LINKER_FLAGS = -L./lib -lSDL2

build:
	${CC} ${LINKER_FLAGS} src/main.c -o game
	./game
	rm -rf game
