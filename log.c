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
            fprintf(fp, "------------\n");
            fprintf(fp, "%s\n",head);
            fprintf(fp, "%s\n",message);
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
 * prints n or all logs from .awa_log. 
 */
int print_log(int n){
    FILE* fp;
    char buf[1024];
    char path[1024];
    char* home;
    if (n == 0) return 0;
    if (home = getenv("HOME"), home != NULL) {
        snprintf(path, sizeof(path), "%s%s", home, "/.awa_log");
        if ((fp = fopen(path,"r"), fp != NULL)){
            FILE* fp_find = fopen(path, "r");
            int i = 0;
            //how many updates is there in .awa_log?
            while(fgets(buf, sizeof(buf), fp_find) != NULL)
                if(strstr(buf,"-----------"))
                    i++;
            fclose(fp_find);
            // if i > n then wait untill update i-n to start print, else 
            // print all lines.
            if(n > 0)
                n = i > n ? i - n: -1;
            while(fgets(buf, sizeof(buf), fp) != NULL){
                if( n < 0 )
                    printf("%s", buf);
                if (strstr(buf,"----"))
                    n --; 
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
