SOURCES:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,${SOURCES})
CFLAGS:=-g -I../include -I/usr/include/mysql
main:${OBJS}
	gcc $^ -o $@ -pthread -lmysqlclient

clean:
	rm main ${OBJS}
