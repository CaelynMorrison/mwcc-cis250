#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 32
#define ARG_COUNT 5

/*
char* parse_command(char *line) {
    if (line[0] == '#') {
            return NULL;
        }
    char *command = strtok(current_line, " ");
    if (command == NULL) {
        return NULL;
    }
    char *arg_array[ARG_COUNT];
    int count = 0;
    while (command != NULL && count < ARG_COUNT) {
        arg_array[count] = command;
        count++;
        arg_array[count] = NULL;
        command = strtok(NULL, " ");
    }
}
*/


int run_test(char **args) {
    pid_t pid = fork();
    //printf("%i\n", pid);
    if (pid == -1) {
        perror("Fork Failed\n");
        return 1;
    } else if (pid == 0) {
        printf("Running the %s command.\n", args[0]);
        execvp(args[0], args);
        perror("Exec failed");
        exit(1);
        return 1;
    } else {
        printf("Waiting...\n");
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Finished with status %d\n", WEXITSTATUS(status));
            if (WEXITSTATUS(status) != 0) {
                return 1;
            }
        }
    }
    return 0;

}


int main() {
    int tests_count = 0;
    int tests_fail = 0;

    FILE *tests_file =  fopen("tests.txt", "r");
    if (tests_file == NULL) {
        perror("open");
        return 1;
    }
    
    char current_line[BUFFER_SIZE];
    while (fgets(current_line, BUFFER_SIZE, tests_file) != NULL) {
        
        char *arg_array[ARG_COUNT];
        int count = 0;
        current_line[strcspn(current_line, "\r\n")] = '\0';
        if (current_line[0] == '#' || strlen(current_line) == 0) {
            continue;
        }
        char *command = strtok(current_line, " ");

        while (command != NULL && count < ARG_COUNT) {
            arg_array[count] = command;
            count++;
            arg_array[count] = NULL;
            command = strtok(NULL, " ");
        }
        tests_count++;
        int result = run_test(arg_array);
        tests_fail += result;
              
    }

    if (fclose(tests_file) != 0) {
        perror("Error closing test plan.");
        return 1;
    };

    printf("================================\n");
    printf(" TEST SUMMARY\n");
    printf("================================\n");
    printf("Tests Executed: %i\n", tests_count);
    printf("Tests Passed: %i\n", tests_count - tests_fail);
    printf("Tests Failed: %i\n", tests_fail);
    printf("================================\n");

    return 0;

    
}