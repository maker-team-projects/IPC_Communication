// shared_memory_writer.cpp
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main() {
    const char *shm_name = "/my_shared_memory";
    const size_t SIZE = 1024;

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

    // 공유 메모리에 데이터 쓰기
    const char *message = "Hello from C++!";
    std::memcpy(ptr, message, std::strlen(message) + 1);

    // 공유 메모리 연결 해제
    munmap(ptr, SIZE);
    close(shm_fd);

    return 0;
}
