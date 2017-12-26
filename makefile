CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g
LDFLAGS = lpthreads
OBJS = main.c notify.h 
OUT = awa

all:
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)
install:
	
	ln -s $(shell pwd)/awa /usr/bin/awa

list:
	ps ax  | egrep awa

kill:
	killall awa
