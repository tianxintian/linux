SOURCES:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,${SOURCES})
CFLAGS:=-I../include
main:${OBJS}
	gcc $^ -o $@ -pthread

clean:
	rm main ${OBJS}
