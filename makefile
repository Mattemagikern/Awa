CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g
LDFLAGS =
OBJS = main.c notify.h log.h
OUT = awa
DESTDIR?=/usr/local/bin

all:
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT) 
install:all
	mv $(OUT) $(DESTDIR)

list:
	ps ax  | egrep awa

kill:
	killall awa
