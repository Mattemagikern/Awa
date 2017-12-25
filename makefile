CC = gcc
CflAGS = -Wall -g -pedantic -std=c99 
LDFLAGS =
OBJS = main.c linux_notify.c 
OUT = awa

all:
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)
install:
	
	ln -s $(shell pwd)/awa /usr/bin/awa

list:
	ps ax  | egrep ./a.out

kill:
	killall awa
