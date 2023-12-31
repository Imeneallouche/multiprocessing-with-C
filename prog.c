#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Function to display a message in a box
void displayMessageInBox(const char *message) {
    int length = strlen(message);
    printf("\n+");
    for (int i = 0; i < length + 4; i++) {
        printf("-");
    }
    printf("+\n");
    printf("|  %s  |\n", message);
    printf("+");
    for (int i = 0; i < length + 4; i++) {
        printf("-");
    }
    printf("+\n");
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source_file.c>\n", argv[0]);
        return 1;
    }

    // Extract the base name of the source file (without the extension)
    char *source_file = argv[1];
    char *base_name = strrchr(source_file, '/');
    if (base_name == NULL) {
        base_name = source_file;
    } else {
        base_name++; // Skip the '/' character
    }

    // Generate the names of the object file and the executable file
    char object_file[256];
    char executable_file[256];
    snprintf(object_file, sizeof(object_file), "%s.o", base_name);
    snprintf(executable_file, sizeof(executable_file), "prog_%s", base_name);

    // Create P1 process to compile the source file into an object file
    pid_t pid_P1 = fork();

    if (pid_P1 < 0) {
        perror("P1 fork failed");
        return 1;
    } else if (pid_P1 == 0) {
        // P1 child process

        // Compile the source file to generate an object file
        execlp("gcc", "gcc", "-c", source_file, "-o", object_file, NULL);

        // If execlp fails, it means there was an error in the compilation
        perror("P1 compilation failed");
        exit(1);
    }

    int status_P1;
    wait(&status_P1);

    if (WIFEXITED(status_P1) && WEXITSTATUS(status_P1) == 0) {
        // P1 completed successfully

        // Create P2 process to link the object file into an executable
        pid_t pid_P2 = fork();

        if (pid_P2 < 0) {
            perror("P2 fork failed");
            return 1;
        } else if (pid_P2 == 0) {
            // P2 child process

            // Link the object file to create an executable
            execlp("gcc", "gcc", object_file, "-o", executable_file, NULL);

            // If execlp fails, it means there was an error in the linking
            perror("P2 linking failed");
            exit(1);
        }

        int status_P2;
        wait(&status_P2);

        if (WIFEXITED(status_P2) && WEXITSTATUS(status_P2) == 0) {
            // P2 completed successfully

            // Create P3 process to execute the program generated by P2
            pid_t pid_P3 = fork();

            if (pid_P3 < 0) {
                perror("P3 fork failed");
                return 1;
            } else if (pid_P3 == 0) {
                // P3 child process

                // Execute the program generated by P2
                // Display a message at the start of the program
                displayMessageInBox("THE PROGRAM WILL START EXECUTING RIGHT NOW");
                execl(executable_file, executable_file, NULL);


                // If execl fails, it means there was an error in program execution
                printf("\n\n");
                perror("P3 execution failed");
                exit(1);
            }

            int status_P3;
            wait(&status_P3);
            // Display a message at the end of the program
            displayMessageInBox("THE PROGRAM HAS ENDED RIGHT IN HERE");

            if (WIFEXITED(status_P3)) {
                printf("\n P3 exited with status %d\n", WEXITSTATUS(status_P3));
            } else if (WIFSIGNALED(status_P3)) {
                printf("P3 terminated abnormally with signal %d\n", WTERMSIG(status_P3));
            }
        } else {
            // P2 terminated abnormally
            printf("P2 exited with status %d\n", WEXITSTATUS(status_P2));
        }
    } else {
        // P1 terminated abnormally
        printf("P1 exited with status %d\n", WEXITSTATUS(status_P1));
    }

    return 0;
}

