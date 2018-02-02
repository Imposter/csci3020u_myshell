/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2018 Rishabh Patel, Eyaz Rehman
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256

// Put global environment variables here

// Define functions declared in myshell.h here

int main(int argc, char *argv[])
{
    // Input buffer and and commands
    char buffer[BUFFER_LEN] = { 0 };
    char command[BUFFER_LEN] = { 0 };
    char arg[BUFFER_LEN] = { 0 };

    // Parse the commands provided using argc and argv

    // Perform an infinite loop getting command input from users
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {
        // Perform string tokenization to get the command and argument
        {
            // TODO/NOTE: There's a seg fault when nothing is written and return is pressed.

            // Get buffer length
            int len = strlen(buffer);
            if (!len) continue;

            // Clear buffers
            memset(command, 0, BUFFER_LEN);
            memset(arg, 0, BUFFER_LEN);

            // Copy command
            const char *ptr = strtok(buffer, " \n");
            len = strlen(ptr);
            strncpy(command, ptr, len);

            // Copy arg if any
            if ((ptr = strtok(NULL, " \n")) != NULL) 
            {
                len = strlen(ptr);
                strncpy(arg, ptr, len);
            }

#if 0
            printf("command: %s\r\n", command);
            printf("arg: %s\r\n", arg);
#endif
        }

        // Check the command and execute the operations for each command
        // cd command -- change the current directory
        if (strcmp(command, "cd") == 0)
        {
            // your code here
        }
        else if (strcmp(command, "clr") == 0)
        {
            printf("\033c");
        }
        else if (strcmp (command, "pause") == 0)
        {
            /*printf("Press ENTER to continue . . .");
            while (getchar() != '\n') 
                sleep(10);
            printf("Ready");*/
        }
        else if (strcmp(command, "help") == 0) 
        {
            FILE *file = fopen("README.md", "r");
            if (file == NULL) 
            {
                fputs("README.md not found!\r\n", stderr);
                continue;
            }

            char file_buffer[BUFFER_LEN];
            while (fread(file_buffer, sizeof(char), BUFFER_LEN - 1, file) > 0)
                printf(file_buffer);
            printf("\r\n");
        }
        else if (strcmp(command, "quit") == 0)
        {
            return EXIT_SUCCESS;
        }
        else
        {
            fputs("Unsupported command, use help to display the manual\r\n", stderr);
        }
    }
    return EXIT_SUCCESS;
}