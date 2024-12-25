#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_PATH "serial_fifo"

void create_fifo(const char *fifo_path) {
    if (access(fifo_path, F_OK) != -1) {
        printf("FIFO file %s exists.\n", fifo_path);
    } else {
        printf("FIFO file %s does not exist. Creating FIFO...\n");
        if (mkfifo(fifo_path, 0666) == -1) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
        printf("FIFO file %s created.\n", fifo_path);
    }
}

int main() {
    int fd;
    char message[100];
    int counter = 0;

    // 네임드 파이프 생성
    create_fifo(FIFO_PATH);

    // 파이프 열기 (쓰기 모드)
    fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 지속적인 데이터 전송
    while (1) {
        snprintf(message, sizeof(message), "Message %d from C\n", counter++);
        write(fd, message, strlen(message) + 1);  // null-terminator 포함
        usleep(500000);  // 0.5초 대기
    }

    // 파이프 닫기 (무한 루프에서 벗어나지 않으면 도달하지 않음)
    close(fd);

    return 0;
}
