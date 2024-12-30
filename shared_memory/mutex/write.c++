#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <csignal>
#include <stdio.h>

#define MEMORY_N 2
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

void Settings(int index);
void Data_send(int index, char send_message[SHM_SIZE - sizeof(int) * 2]);
void Exit(int signum);

SharedData *shared_mem[MEMORY_N];
SharedData data[MEMORY_N];
int shm_key[MEMORY_N] = {1234, 1235};
int sem_key[MEMORY_N] = {5678, 5679};
int shmid[MEMORY_N], semid[MEMORY_N];
int seq[MEMORY_N] = {0,};
union semun arg[MEMORY_N];
struct sembuf p_op[MEMORY_N], v_op[MEMORY_N];

int main() {
    // Shared Memory 생성 및 매핑
    int loop = 0;
    char message[SHM_SIZE - sizeof(int) * 2];

    signal(SIGINT, Exit);
    signal(SIGTERM, Exit);

    Settings(0);
    Settings(1);

    strcpy(message, "Start 0");
    Data_send(0, message);
    strcpy(message, "Start 1");
    Data_send(1, message);

    // 데이터 쓰기
    while (true) {
        sprintf(message, "[%d] Hello from C++ 0", loop);
        Data_send(0, message);
        sprintf(message, "[%d] Hello from C++ 1", loop);
        Data_send(1, message);
        loop++;
    }
    return 0;
}

void Settings(int index){
    shmid[index] = shmget(shm_key[index], SHM_SIZE, IPC_CREAT | 0666);
    if (shmid[index] < 0) {
        perror("shmget");
        exit(1);
    }
    shared_mem[index] = (SharedData *)shmat(shmid[index], NULL, 0);
    if (shared_mem[index] == (SharedData *)-1) {
        perror("shmat");
        exit(1);
    }

    // Semaphore 생성 및 초기화
    semid[index] = semget(sem_key[index], 1, IPC_CREAT | 0666);
    if (semid[index] < 0) {
        perror("semget");
        exit(1);
    }

    arg[index].val = 1;
    if (semctl(semid[index], 0, SETVAL, arg[index]) < 0) {
        perror("semctl");
        exit(1);
    }

    // P 연산 (Wait, Mutex 획득)
    p_op[index].sem_num = 0;
    p_op[index].sem_op = -1; // decrement the semaphore
    p_op[index].sem_flg = SEM_UNDO;

    // V 연산 (Signal, Mutex 해제)
    v_op[index].sem_num = 0;
    v_op[index].sem_op = 1; // increment the semaphore
    v_op[index].sem_flg = SEM_UNDO;

    data[index].valid = 0;
}

void Data_send(int index, char send_message[SHM_SIZE - sizeof(int) * 2]){
    // P 연산 수행
    if (semop(semid[index], &p_op[index], 1) < 0) {
        perror("semop P");
        exit(1);
    }
    
    if (shared_mem[index]->valid == 0){
        printf("Writing data to %d index shared memory...\n", index);
        data[index].seq_number = seq[index];
        data[index].valid = 1;
        strcpy(data[index].message, send_message);
        *shared_mem[index] = data[index];
        seq[index]++;
    }

    // V 연산 수행
    if (semop(semid[index], &v_op[index], 1) < 0) {
        perror("semop V");
        exit(1);
    }
}

void Exit(int signum){
    printf("Program Exit\n");
    for (int index = 0; index < MEMORY_N; index++){
        if (shmdt(shared_mem[index]) < 0) {
            perror("shmdt");
            exit(1);
        }
        if (shmctl(shmid[index], IPC_RMID, NULL) < 0) {
            perror("shmctl");
            exit(1);
        }
        if (semctl(semid[index], 0, IPC_RMID, arg) < 0) {
            perror("semctl IPC_RMID");
            exit(1);
        }
    }
    exit(signum);
}