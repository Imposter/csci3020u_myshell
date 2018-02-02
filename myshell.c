/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2018 Rishabh Patel, Eyaz Rehman
 * All rights reserved.
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
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
            // Clear buffers
            memset(command, 0, BUFFER_LEN);
            memset(arg, 0, BUFFER_LEN);

            // Get buffer length
            int len = strlen(buffer);

            // Check if the entire string is just whitespace
            bool is_whitespace = true;
            for (int i = 0; i < len; i++) 
            {
                if (!isspace(buffer[i]))
                {
                    is_whitespace = false;
                    break;
                }
            }

            // If it is, then we don't need to handle it
            if (is_whitespace)
                continue;

            // Replace newline with null terminator
            buffer[len - 1] = '\0';

            // Copy command
            const char *ptr = strtok(buffer, " \n");
            len = strlen(ptr);
            strncpy(command, ptr, len);

            // Copy arg if any
            if ((ptr = strtok(NULL, "\n")) != NULL) 
            {
                len = strlen(ptr);
                strncpy(arg, ptr, len);
            }
        }

        // Check the command and execute the operations for each command
        if (strcmp(command, "echo") == 0) 
        {
            printf("%s\r\n", arg);
        } 
        else if (strcmp(command, "cd") == 0)
        {
            // your code here
        }
        else if (strcmp(command, "clr") == 0)
        {
            printf("\033c");
        }
        else if (strcmp (command, "pause") == 0)
        {
            printf("Press ENTER to continue . . .");

            // TODO: ...
            // Keep reading characters until a newline is received
            //char c; 
            //do 
            //{
            //    fgets(&c, sizeof(char), stdin);
            //} while (c != '\n');
            //printf("Ready\r\n");
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