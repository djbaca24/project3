#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
        // TODO
        // return 1 if cmd is one of the allowed commands
        // return 0 otherwise
        for (int i=0; i<N; i++){
            if (strcmp(cmd, allowed[i]) == 1){
                return 1;
            }
        }
        return 0;
}


void printHelp() {
    printf("The allowed commands are:\n");
    for (int i=0; i<N; i++) {
        printf("%d: %s\n", i + 1, allowed[i]);
        }
    }


int main() {

    // TODO
    // Add variables as needed
    char *argsArray[21];
    pid_t pid;
    int results;

    char line[256];

    while (1) {

        fprintf(stderr,"rsh>");

        if (fgets(line,256,stdin)==NULL) continue;

        if (strcmp(line,"\n")==0) continue;

        line[strlen(line)-1]='\0';

        // TODO
        // Add code to spawn processes for the first 9 commands
        // And add code to execute cd, exit, help commands
        // Use the example provided in myspawn.c
        int argCount = 0;
        char *tok = strtok(line, " ");

        while (tok != NULL && argCount < 20) {
            argsArray[argCount++] = tok;
            tok = strtok(NULL, " ");
        }

        argsArray[argCount] = NULL;

        if (!isAllowed(argsArray[0])) {
            printf("NOT ALLOWED!\n");
            continue;
        }

        if (strcmp(argsArray[0], "exit") == 0) {
            return 0;
        }else if (strcmp(argsArray[0], "help") == 0) {
            printHelp();
            continue;
        }else if (strcmp(argsArray[0], "cd") == 0) {
            if (argCount > 2) {
                printf("-rsh: cd: too many arguments\n");
            }else if (argCount == 1 || chdir(argsArray[1])!= 0) {
                perror("cd");
            }
            continue;
        }

        if (posix_spawnp(&pid, argsArray[0], NULL, NULL, argsArray, environ) != 0) {
            perror("posix_spawnp");
            continue;
        }

        if(waitpid(pid, &results, 0) == -1) {
            perror("waitpid");
        }

    }
    return 0;
}
