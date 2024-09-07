CC = clang
INCLUDE = -I./include include/glad/glad.c
LIBS = -L./lib -lSDL2 -ldl
SRC_FILES = src/main.c
FRAMEWORK = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation

build:
	${CC} ${LIBS} ${INCLUDE} ${SRC_FILES} ${FRAMEWORK} -o game
	./game
	rm -rf game
