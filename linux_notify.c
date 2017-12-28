#if __linux__
#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

/*
 * Trims white spaces in string
 * Assumes that it's not a litteral string. 
 */
void trim_spaces(char* str){
    char* dest = str;
    do 
        while(isspace(*str) && isspace(*(str + 1)))
            str++; 
    while((*dest++ = *str++));
}

/*
 * Sends a notify using the linux notify-send.
 * Assumes the function was called from this project root. 
 */
int notify(char* head, char* message){
    char cwd[1024] = "";
    char base[20] = "notify-send -i";
    char path[1024] = "";
    trim_spaces(message);
    if (sprintf(cwd, "/proc/%d/exe", getpid()) > -1){
        readlink(cwd, cwd, 1024);
        strcpy((cwd + strlen(cwd) - 4), "/awa.png");
        sprintf(path, "%s %s \"%s\" \"%s\"", base, cwd, head ,message);
        system(path);
    }else{
        perror("sprintf /proc/ error");
        return 1;
    }
    return 0;
}

void ghost(bool debug){
    daemon(1,debug);
}
#endif
