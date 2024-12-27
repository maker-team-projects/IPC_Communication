import time
import ctypes
import sysv_ipc

class SharedData(ctypes.Structure):
    _fields_ = [("seq_number", ctypes.c_int),
                ("valid", ctypes.c_int),
                ("message", ctypes.c_char * (1024 - 2 * ctypes.sizeof(ctypes.c_int)))]

def write_to_shared_memory():
    key = 1234  # SHM_KEY와 일치시킴
    sem_key = 5678  # SEM_KEY와 일치시킴
    shm = sysv_ipc.SharedMemory(key, flags=sysv_ipc.IPC_CREAT, size=1024)
    sem = sysv_ipc.Semaphore(sem_key, flags=sysv_ipc.IPC_CREAT, initial_value=1)
    
    seq_number = 0

    while True:
        # P 연산 (Wait)
        sem.acquire()

        buffer = shm.read()
        data = SharedData.from_buffer_copy(buffer)
        if data.valid == 0:
            # 공유 메모리에 데이터 쓰기
            data = SharedData(seq_number, 1, bytes(f"[{seq_number}] Hello from Python!", 'utf-8'))
            shm.write(data)
            print(f"Writing data to shared memory: [{data.seq_number}] {data.message.decode('utf-8')}")

        # V 연산 (Signal)
        sem.release()

        seq_number += 1

if __name__ == '__main__':
    write_to_shared_memory()
