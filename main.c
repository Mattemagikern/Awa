#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "notify.h"

#define DEBUG 0
int check_status(){
    FILE* fp;
    char output[1024];
    char last_commit[256] = "";
    char commit[256];

    if(fp = popen("git log -n 1", "r"), fp != NULL){
        while (fgets(output, sizeof(output)-1, fp) != NULL) {
            if(strstr(output,"commit"))
                strcpy(last_commit, (output + 7));
        }
        pclose(fp);
    }

    while (1) {
        char message[1024];
        if(!system("git fetch --all")){
            if(fp = popen("git log -n 1", "r"), fp != NULL){
                while(fgets(output, sizeof(output)-1, fp) != NULL) {
                    if(strstr(output,"commit"))
                        strcpy(commit, (output + 7));
                    else if(strstr(output,"Date"))
                        strncat(message, output,strlen(output) - 7);
                    else
                        strcat(message,output);
                }
                if(strcmp(commit, last_commit)){
                    notify("New commit!",message);
                    strcpy(last_commit, commit);
                }
                pclose(fp);
            }else{
                return 1;
            }
        }
        memset(message, 0, sizeof(message));
        sleep(2);
    }
}

void* watch(char* path){
    if(!chdir(path)){
        notify("Hello, I'm Awa", "I'll keep you posted!");
        if(check_status())
            printf("check_status: error\n");
    }else{
        printf("error changing dir\n");
    }
}

void add_watcher(char* new_path){
    FILE* fp;
    char buf[1024];
    char* home;
    char path[1024];
    if (home = getenv("HOME"), home != NULL) {
        snprintf(path, sizeof(path), "%s%s", home, "/.awa");
        if (fp = fopen(path,"a"), fp != NULL){
            fprintf(fp, "path %s\n", new_path);
        }else{
            printf("Could not open ~/.awa , exits\n");
        }
    }else{
        printf("Could not find home\n");
    }
}

void remove_watcher(const char* new_path){
    FILE* fp;
    FILE* fpnew;
    char buf[1024];
    char path[1024];
    char temp[1024];
    char* home;
    if (home = getenv("HOME"), home != NULL) {
        snprintf(path, sizeof(path), "%s%s", home, "/.awa");
        snprintf(temp, sizeof(temp), "%s%s", home, "/.temp");
        if ((fp = fopen(path,"rw"), fp != NULL) && 
                (fpnew = fopen(temp,"ab+"), fpnew != NULL )){
            while(fgets(buf, sizeof(buf), fp) != NULL){
                if(strstr(buf, new_path) == NULL)
                    fprintf(fpnew, "%s", buf);
            }
            fclose(fp);
            fclose(fpnew);
            if(rename(strcat(home,"/.temp"),path))
                printf("error renaming\n");
        }else{
            printf("Could not open ~/.awa , exits\n");
            exit(1);
        }
    }else{
        printf("couldn't locate your home directory\n");
        exit(1);
    }
}

int main(int argc, char const* argv[]){
    if (argc > 1) {
        if (!strcmp(argv[1],"kill")) {
            //need a better solution to the kill command. 
            system("killall awa");
        } else if (!strcmp(argv[1],"add")) {
            add_watcher((char*) argv[2]);
        } else if (!strcmp(argv[1],"remove")) {
            remove_watcher((char*) argv[2]);
        }
    } else {
        FILE* fp;
        char buf[1024];
        char path[50];
        char* home;
        ghost(DEBUG);
        if (home = getenv("HOME"), home != NULL) {
            snprintf(path, sizeof(path), "%s%s", home, "/.awa");
            if (fp = fopen(path,"r"), fp != NULL){
                while(fgets(buf, sizeof(buf), fp) != NULL){
                    if (strstr(buf,"path")) {
                        buf[strlen(buf)-1] = 0;
                        watch(buf + 5);
                    }
                }
            }else{
                printf("Could not open ~/.awa , exits\n");
                return 1;
            }
        }else{
            printf("couldn't locate your home directory\n");
            return 1;
        }
    }
    return 0;
}
