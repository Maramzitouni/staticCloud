CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

LIB = -lSDL2

BIN = ./code/app.c

DEST = ./executable/app.exe

all:
	${CC} ${CFLAGS} ${BIN} -o ${DEST} ${LIB}

clean:
	${RM} ${OBJ} ${DEST}
