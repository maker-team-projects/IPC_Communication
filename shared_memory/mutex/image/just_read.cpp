#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <string>

#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <cstring>
#include <csignal>
#include <cstdlib>
#include <stdio.h>

using namespace cv;
using namespace std;

#define MEMORY_N 1
#define SHM_SIZE 2097152

union semun_arg {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

struct SharedData {
    int seq_number;
    char message[SHM_SIZE - sizeof(int) * 2];
};

void Settings(int index);
char *Data_receive(int index);
bool is_base64( unsigned char c );
string base64_decode(string const& encoded_string);
Mat str2mat(const string& s);
void Exit(int signum);
void Exit();

SharedData *shared_mem[MEMORY_N];
int shm_key[MEMORY_N] = {1232};
int sem_key[MEMORY_N] = {5679};
int shmid[MEMORY_N], semid[MEMORY_N];
int seq[MEMORY_N] = {0,};
int last_seq_number[MEMORY_N] = {-1,};
union semun_arg shared_arg[MEMORY_N];
struct sembuf p_op[MEMORY_N], v_op[MEMORY_N];
static const string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

int main() {    
    char message[SHM_SIZE - sizeof(int) * 2];

    signal(SIGINT, Exit);
    signal(SIGTERM, Exit);
    atexit(Exit);

    Settings(0);

    while (true) {
        strcpy(message, Data_receive(0));
        Mat img = str2mat(message);
        imshow("Read_Image", img);
        if (waitKey(1) == 27){
            break;
        }
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

    shared_arg[index].val = 1;
    if (semctl(semid[index], 0, SETVAL, shared_arg[index]) < 0) {
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
}

char *Data_receive(int index){
    if (semop(semid[index], &p_op[index], 1) < 0) {
        perror("semop P");
        exit(1);
    }

    if (shared_mem[index]->seq_number != last_seq_number[index]) {
        last_seq_number[index] = shared_mem[index]->seq_number;
    }

    // V 연산 수행
    if (semop(semid[index], &v_op[index], 1) < 0) {
        perror("semop V");
        exit(1);
    }
    return shared_mem[index]->message;
}

bool is_base64( unsigned char c ) 
{ 
	return (isalnum(c) || (c == '+') || (c == '/'));
}

string base64_decode(string const& encoded_string)
{
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) 
	{
		char_array_4[i++] = encoded_string[in_]; in_++;

		if (i == 4) 
		{
			for (i = 0; i < 4; i++) 
			{	
				char_array_4[i] = base64_chars.find(char_array_4[i]);
			}

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
			{
				ret += char_array_3[i];
			}

			i = 0;
		}
	}

	if (i) 
	{
		for (j = i; j < 4; j++) 
		{
			char_array_4[j] = 0;
		}
		
		for (j = 0; j < 4; j++) 
		{	
			char_array_4[j] = base64_chars.find(char_array_4[j]);
		}

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++)
		{	
			ret += char_array_3[j];
		}
	}

	return ret;
}

Mat str2mat(const string& s)
{
	// Decode data
	string decoded_string = base64_decode(s);
	vector<uchar> data(decoded_string.begin(), decoded_string.end());

	Mat img = imdecode(data, IMREAD_UNCHANGED);
	return img;
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
        if (semctl(semid[index], 0, IPC_RMID, shared_arg) < 0) {
            perror("semctl IPC_RMID");
            exit(1);
        }
    }
    exit(signum);
}

void Exit(){
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
        if (semctl(semid[index], 0, IPC_RMID, shared_arg) < 0) {
            perror("semctl IPC_RMID");
            exit(1);
        }
    }
}