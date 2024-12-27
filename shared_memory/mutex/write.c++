#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <stdio.h>

#define SHM_KEY 1234
#define SEM_KEY 5678
#define SHM_SIZE 1024

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

struct SharedData {
    int seq_number;
    int valid;  // 상태 플래그 추가
    char message[SHM_SIZE - sizeof(int) * 2];
};

int main() {
    // Shared Memory 생성 및 매핑
    int loop = 0;
    SharedData data;

    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    SharedData *shared_mem = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_mem == (SharedData *)-1) {
        perror("shmat");
        exit(1);
    }

    // Semaphore 생성 및 초기화
    int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget");
        exit(1);
    }
    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) < 0) {
        perror("semctl");
        exit(1);
    }

    // P 연산 (Wait, Mutex 획득)
    struct sembuf p_op;
    p_op.sem_num = 0;
    p_op.sem_op = -1; // decrement the semaphore
    p_op.sem_flg = SEM_UNDO;

    // V 연산 (Signal, Mutex 해제)
    struct sembuf v_op;
    v_op.sem_num = 0;
    v_op.sem_op = 1; // increment the semaphore
    v_op.sem_flg = SEM_UNDO;

    // P 연산 수행
    if (semop(semid, &p_op, 1) < 0) {
        perror("semop P");
        exit(1);
    }
    
    printf("Writing data to shared memory...\n");
    data.seq_number = loop;
    data.valid = 1;  // 데이터 유효 상태 설정
    sprintf(data.message, "[%d] Hello from C++", loop);
    *shared_mem = data;
    loop++;

    // V 연산 수행
    if (semop(semid, &v_op, 1) < 0) {
        perror("semop V");
        exit(1);
    }

    // 데이터 쓰기
    while (true) {
        // P 연산 수행
        if (semop(semid, &p_op, 1) < 0) {
            perror("semop P");
            exit(1);
        }
        if (shared_mem->valid == 0){
            printf("Writing data to shared memory...\n");
            data.seq_number = loop;
            data.valid = 1;  // 데이터 유효 상태 설정
            sprintf(data.message, "[%d] Hello from C++", loop);
            *shared_mem = data;
            loop++;
        }
        // V 연산 수행
        if (semop(semid, &v_op, 1) < 0) {
            perror("semop V");
            exit(1);
        }
    }

    // Shared Memory 해제
    if (shmdt(shared_mem) < 0) {
        perror("shmdt");
        exit(1);
    }
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        perror("shmctl");
        exit(1);
    }
    if (semctl(semid, 0, IPC_RMID, arg) < 0) {
        perror("semctl IPC_RMID");
        exit(1);
    }

    return 0;
}
