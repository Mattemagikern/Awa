#if __linux__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int notify(char* head, char* message){
    char cwd[1024];
    char base[20] = "notify-send -i";
    char path[1024]; 
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        sprintf(cwd, "/proc/%d/exe", getpid());
        readlink(cwd, cwd, 1024);
        strcpy((cwd + strlen(cwd) - 4), "/awa.png");
        sprintf(path, "%s %s \"%s\" \"%s\"", base, cwd, head ,message);
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
