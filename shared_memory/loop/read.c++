#include <iostream>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <csignal>

const char *shm_name = "/my_shared_memory";
const size_t SIZE = 1024;
int shm_fd;
void *ptr;

void Exit(int signum);

int main() {
    signal(SIGINT, Exit);
    signal(SIGTERM, Exit);

    // 공유 메모리 열기
    shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        fprintf(stderr, "Shared memory open failed\n");
        return 1;
    }

    // 공유 메모리 매핑
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "Memory mapping failed\n");
        return 1;
    }

    // 공유 메모리에서 데이터 읽기
    while (1){
        printf("Data read from shared memory: %s\n", static_cast<char*>(ptr));
    }
    return 0;
}

void Exit(int signum){
    printf("Program Exit\n");
    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(shm_name);
    exit(signum);
}