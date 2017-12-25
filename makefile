CC = gcc
CflAGS = -Wall -g -pedantic -std=c99 
LDFLAGS =
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
