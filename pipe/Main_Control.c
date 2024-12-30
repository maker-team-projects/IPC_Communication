#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

#define DATA_SIZE 5
#define BUFFER_SIZE 1000
#define fd_n 5
#define previous_path "FIFO_File/"

void Settings();
void int_array_printf(int *data, int size);
void int_array_to_string(char* message, int* data);
void string_to_int_array(const char* message, int* result, int size);
void Exit(int signum);
int loop_mode = 0;

int fd[fd_n];
char* fifo_path[fd_n] = {"serial_fifo", "web_send", "web_receive", "keyboard_fifo", "keyboard_feedback_fifo"};
char fifo_mode[fd_n] = {'w', 'w', 'r', 'r', 'w'};

int main(){
    char message[BUFFER_SIZE];
    int data[DATA_SIZE] = {90, 90, 90, 90, 90};

    signal(SIGINT, Exit);
    signal(SIGTERM, Exit);
    Settings();

    while (1){
        int_array_to_string(message, data);
        write(fd[3], message, strlen(message));

        memset(message, 0, sizeof(message));
        if (read(fd[3], message, sizeof(message) - 1) > 0){
            string_to_int_array(message, data, DATA_SIZE);
            //int_array_printf(data, DATA_SIZE);
        }

        write(fd[1], message, strlen(message));

        memset(message, 0, sizeof(message));
        if (read(fd[2], message, sizeof(message) - 1) > 0){
            string_to_int_array(message, data, DATA_SIZE);
            int_array_printf(data, DATA_SIZE);
        }
        usleep(50000);
    }

    return 0;
}

void Settings(){
    for (int i = 0; i < fd_n; i++){
        char path[BUFFER_SIZE];
        sprintf(path, "%s%s", previous_path, fifo_path[i]);
        if (access(path, F_OK) != -1){
            printf("%s FIFO file exists.\n", fifo_path[i]);
        }
        else{
            printf("%s FIFO file does not exist. Creating %s FIFO...\n", fifo_path[i], fifo_path[i]);
            if (mkfifo(path, 0666) == -1){
                perror("mkfifo");
                exit(EXIT_FAILURE);
            }
            printf("%s FIFO file created.\n", fifo_path[i]);
        }

        if (fifo_mode[i] == 'w'){
            fd[i] = open(path, O_WRONLY);
        }
        else{
            fd[i] = open(path, O_RDONLY);
        }
        if (fd[i] == -1){
            perror("open");
            exit(EXIT_FAILURE);
        }
    }
}

void int_array_to_string(char* message, int* data){
    snprintf(message, BUFFER_SIZE, "%d,%d,%d,%d,%d\n", data[0], data[1], data[2], data[3], data[4]);
}

void string_to_int_array(const char* message, int* result, int size){
    char temp_str[BUFFER_SIZE];
    strcpy(temp_str, message);
    temp_str[sizeof(temp_str)-1] = '\0';

    char* token = strtok(temp_str, ",");
    int i = 0;
    while (token != NULL && i < size){
        result[i++] = atoi(token);
        token = strtok(NULL, ",");
    }
}

void int_array_printf(int *data, int size){
    int i;
    for(i = 0; i < size; i++){
        printf("%d ", data[i]);
    }
    printf("\n");
}

void Exit(int signum){
    printf("Program Exit\n");
    for(int i = 0; i < fd_n; i++){
        close(fd[i]);
    }
    exit(signum);
}