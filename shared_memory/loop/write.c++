// shared_memory_writer.cpp
#include <iostream>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

#define BUFFER_SIZE 1000

const char *shm_name = "/my_shared_memory";
const size_t SIZE = 1024;
int shm_fd;
void *ptr;

void Exit();

int main() {
    const char *shm_name = "/my_shared_memory";
    const size_t SIZE = 1024;
    char message[BUFFER_SIZE];
    int loop = 0;

    // 공유 메모리 생성
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        fprintf(stderr, "Shared memory creation failed\n");
        return 1;
    }

    // 공유 메모리 크기 설정
    ftruncate(shm_fd, SIZE);

    // 공유 메모리 매핑
    void *ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "Memory mapping failed\n");
        return 1;
    }

    atexit(Exit);

    while (1){
        // 공유 메모리에 데이터 쓰기
        sprintf(message, "[%d] Hello from C++!", loop);
        std::memset(ptr, 0, SIZE);
        std::memcpy(ptr, message, std::strlen(message) + 1);
        loop++;
    }
    return 0;
}

void Exit(){
    printf("Program Exit\n");
    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(shm_name);
}