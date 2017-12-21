#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define DEBUG 0

int notify(char* head, char* message){
    char cwd[1024] = "";
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
        char message[2048];
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

int main(int argc, char const* argv[])
{
    FILE* fp;
    char buf[1024];
    char path[50];
    char* home;
    if (home = getenv("HOME"), home != NULL) {
        snprintf(path, sizeof(path), "%s%s", home, "/.awa");
        if (fp = fopen(path,"r"), fp != NULL){
            while(fgets(buf, sizeof(buf), fp) != NULL){
                if (strstr(buf,"path")) {
                    daemon(1,DEBUG);
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
    return 0;
}
