CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g
LDFLAGS =
OBJS = main.c notify.h log.h
OUT = awa

INSTALL_PATH?=/usr/local/bin

all:
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT) 
install:all
	mv $(OUT) $(INSTALL_PATH)

list:
	ps ax  | egrep awa

kill:
	killall awa
