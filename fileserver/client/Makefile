SOURCES:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,${SOURCES})
main:${OBJS}
	gcc $^ -o $@ -lcrypt -lcrypto

clean:
	rm main ${OBJS}
