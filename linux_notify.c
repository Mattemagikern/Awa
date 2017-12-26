#if __linux__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
/*
 * Trims white spaces in string
 */
void trim_spaces(char *str){
    char* dest = str;
    do 
        while(isspace(*str) && isspace(*(str + 1)))
            str++; 
    while(*dest++ = *str++);
}
/*
 * Sends a notify using the linux notify-send.
 * Assumes the function was called from this project root. 
 */
int notify(char* head, char* message){
    char cwd[1024]="";
    char base[20] = "notify-send -i";
    char path[1024]="";
    trim_spaces("hello   I");
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        //sprintf(cwd, "/proc/%d/exe", getpid());
        //readlink(cwd, cwd, 1024);
        //strcpy((cwd + strlen(cwd) - strlen(__FILE__)), "/awa.png");
        strcat(cwd,"/awa.png");
        sprintf(path, "%s %s \"%s\" \"%s\"", base, cwd, head ,message);
        printf("%s\n", path);
        system(path);
    }else{
        perror("getcwd() error");
        return 1;
    }
    return 0;
}

void ghost(bool debug){
    daemon(1,debug);
}
#endif
