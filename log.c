#include <stdio.h>
#include <string.h>

/*
 * Writes a message to .awa_log. 
 */
int write_to_log(char* head, char* message){
    FILE* fp;
    char path[50];
    char* home;
    if (home = getenv("HOME"), home != NULL) {
        snprintf(path, sizeof(path), "%s%s", home, "/.awa_log");
        if ((fp = fopen(path,"a"))){
            fprintf(fp, "%s\n",head);
            fprintf(fp, "%s\n",message);
            fprintf(fp, "------------\n");
            fclose(fp);
        }else{
            printf("Could not open ~/.awa\n");
            return 1;
        }
    }else{
        printf("Could not locate Home\n");
        return 1;
    }
    return 0;
}

/*
 * prints n logs from .awa_log. 
 */
int print_log(int n){
    FILE* fp;
    char buf[1024];
    char path[1024];
    char* home;
    n = n*3;
    if (home = getenv("HOME"), home != NULL) {
        snprintf(path, sizeof(path), "%s%s", home, "/.awa_log");
        if ((fp = fopen(path,"r"), fp != NULL)){
            while(fgets(buf, sizeof(buf), fp) != NULL && n){
                printf("%s", buf);
                n--;
            }
            fclose(fp);
        }else{
            printf("couldn't read .awa_log\n");
            return 1;
        }
    }else{
        printf("couldn't locate your home folder\n");
        return 1;
    }
    return 0;
}
