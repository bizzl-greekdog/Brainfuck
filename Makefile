#!/usr/ccs/bin/make

CC = gcc
COPTS = -Wall
CLIBS =
RM = /bin/rm

all: brainfuck

brainfuck: brainfuck.o
	${CC} ${COPTS} ${CLIBS} -o brainfuck brainfuck.o

brainfuck.o: brainfuck.c
	${CC} ${COPTS} ${CLIBS} -c -o brainfuck.o brainfuck.c

clean:
	ls *.o			>> /dev/null 2>&1 && ${RM} *.o			|| true
	ls brainfuck		>> /dev/null 2>&1 && ${RM} brainfuck		|| true
