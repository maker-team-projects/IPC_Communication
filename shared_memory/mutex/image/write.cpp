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
    int valid;  // 상태 플래그 추가
    char message[SHM_SIZE - sizeof(int) * 2];
};

void Settings(int index);
void Data_send(int index, char send_message[SHM_SIZE - sizeof(int) * 2]);
string base64_encode(uchar const* bytes_to_encode, unsigned int in_len);
string mat2str(const Mat& m);
void Exit(int signum);
void Exit();

SharedData *shared_mem[MEMORY_N];
SharedData shared_data[MEMORY_N];
int shm_key[MEMORY_N] = {1232};
int sem_key[MEMORY_N] = {5679};
int shmid[MEMORY_N], semid[MEMORY_N];
int seq[MEMORY_N] = {0,};
union semun_arg shared_arg[MEMORY_N];
struct sembuf p_op[MEMORY_N], v_op[MEMORY_N];
static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

int main() {
    // Shared Memory 생성 및 매핑
    int loop = 0;
    char message[SHM_SIZE - sizeof(int) * 2];
    Mat img;

    signal(SIGINT, Exit);
    signal(SIGTERM, Exit);
    atexit(Exit);

    Settings(0);

    VideoCapture cap(0);

	cap.set(CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CAP_PROP_FRAME_HEIGHT, 480);
	
	if (!cap.isOpened()) {	// isOpened() : 카메라 정보가 있는지 없는지 (1: 있다, 0: 없다)
		printf("Can't open the camera");
		return -1;
	}

    // 데이터 쓰기
    while (true) {
        cap >> img;
        string base64 = mat2str(img);
        size_t base64_len = base64.length();
        if (base64_len > SHM_SIZE - sizeof(int) * 2){
            cout << "memory_over " << base64_len << endl;
            break;
        }
        strcpy(message, base64.c_str());
        Data_send(0, message);
        imshow("Write_Image", img);
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

    shared_data[index].valid = 0;
}

void Data_send(int index, char send_message[SHM_SIZE - sizeof(int) * 2]){
    // P 연산 수행
    if (semop(semid[index], &p_op[index], 1) < 0) {
        perror("semop P");
        exit(1);
    }
    
    if (shared_mem[index]->valid == 0){
        printf("Writing data to %d index shared memory...\n", index);
        shared_data[index].seq_number = seq[index];
        shared_data[index].valid = 1;
        strcpy(shared_data[index].message, send_message);
        *shared_mem[index] = shared_data[index];
        seq[index]++;
    }

    // V 연산 수행
    if (semop(semid[index], &v_op[index], 1) < 0) {
        perror("semop V");
        exit(1);
    }
}

string base64_encode(uchar const* bytes_to_encode, unsigned int in_len) 
{
	string ret;

	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) 
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4); i++) 
			{
				ret += base64_chars[char_array_4[i]];
			}
			i = 0;
		}
	}

	if (i) 
	{
		for (j = i; j < 3; j++) 
		{
			char_array_3[j] = '\0';
		}

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++) 
		{
			ret += base64_chars[char_array_4[j]];
		}
		
		while ((i++ < 3)) 
		{
			ret += '=';
		}
	}

	return ret;
}

string mat2str(const Mat& m)
{
	int params[3] = {0};
	params[0] = IMWRITE_JPEG_QUALITY;
	params[1] = 100;

	vector<uchar> buf;
	bool code = cv::imencode(".jpg", m, buf, std::vector<int>(params, params+2));
	uchar* result = reinterpret_cast<uchar*> (&buf[0]);

	return base64_encode(result, buf.size());

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