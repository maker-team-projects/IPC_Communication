#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO_PATH "serial_fifo"

int main() {
    char buffer[128];
    int fd;

    // FIFO 파일 열기 (읽기 전용)
    fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        if (read(fd, buffer, sizeof(buffer) - 1) > 0) {
            printf("Consumed: %s", buffer);
        }
        sleep(1);  // 1초 대기
    }

    close(fd);
    return 0;
}
