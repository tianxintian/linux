SOURCES:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SOURCES))
CFLAGS:=-g
CC:=gcc
ELF:=main
$(ELF):$(OBJS)
	gcc $^ -o $@

clean:
	rm -rf $(OBJS) $(ELF)

