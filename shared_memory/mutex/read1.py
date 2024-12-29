import multiprocessing
import time
import ctypes
import sysv_ipc

class SharedData(ctypes.Structure):
    _fields_ = [("seq_number", ctypes.c_int),
                ("valid", ctypes.c_int),
                ("message", ctypes.c_char * (1024 - 2 * ctypes.sizeof(ctypes.c_int)))]

def read_from_shared_memory(lock):
    key = 1235  # SHM_KEY와 일치시킴
    sem_key = 5679  # SEM_KEY와 일치시킴
    shm = sysv_ipc.SharedMemory(key)
    sem = sysv_ipc.Semaphore(sem_key)
    
    last_seq_number = -1

    while True:
        with lock:
            # P 연산 (Wait)
            sem.acquire()

            # 공유 메모리에서 데이터 읽기
            buffer = shm.read()
            data = SharedData.from_buffer_copy(buffer)
            if data.valid == 1 and data.seq_number != last_seq_number:
                last_seq_number = data.seq_number
                message = data.message.decode('utf-8', errors='ignore')
                print(f"Received data from C++ [Seq: {data.seq_number}]: {message}")
                data.valid = 0  # 데이터 사용 후 무효 상태로 설정
                # 데이터를 다시 공유 메모리에 씀
                shm.write(data)

            # V 연산 (Signal)
            sem.release()

if __name__ == '__main__':
    # 락을 Manager로 설정
    manager = multiprocessing.Manager()
    lock = manager.Lock()

    # 프로세스 시작
    p = multiprocessing.Process(target=read_from_shared_memory, args=(lock,))
    p.start()
    p.join()
