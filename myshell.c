/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2018 Thagshan Mohanarathnam, Rameet Sekhon, Rishabh Patel, Eyaz Rehman
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
#include <sys/stat.h>
#include <linux/limits.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"

// Put macros or constants here using #define
#define SHELL_NAME "MyShell by Eyaz Rehman and Rishabh Patel"
#define BUFFER_LEN 256

// Put global environment variables here
extern char **environ;
char current_path[PATH_MAX];
char shell_path[PATH_MAX];

int main(int argc, char *argv[])
{
    // Input buffer and and commands
    char buffer[BUFFER_LEN] = { 0 };
    char command[BUFFER_LEN] = { 0 };
    char arg[BUFFER_LEN] = { 0 };
    char output[BUFFER_LEN] = { 0 };

    // Get and store current directory
    getcwd(current_path, PATH_MAX);

    // Get shell path
    sprintf(shell_path, "%s/myshell", current_path);

    // Set environment variable for shell
    char env_shell[PATH_MAX + BUFFER_LEN];
    sprintf(env_shell, "shell=%s", shell_path);
    putenv(env_shell);

    // Check if commands were provided from a file
    FILE *input_stream = stdin;
    if (argc > 1)
    {
        // Open file and set it as the input stream
        char *file_path = argv[1];
        input_stream = fopen(file_path, "r");
        if (input_stream == NULL) 
        {
            fprintf(stderr, "%s not found!\r\n", file_path);
            return EXIT_FAILURE;
        }
    }

    fputs(SHELL_NAME "\r\n", stdout);

    // Perform an infinite loop getting command input from users
    while (true)
    {
        // Print prompt
        if (input_stream == stdin)
            printf("%s> ", current_path);

        // Read input
        if (fgets(buffer, BUFFER_LEN, input_stream) == NULL)
        {
            // Ignore EOF if reading from console, otherwise exit
            if (input_stream == stdin)
                continue;
            else break;
        }

        // Perform string tokenization to get the command and argument
        {
            // Clear buffers
            memset(command, 0, BUFFER_LEN);
            memset(arg, 0, BUFFER_LEN);
            memset(output, 0, BUFFER_LEN);

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
            if ((ptr = strtok(NULL, ">\n")) != NULL) 
            {
                len = strlen(ptr);
                strncpy(arg, ptr, len);
            }

            // Check if output redirect exists
            if ((ptr = strtok(NULL, "\n")) != NULL) 
            {
                len = strlen(ptr);
                strncpy(output, ptr, len);
            }
        }

        // If there's an output path, open the redirection
        FILE *output_stream = stdout;
        if (strlen(output) > 0)
        {
            // Open file and set it as the output stream
            output_stream = fopen(output, "w+");
            if (output_stream == NULL) 
            {
                fprintf(stderr, "%s could not be opened!\r\n", output);
                continue;
            }
        }

        // Check the command and execute the operations for each command
        if (strcmp(command, "echo") == 0) 
        {
            fprintf(output_stream, "%s\r\n", arg);
        } 
        else if (strcmp(command, "cd") == 0)
        {
            if (strlen(arg) < 1) 
            {
                fputs("Invalid directory\r\n", stderr);
                continue;
            }

            char abs_path[PATH_MAX];

            // Check if path entered is relative or not
            if (arg[0] == '.') 
            {
                // Add current directory to abs_path first
                strncpy(abs_path, current_path, PATH_MAX);
                strncat(abs_path, arg, PATH_MAX - strlen(abs_path));
            }
            else
            {
                strcpy(abs_path, arg);
            }

            // Resolve path
            if (realpath(arg, abs_path) == NULL)
            {
                fputs("Unable to resolve directory\r\n", stderr);
                continue;
            }

            // Check if path exists
            struct stat s;
            if (stat(abs_path, &s) != 0 || !S_ISDIR(s.st_mode))
            {
                fputs("Directory does not exist\r\n", stderr);
                continue;
            }

            // Set current directory
            strcpy(current_path, abs_path);

            // Set working directory
            int ret;
            if ((ret = chdir(current_path)) != 0) 
            {
                fprintf(stderr, "Unable to change directory (error: %d)\r\n", ret);
            }
        }
        else if (strcmp(command, "dir") == 0 || strcmp(command, "ls") == 0) 
        {
            // Print out data from the current working directory
            DIR *directory = opendir(current_path);
            struct dirent *directory_entry;
            while ((directory_entry = readdir(directory)) != NULL) 
            {
                if (strcmp(directory_entry->d_name, ".") == 0
                    || strcmp(directory_entry->d_name, "..") == 0)
                {
                    // Don't print . and ..
                    continue;
                }
                
                fprintf(output_stream, "%s\r\n", directory_entry->d_name);
            }

            closedir(directory);
        }
        else if (strcmp(command, "environ") == 0 || strcmp(command, "env") == 0) 
        {
            char **ptr = environ;
            while (*ptr != NULL) 
            {
                // Print var
                fprintf(output_stream, "%s\r\n", *ptr);
                ptr++;
            }
        }
        else if (strcmp(command, "clr") == 0 || strcmp(command, "cls") == 0)
        {
            printf("\033c");
        }
        else if (strcmp (command, "pause") == 0)
        {
            printf("Press ENTER to continue . . .");

            // Keep reading characters until a newline is received
            while (fgets(buffer, BUFFER_LEN, stdin) == NULL);
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
                fprintf(output_stream, "%s", file_buffer);
            fprintf(output_stream, "\r\n");
        }
        else if (strcmp(command, "quit") == 0)
        {
            return EXIT_SUCCESS;
        }
        else
        {
            fputs("Unsupported command, use help to display the manual\r\n", stderr);
        }

        // If redirection file is open, close it
        if (output_stream != stdout)
            fclose(output_stream);
    }

    if (input_stream != stdin) 
        fclose(input_stream);

    return EXIT_SUCCESS;
}