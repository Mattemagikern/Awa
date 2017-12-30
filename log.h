#ifndef LOG_H 
#define LOG_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "log.c"
int write_to_log(char* type, char* message);
int print_log(int n);
#endif
