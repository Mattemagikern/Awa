#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "notify.h"
#include "log.h"

#define DEBUG 0
int check_status(char* path, char* branch){
    FILE* fp;
    char output[50];
    char last_commit[256] = "";
    char commit[256]= "";
    char message[1024] = "";
    char head[100] = "New commit in ";
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "git %s %s", "rev-parse", branch);
    if(fp = popen("git rev-parse origin/master", "r"), fp != NULL){
        while (fgets(output, sizeof(output)-1, fp) != NULL)
            strcpy(last_commit,output);
        pclose(fp);
    }

    snprintf(cmd, sizeof(cmd), "git %s %s", "log -n 1 ..", branch);
    while (1) {
        if(!system("git fetch --all")){
            if(fp = popen("git log -n 1 ..origin/master", "r"), fp != NULL){
                while(fgets(output, sizeof(output)-1, fp) != NULL) {
                    if(strstr(output,"commit"))
                        strcpy(commit, (output + 7));
                    else if(strstr(output,"Date"))
                        strncat(message, output, strlen(output) - 7);
                    else
                        strcat(message,output);
                }
                if(strlen(commit) > 0 && strcmp(commit, last_commit)){
                    snprintf(head, sizeof(head), "%s %s!", "New commit in", path);
                    notify(head, message);
                    write_to_log(head, message);
                    strcpy(last_commit, commit);
                    get_status(path);
                }
                pclose(fp);
            }else{
                return 1;
            }
        }
        memset(message, 0, sizeof(message));
        struct timespec ts;
        ts.tv_sec = 10;
        ts.tv_nsec = 5;
        nanosleep(&ts, NULL);
    }
}

int get_status(char* name){
    FILE* fp;
    char output[100];
    char message[1024] = "";
    char* substr;
    char head_log[100] = "Summary for: ";
    strcat(head_log, name);

    if(fp = popen("git branch -vv", "r"), fp != NULL){
        while(fgets(output, sizeof(output)-1, fp) != NULL) {
            if ((substr = strstr(output,"]"))){
                output[strlen(output) - strlen(substr) + 1] = 0;
                strcat(output,"\n");
                strcat(message,output);
            }
        }

        fclose(fp);
        notify(name, message);
        write_to_log(head_log, message);
    }else{
        printf("error get_status\n");
        return 1;
    }
    return 0;
}

void watch(char* path, char* branch){
    char name[100] = "";
    printf("%s\n", path);
    if(!chdir(path)){
        strcat(name,strrchr(path,(int)'/') + 1);
        system("git fetch --all");
        printf("path: %s\n", path);
        printf("branc: %s\n", branch);
        get_status(name);
        if(check_status(name,branch))
            printf("check_status: error\n");
    }else{
        printf("error changing dir\n");
    }
}

void add_watcher(char* new_path, char* branch){
    FILE* fp;
    char* home;
    char path[1024];
    if (home = getenv("HOME"), home != NULL) {
        snprintf(path, sizeof(path), "%s%s", home, "/.awa");
        if (fp = fopen(path,"a"), fp != NULL){
            if(branch != NULL)
                fprintf(fp, "%s %s\n", new_path, branch);
            else
                fprintf(fp, "%s origin/master\n", new_path);
            fclose(fp);
            ghost(DEBUG);
            watch(new_path, branch);
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
        char* ptr;
        int n = -1;
        if (!strcmp(argv[1],"kill")) {
            //need a better solution to the kill command. 
            system("killall awa");
        } else if (!strcmp(argv[1],"add")) {
            if (argc  > 2 )
                add_watcher((char*) argv[2], (char*) argv[3]);
            else
                add_watcher((char*) argv[2], NULL);

        } else if (!strcmp(argv[1],"remove")) {
            remove_watcher((char*) argv[2]);
        }else if(!strcmp(argv[1],"log")){
            if (argc  > 3 && (ptr = strstr(argv[2],"-n"))){
                n = atoi(argv[3]);
            }
            print_log(n);
        }
    } else {
        FILE* fp;
        char buf[1024];
        char* home;
        char awa[50];
        char* branch;
        char* path;
        pid_t b;
        char errormsg[50] = "An error occurred, exits";
        ghost(DEBUG);
        if (home = getenv("HOME"), home != NULL) {
            snprintf(awa, sizeof(awa), "%s%s", home, "/.awa");
            snprintf(buf, sizeof(buf), "%s%s", home, "/.awa_log");
            fp = fopen(buf, "w+");
            fclose(fp);

            char str[25] = "I'll keep you posted!";
            if(notify("Hello, I'm Awa", str))
                printf("notify error\n");

            if (fp = fopen(awa,"r"), fp != NULL){
                while(fgets(buf, sizeof(buf)-1, fp) != NULL){
                    path = strtok(buf," ");
                    branch = strtok(NULL," ");
                        //fork the process. 
                        if(!(b = fork()) && b != -1){
                            branch[strlen(branch)-1] = 0;
                            watch(path, branch);
                            break;
                        }
                    memset(buf,0, sizeof(buf));
                }
                fclose(fp);
                while(1);
            }else{
                printf("Could not open ~/.awa , exits\n");
                notify("Godbye!",errormsg);
                return 1;
            }
        }else{
            printf("couldn't locate your home directory\n");
            notify("Godbye!",errormsg);
            return 1;
        }
    }
    return 0;
}
