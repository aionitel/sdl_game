CC = clang
INCLUDE = -I./include
LIBS = -L./lib -lSDL2 -ldl
SRC_FILES = src/main.c src/glad.c
ENTRY = src/main.c src/glad.c

build:
	${CC} ${LIBS} ${INCLUDE} ${ENTRY} -o game
	./game
	rm -rf game
