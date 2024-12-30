#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define DATA_SIZE 5
#define BUFFER_SIZE 1000
#define FIFO_PATH "FIFO_File/test_fifo"

void int_array_printf(int *data, int size);
void fifo_check_make();
void loop_move(char* message, int* data);
void string_to_int_array(const char* message, int* result, int size);
int loop_mode = 0;

int main(){
    int serial_fd, keyboard_fd, keyboard_feedback_fd;
    char message[BUFFER_SIZE];
    int data[DATA_SIZE] = {90, 90, 90, 90, 90};

    fifo_check_make();
    int_array_printf(data, DATA_SIZE);

    serial_fd = open(FIFO_PATH, O_WRONLY);
    if (serial_fd == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    while (1){
        // loop_move(message, data);
        // write(serial_fd, message, strlen(message));  // loop function
        write(keyboard_feedback_fd, message, strlen(message));

        memset(message, 0, sizeof(message));
        if (read(keyboard_fd, message, sizeof(message) - 1) > 0){
            //printf("Received: %s", message);
            string_to_int_array(message, data, DATA_SIZE);
            //int_array_printf(data, DATA_SIZE);
        }

        usleep(50000);
    }

    close(serial_fd);
    close(keyboard_fd);
    close(keyboard_feedback_fd);

    return 0;
}

void int_array_printf(int *data, int size){
    int i;
    for(i = 0; i < size; i++){
        printf("%d ", data[i]);
    }
    printf("\n");
}

void fifo_check_make(){
    if (access(FIFO_PATH, F_OK) != -1){
        printf("FIFO file exists.\n");
    }
    else{
        printf("FIFO file does not exist. Creating FIFO...\n");
        if (mkfifo(FIFO_PATH, 0666) == -1){
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
        printf("FIFO file created.\n");
    }
}


void loop_move(char* message, int* data){
    if (loop_mode == 0){
        data[0]++;
        // data[1]++;
        // data[2]++;
        // data[3]++;
        // data[4]++;
    }
    else if (loop_mode == 1){
        data[0]--;
        // data[1]--;
        // data[2]--;
        // data[3]--;
        // data[4]--;
    }
    if (data[0] >= 180){
        loop_mode = 1;
    }
    if (data[0] <= 0){
        loop_mode = 0;
    }
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