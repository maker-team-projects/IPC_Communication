// shared_memory_reader.cpp
#include <iostream>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main() {
    const char *shm_name = "/my_shared_memory";
    const size_t SIZE = 1024;

    // 공유 메모리 열기
    int shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        fprintf(stderr, "Shared memory open failed\n");
        return 1;
    }

    // 공유 메모리 매핑
    void *ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "Memory mapping failed\n");
        return 1;
    }

    // 공유 메모리에서 데이터 읽기
    printf("Data read from shared memory: %s\n", static_cast<char*>(ptr));

    // 공유 메모리 연결 해제
    munmap(ptr, SIZE);
    close(shm_fd);

    return 0;
}
