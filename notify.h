#ifndef NOTIFY_H
#define NOTIFY_H
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#if __linux__
#include "linux_notify.c"
#elif __APPLE__
#include "apple_notify.c"
#endif
int notify(char* head, char* message);
void ghost(bool debug);
#endif
