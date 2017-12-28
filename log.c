#include <stdio.h>
#include <string.h>

/*
 * Writes a message to .awa_log. 
 */
int write_to_log(char* head, char* message){
    FILE* fp;
    char buf[1024];
    char path[50];
    char* home;
    if (home = getenv("HOME"), home != NULL) {
        snprintf(path, sizeof(path), "%s%s", home, "/.awa_log");
        if (fp = fopen(path,"a"), fp != NULL){
            fprintf(fp, "------------\n");
            fprintf(fp, "%s\n",head);
            fprintf(fp, "%s\n",message);
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
