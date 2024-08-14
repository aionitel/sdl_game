CC = clang
INCLUDE = -I./include
LIBS = -L./lib -lSDL2 -ldl
SRC_FILES = src/main.c src/glad.c
ENTRY = src/main.c src/glad.c
FRAMEWORK = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation

build:
	${CC} ${LIBS} ${INCLUDE} ${ENTRY} ${FRAMEWORK} -o game
	./game
	rm -rf game
